#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <AL/al.h>
#include <string>

namespace ECS::Components
{
struct AudioSource
{
    ALuint source = 0;
    ALuint buffer = 0;

    bool loop = false;
    float gain = 1.0f;
    float pitch = 1.0f;

    // Non-managed pointer to the audio data.
    // The AudioSystem will be responsible for loading and unloading this.
    const char *filePath = nullptr;

    // Dirty flag to signal the system to update the source properties
    bool isDirty = true;
};
} // namespace ECS::Components

#endif // AUDIOSOURCE_H