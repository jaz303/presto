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

