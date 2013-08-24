#include <v8.h>
#include <allegro5/allegro.h>

using namespace v8;

#define K_INT(k) \
    obj->Set(Handle<String>(String::New(#k)), Integer::New(k))
    
Handle<Value> installConstants(const Arguments& args) {

    HandleScope _;

    Handle<Object> obj = Handle<Object>::Cast(args[0]);

    //
    // Display flags

    K_INT(ALLEGRO_WINDOWED);
    K_INT(ALLEGRO_FULLSCREEN);
    K_INT(ALLEGRO_FULLSCREEN_WINDOW);
    K_INT(ALLEGRO_RESIZABLE);
    K_INT(ALLEGRO_OPENGL);
    K_INT(ALLEGRO_OPENGL_3_0);
    K_INT(ALLEGRO_OPENGL_FORWARD_COMPATIBLE);
#ifdef ALLEGRO_DIRECT3D
    K_INT(ALLEGRO_DIRECT3D);
#endif
    K_INT(ALLEGRO_NOFRAME);
    K_INT(ALLEGRO_GENERATE_EXPOSE_EVENTS);

    return _.Close(obj);

}
