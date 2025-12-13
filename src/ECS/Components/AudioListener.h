#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

namespace ECS::Components {
    struct AudioListener {
        // By default, the listener is active.
        // We can use this to disable it if we have multiple cameras/listeners.
        bool active = true;
    };
}

#endif // AUDIOLISTENER_H