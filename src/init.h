#include <v8.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

using namespace v8;

#define BOOL(exp) (HandleScope().Close((exp) ? True() : False()));
#define UNDEF() (HandleScope().Close(Undefined()));

Handle<Value> ps_install(const Arguments& args) {
    return BOOL(al_install_system(ALLEGRO_VERSION_INT, NULL));
}

Handle<Value> ps_uninstall(const Arguments &args) {
    al_uninstall_system();
    return UNDEF();
}

Handle<Value> ps_install_joystick(const Arguments& args) {
    return BOOL(al_install_joystick());
}

Handle<Value> ps_uninstall_joystick(const Arguments &args) {
    al_uninstall_joystick();
    return UNDEF();
}

Handle<Value> ps_install_keyboard(const Arguments& args) {
    return BOOL(al_install_keyboard());
}

Handle<Value> ps_uninstall_keyboard(const Arguments &args) {
    al_uninstall_keyboard();
    return UNDEF();
}

Handle<Value> ps_install_mouse(const Arguments& args) {
    return BOOL(al_install_mouse());
}

Handle<Value> ps_uninstall_mouse(const Arguments &args) {
    al_uninstall_mouse();
    return UNDEF();
}

Handle<Value> ps_install_audio(const Arguments& args) {
    return BOOL(al_install_audio());
}

Handle<Value> ps_uninstall_audio(const Arguments &args) {
    al_uninstall_audio();
    return UNDEF();
}

Handle<Value> ps_install_font(const Arguments& args) {
    al_init_font_addon();
    return UNDEF();
}

Handle<Value> ps_uninstall_font(const Arguments &args) {
    al_shutdown_font_addon();
    return UNDEF();
}

Handle<Value> ps_install_image(const Arguments& args) {
    return BOOL(al_init_image_addon());
}

Handle<Value> ps_uninstall_image(const Arguments &args) {
    al_shutdown_image_addon();
    return UNDEF();
}

Handle<Value> ps_install_primitives(const Arguments& args) {
    return BOOL(al_init_primitives_addon());
}

Handle<Value> ps_uninstall_primitives(const Arguments &args) {
    al_shutdown_primitives_addon();
    return UNDEF();
}