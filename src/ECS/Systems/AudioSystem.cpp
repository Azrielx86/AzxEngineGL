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
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    AudioEngine() {
        device = alcOpenDevice(nullptr);
        if (!device) {
            std::cerr << "ERROR: Failed to open OpenAL device." << std::endl;
            return;
        }

        context = alcCreateContext(device, nullptr);
        if (!context) {
            std::cerr << "ERROR: Failed to create OpenAL context." << std::endl;
            alcCloseDevice(device);
            return;
        }

        if (!alcMakeContextCurrent(context)) {
            std::cerr << "ERROR: Failed to make OpenAL context current." << std::endl;
            alcDestroyContext(context);
            alcCloseDevice(device);
            return;
        }

        alutInit(0, nullptr);
        std::cout << "OpenAL Initialized: " << alGetString(AL_VERSION) << std::endl;
    }

    ~AudioEngine() {
        alutExit();
        if (context) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(context);
        }
        if (device) {
            alcCloseDevice(device);
        }
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
            // glm::vec3 forward = transform.rotation * glm::vec3(0, 0, -1);
            // glm::vec3 up = transform.rotation * glm::vec3(0, 1, 0);
            // ALfloat orientation[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
            // alListenerfv(AL_ORIENTATION, orientation);
            
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
            if (source.filePath && source.buffer == 0) {
                source.buffer = alutCreateBufferFromFile(source.filePath);
                if (source.buffer == AL_NONE) {
                    std::cerr << "Error loading audio file: " << alutGetErrorString(alutGetError()) << std::endl;
                } else {
                    alGenSources(1, &source.source);
                    alSourcei(source.source, AL_BUFFER, source.buffer);
                }
            }
            
            alSourcef(source.source, AL_PITCH, source.pitch);
            alSourcef(source.source, AL_GAIN, source.gain);
            alSourcei(source.source, AL_LOOPING, source.loop);
            source.isDirty = false;
        }

        alSourcefv(source.source, AL_POSITION, glm::value_ptr(transform.translation));
        // TODO: Velocity should be calculated
        ALfloat velocity[] = { 0.0f, 0.0f, 0.0f };
        alSourcefv(source.source, AL_VELOCITY, velocity);
    }
}

} // namespace ECS::Systems