#ifndef __PS_KEYBOARD_H__
#define __PS_KEYBOARD_H__

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

using namespace v8;

class PSKeyboard {
public:
    static void init(Handle<Object> target);
};

class PSKeyboardState : node::ObjectWrap {
public:
    PSKeyboardState() { al_get_keyboard_state(&state_); }
    ~PSKeyboardState() { }
    
    static Persistent<FunctionTemplate> tpl;
    static Handle<Object> createInstance();

    static Handle<Value> IsKeyDown(const Arguments& args);
    static Handle<Value> Refresh(const Arguments& args);
    
private:
    ALLEGRO_KEYBOARD_STATE state_;
    
    PSKeyboardState(const PSKeyboardState&);
    void operator=(const PSKeyboardState&);
};

#endif