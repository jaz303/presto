#include <v8.h>

#include <allegro5/allegro.h>

using namespace v8;

Handle<Value> createDisplay(const Arguments &args) {
    al_create_display(800, 600);
    return HandleScope().Close(Undefined());
}

Handle<Value> inhibitScreensaver(const Arguments &args) {
    HandleScope _;
    bool inhibit = (args.Length() == 0) || args[0]->ToBoolean()->IsTrue();
    return _.Close(al_inhibit_screensaver(inhibit) ? True() : False());
}