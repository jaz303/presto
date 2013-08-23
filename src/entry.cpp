#include <v8.h>
#include <node.h>
#include <iostream>
#include "uv.h"

#include <allegro5/allegro.h>

#include "api.h"

using namespace v8;

static Persistent<Function> s_entryPoint;
static Persistent<Context>  s_context;

static int presto_main(int argc, char **argv) {

    {
        Locker locker(Isolate::GetCurrent());
        Isolate::Scope scope(Isolate::GetCurrent());

        s_context->Enter();
        HandleScope _;

        s_entryPoint->Call(s_context->Global(), 0, NULL);
        uv_run(uv_default_loop(), UV_RUN_DEFAULT);

        s_context->Exit();
    }

    return 0;

}

Handle<Value> run(const Arguments& args) {

    if (args.Length() == 0 || !args[0]->IsFunction()) {
        std::cerr << "error: presto_bindings.run() requires a function as its first argument" << std::endl;
        return HandleScope().Close(Undefined());
    }

    s_entryPoint = Persistent<Function>(Handle<Function>::Cast(args[0]));
    s_context = Persistent<Context>(Context::GetCurrent());
    s_context->Exit();

    {
        Unlocker unlocker(Isolate::GetCurrent());
        al_run_main(0, NULL, presto_main);
    }

    s_context->Enter();
    s_context.Dispose();
    s_entryPoint.Dispose();

    return HandleScope().Close(Undefined());

}

#define EXPOSE_SUBYSTEM(name) \
    NODE_SET_METHOD(target, "ps_install_" #name, ps_install_##name); \
    NODE_SET_METHOD(target, "ps_uninstall_" #name, ps_uninstall_##name);

void init(Handle<Object> target) {

    PSEventQueue::init(target);

    NODE_SET_METHOD(target, "run", run);

    NODE_SET_METHOD(target, "ps_install", ps_install);
    NODE_SET_METHOD(target, "ps_uninstall", ps_uninstall);

    EXPOSE_SUBYSTEM(joystick);
    EXPOSE_SUBYSTEM(keyboard);
    EXPOSE_SUBYSTEM(mouse);
    EXPOSE_SUBYSTEM(audio);
    EXPOSE_SUBYSTEM(font);
    EXPOSE_SUBYSTEM(image);
    EXPOSE_SUBYSTEM(primitives);

    NODE_SET_METHOD(target, "ps_create_display", ps_create_display);
    NODE_SET_METHOD(target, "ps_inhibit_screensaver", ps_inhibit_screensaver);

    NODE_SET_METHOD(target, "createEventQueue", createEventQueue);

}

NODE_MODULE(presto_bindings, init);
