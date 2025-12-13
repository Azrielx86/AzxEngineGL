#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "ECS/ISystem.h"

namespace ECS::Systems {
    class AudioSystem final : public ISystem {
    public:
        AudioSystem();
        ~AudioSystem() override;

        void Update(Registry& registry, float deltaTime) override;

    private:
        // Private implementation for OpenAL context and device
        class AudioEngine;
        AudioEngine* audioEngine;
    };
}

#endif // AUDIOSYSTEM_H