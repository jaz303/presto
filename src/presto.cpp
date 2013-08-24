#include <v8.h>
#include <node.h>

#include "api.h"

#define EXPOSE_SUBYSTEM(name) \
    NODE_SET_METHOD(target, "ps_install_" #name, ps_install_##name); \
    NODE_SET_METHOD(target, "ps_uninstall_" #name, ps_uninstall_##name);

void init(Handle<Object> target) {

    PSEventQueue::init(target);
    PSDisplay::init(target);

    NODE_SET_METHOD(target, "ps_install", ps_install);
    NODE_SET_METHOD(target, "ps_uninstall", ps_uninstall);

    EXPOSE_SUBYSTEM(joystick);
    EXPOSE_SUBYSTEM(keyboard);
    EXPOSE_SUBYSTEM(mouse);
    EXPOSE_SUBYSTEM(audio);
    EXPOSE_SUBYSTEM(font);
    EXPOSE_SUBYSTEM(image);
    EXPOSE_SUBYSTEM(primitives);

    NODE_SET_METHOD(target, "createEventQueue", createEventQueue);

}

NODE_MODULE(presto_bindings, init);