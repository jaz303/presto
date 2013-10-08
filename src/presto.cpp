#include <v8.h>
#include <node.h>

#include "api.h"
#include "helpers.h"

PS_DECLARE_KEY(color_red);
PS_DECLARE_KEY(color_green);
PS_DECLARE_KEY(color_blue);
PS_DECLARE_KEY(color_alpha);

void initColorKeys() {
    PS_INIT_KEY(color_red,      "r");
    PS_INIT_KEY(color_green,    "g");
    PS_INIT_KEY(color_blue,     "b");
    PS_INIT_KEY(color_alpha,    "a");
}

ALLEGRO_COLOR mapColor(Handle<Value> color) {
    
    if (color->IsNumber()) {
        
        uint32_t argb = color->ToUint32()->Value();
        return al_map_rgba(argb >> 16, argb >> 8, argb, argb >> 24);
   
    } else if (color->IsObject()) {
        
        Handle<Object> co = Handle<Object>::Cast(color);

        float r = co->Get(key_color_red)->ToNumber()->Value(),
              g = co->Get(key_color_green)->ToNumber()->Value(),
              b = co->Get(key_color_blue)->ToNumber()->Value();

        float a = co->Has(key_color_alpha)
                    ? co->Get(key_color_alpha)->ToNumber()->Value()
                    : 1.0f;

        return al_map_rgba_f(r, g, b, a);

    } else {
        
        return al_map_rgb(0, 0, 0);
    
    }

}

#define EXPOSE_SUBYSTEM(name) \
    NODE_SET_METHOD(target, "ps_install_" #name, ps_install_##name); \
    NODE_SET_METHOD(target, "ps_uninstall_" #name, ps_uninstall_##name);

void init(Handle<Object> target) {

    initColorKeys();

    PSTarget::init(target);
    PSEventQueue::init(target);
    PSDisplay::init(target);
    PSBitmap::init(target);

    NODE_SET_METHOD(target, "installConstants", installConstants);

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