#include <v8.h>
#include <node.h>
#include <iostream>
#include <pthread.h>

#include <allegro5/allegro.h>

using namespace v8;

static Persistent<Function> s_entryPoint;

static int our_main(int argc, char **argv) {

    Isolate *isolate = Isolate::GetCurrent();

    {
        Locker locker(isolate);
        isolate->Enter();

        HandleScope _;

        al_init();

        s_entryPoint->Call(Context::GetCurrent()->Global(), 0, NULL);

        isolate->Exit();
    }

    return 1;

}

Handle<Value> run(const Arguments& args) {

    HandleScope _;

    if (!args[0]->IsFunction()) {
        std::cerr << "error: presto.run() requires a function as its first argument" << std::endl;
        return _.Close(Undefined());
    }

    Isolate *isolate = Isolate::GetCurrent();

    {
        isolate->Exit();
        Unlocker unlocker(isolate);

        s_entryPoint = Persistent<Function>(Handle<Function>::Cast(args[0]));
        al_run_main(0, NULL, our_main);
        s_entryPoint.Dispose();
    }

    isolate->Enter();

    return _.Close(Undefined());

}

void init(Handle<Object> target) {

    NODE_SET_METHOD(target, "run", run);

}

NODE_MODULE(presto_bindings, init);
