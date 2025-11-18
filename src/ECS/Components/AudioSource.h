//
// Created by tohka on 11/15/25.
//

#ifndef PROYECTOFINAL_CGA_AUDIOSOURCE_H
#define PROYECTOFINAL_CGA_AUDIOSOURCE_H

#include <AL/al.h>

namespace ECS::Components
{
struct AudioSource
{
    ALint buffer;
    bool loop;
};
} // namespace ECS::Components

#endif // PROYECTOFINAL_CGA_AUDIOSOURCE_H
