#ifndef __PS_MOUSE_H__
#define __PS_MOUSE_H__

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

using namespace v8;

class PSMouse {
public:
    static void init(Handle<Object> target);
};

class PSMouseCursor : node::ObjectWrap {
public:
    PSMouseCursor(ALLEGRO_MOUSE_CURSOR *cursor) : cursor_(cursor) {}
    ~PSMouseCursor() { al_destroy_mouse_cursor(cursor_); }

    ALLEGRO_MOUSE_CURSOR *allegroMouseCursor() const { return cursor_; }

    static Persistent<FunctionTemplate> tpl;
    static void init(Handle<Object> target);
    static Handle<Object> createInstance(ALLEGRO_MOUSE_CURSOR *cursor);
    
private:
    ALLEGRO_MOUSE_CURSOR *cursor_;

    PSMouseCursor(const PSMouseCursor&);
    void operator=(const PSMouseCursor&);
};

#endif
