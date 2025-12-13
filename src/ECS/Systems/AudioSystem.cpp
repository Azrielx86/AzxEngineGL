#include "AudioSystem.h"
#include "ECS/Components/AudioListener.h"
#include "ECS/Components/AudioSource.h"
#include "ECS/Components/Transform.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace ECS::Systems {

// Private implementation class for OpenAL context
class AudioSystem::AudioEngine {
public:
    AudioEngine() {
        // alutInit will create a context and make it current
        if (alutInit(0, nullptr) == AL_FALSE) {
            std::cerr << "ERROR: alutInit failed: " << alutGetErrorString(alutGetError()) << std::endl;
        } else {
            std::cout << "OpenAL Initialized: " << alGetString(AL_VERSION) << std::endl;
        }
    }

    ~AudioEngine() {
        alutExit();
    }
};

AudioSystem::AudioSystem() : audioEngine(new AudioEngine()) {}

AudioSystem::~AudioSystem() {
    delete audioEngine;
}

void AudioSystem::Update(Registry& registry, [[maybe_unused]] float deltaTime) {
    // Update Listener
    for (const auto& entity : registry.View<Components::AudioListener, Components::Transform>()) {
        auto& listener = registry.GetComponent<Components::AudioListener>(entity);
        if (listener.active) {
            auto& transform = registry.GetComponent<Components::Transform>(entity);
            alListenerfv(AL_POSITION, glm::value_ptr(transform.translation));
            
            // Orientation: first 'at' vector, then 'up' vector
            // TODO: Need to get the forward and up vectors from the transform component
            // For now, static orientation
            ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
            alListenerfv(AL_ORIENTATION, orientation);

            // TODO: Velocity should be calculated and stored in a component
            ALfloat velocity[] = { 0.0f, 0.0f, 0.0f };
            alListenerfv(AL_VELOCITY, velocity);
        }
    }

    // Update Sources
    for (const auto& entity : registry.View<Components::AudioSource, Components::Transform>()) {
        auto& source = registry.GetComponent<Components::AudioSource>(entity);
        auto& transform = registry.GetComponent<Components::Transform>(entity);

        if (source.isDirty) {
            if (source.filePath) {
                // Clear any previous error
                alGetError(); 
                
                ALuint newBuffer = alutCreateBufferFromFile(source.filePath);
                ALenum error = alutGetError();
                if (error != ALUT_ERROR_NO_ERROR) {
                    std::cerr << "Error loading audio file '" << source.filePath << "': " << alutGetErrorString(error) << std::endl;
                } else {
                    if (source.source == 0) {
                        alGenSources(1, &source.source);
                    }
                    
                    // Stop the source before changing the buffer
                    alSourceStop(source.source);

                    // If there was an old buffer, delete it
                    if (source.buffer != 0) {
                        alDeleteBuffers(1, &source.buffer);
                    }
                    source.buffer = newBuffer;
                    
                    alSourcei(source.source, AL_BUFFER, source.buffer);
                    alSourcef(source.source, AL_PITCH, source.pitch);
                    alSourcef(source.source, AL_GAIN, source.gain);
                    alSourcei(source.source, AL_LOOPING, source.loop);
                    
                    alSourcePlay(source.source);
                }
            }
            source.isDirty = false;
        }

        alSourcefv(source.source, AL_POSITION, glm::value_ptr(transform.translation));
        // TODO: Velocity should be calculated
        ALfloat velocity[] = { 0.0f, 0.0f, 0.0f };
        alSourcefv(source.source, AL_VELOCITY, velocity);
    }
}

} // namespace ECS::Systems