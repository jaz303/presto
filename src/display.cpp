#include <v8.h>

#include <allegro5/allegro.h>

using namespace v8;

Handle<Value> ps_create_display(const Arguments &args) {
    al_create_display(800, 600);
    return HandleScope().Close(Undefined());
}