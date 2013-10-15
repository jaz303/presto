#include "keyboard.h"
#include "api.h"
#include "helpers.h"

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

#define UNWRAP_SELF \
    PSKeyboardState *self = node::ObjectWrap::Unwrap<PSKeyboardState>(args.This())

using namespace v8;

static Handle<Value> getKeyboardState(const Arguments& args) {
    return PSKeyboardState::createInstance();
}

static Handle<Value> keycodeToName(const Arguments& args) {
    HandleScope _;
    const char *keyName = al_keycode_to_name(I_ARG(0, keycode));
    return _.Close(String::New(keyName));
}

static Handle<Value> setKeyboardLeds(const Arguments& args) {
    return BOOL(al_set_keyboard_leds(I_ARG(0, leds)));
}

void PSKeyboard::init(Handle<Object> target) {
    
    NODE_SET_METHOD(target, "getKeyboardState",     getKeyboardState);
    NODE_SET_METHOD(target, "keycodeToName",        keycodeToName);
    NODE_SET_METHOD(target, "setKeyboardLeds",      setKeyboardLeds);

    HandleScope _;

    Local<FunctionTemplate> ft = FunctionTemplate::New();
    ft->SetClassName(String::NewSymbol("PSKeyboardState"));

    Handle<ObjectTemplate> it = ft->InstanceTemplate();
    it->SetInternalFieldCount(1);

    PSKeyboardState::tpl = Persistent<FunctionTemplate>::New(ft);

}

Handle<Object> PSKeyboardState::createInstance() {
    
    HandleScope _;

    Local<Object> instance = PSKeyboardState::tpl->InstanceTemplate()->NewInstance();
    
    PSKeyboardState *theState = new PSKeyboardState();
    theState->Wrap(instance);
    
    return _.Close(instance);

}

Handle<Value> PSKeyboardState::IsKeyDown(const Arguments& args) {
    UNWRAP_SELF;
    return BOOL(al_key_down(&self->state_, I_ARG(0, keycode)));
}

Handle<Value> PSKeyboardState::Refresh(const Arguments& args) {
    UNWRAP_SELF;
    al_get_keyboard_state(&self->state_);
    return UNDEFINED();
}
