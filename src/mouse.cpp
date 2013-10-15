#include "mouse.h"
#include "api.h"
#include "helpers.h"

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

PS_DECLARE_KEY(mouse_x);
PS_DECLARE_KEY(mouse_y);
PS_DECLARE_KEY(mouse_w);
PS_DECLARE_KEY(mouse_z);
PS_DECLARE_KEY(mouse_buttons);

using namespace v8;

//
// Mouse

static Handle<Value> getMouseNumAxes(const Arguments& args) {
    return HandleScope().Close(Integer::New(al_get_mouse_num_axes()));
}

static Handle<Value> getMouseNumButtons(const Arguments& args) {
    return HandleScope().Close(Integer::New(al_get_mouse_num_buttons()));
}

static Handle<Value> getMouseState(const Arguments& args) {

    HandleScope _;

    Handle<Object> mouseState = Object::New();

    ALLEGRO_MOUSE_STATE alState;
    al_get_mouse_state(&alState);

    mouseState->Set(key_mouse_x,        Integer::New(alState.x));
    mouseState->Set(key_mouse_y,        Integer::New(alState.y));
    mouseState->Set(key_mouse_w,        Integer::New(alState.w));
    mouseState->Set(key_mouse_z,        Integer::New(alState.z));
    mouseState->Set(key_mouse_buttons,  Integer::New(alState.buttons));

    return _.Close(mouseState);

}

static Handle<Value> setMouseXY(const Arguments& args) {

    HandleScope _;

    PSDisplay *display = 0;

    if (args[0]->IsObject()) {
        Handle<Object> displayObj = Handle<Object>::Cast(args[0]);
        display = node::ObjectWrap::Unwrap<PSDisplay>(displayObj);
    }

    if (display == 0) {
        THROW("setMouseXY(): first argument must be a display");
    }

    bool success = al_set_mouse_xy(display->allegroDisplay(), I_ARG(1, x), I_ARG(2, y));

    return _.Close(success ? True() : False());

}

static Handle<Value> setMouseZ(const Arguments& args) {
    return BOOL(al_set_mouse_z(I_ARG(0, z)));
}

static Handle<Value> setMouseW(const Arguments& args) {
    return BOOL(al_set_mouse_w(I_ARG(0, z)));
}

static Handle<Value> setMouseAxis(const Arguments& args) {
    return BOOL(al_set_mouse_axis(I_ARG(0, which), I_ARG(1, value)));
}

void PSMouse::init(Handle<Object> target)
{
    NODE_SET_METHOD(target, "getMouseNumAxes",          getMouseNumAxes);
    NODE_SET_METHOD(target, "getMouseNumButtons",       getMouseNumButtons);
    NODE_SET_METHOD(target, "getMouseState",            getMouseState);
    NODE_SET_METHOD(target, "setMouseXY",               setMouseXY);
    NODE_SET_METHOD(target, "setMouseZ",                setMouseZ);
    NODE_SET_METHOD(target, "setMouseW",                setMouseW);
    NODE_SET_METHOD(target, "setMouseAxis",             setMouseAxis);

    PS_INIT_KEY(mouse_x,        "x");
    PS_INIT_KEY(mouse_y,        "y");
    PS_INIT_KEY(mouse_w,        "w");
    PS_INIT_KEY(mouse_z,        "z");
    PS_INIT_KEY(mouse_buttons,  "buttons");
}

//
// Cursor

static Handle<Value> createMouseCursor(const Arguments& args) {

    ALLEGRO_BITMAP *bitmap = 0;

    if (args[0]->IsObject()) {
        Handle<Object> bitmapObj = Handle<Object>::Cast(args[0]);
        PSBitmap *psBitmap = node::ObjectWrap::Unwrap<PSBitmap>(bitmapObj);
        if (psBitmap) {
            bitmap = psBitmap->allegroBitmap();
        }
    }

    if (bitmap == 0) {
        THROW("createMouseCursor(): first argument must be a bitmap");
    }

    ALLEGRO_MOUSE_CURSOR *cursor = al_create_mouse_cursor(bitmap, I_ARG(1, x_focus), I_ARG(2, y_focus));

    if (cursor == 0) {
        THROW("createMouseCursor(): al_create_mouse_cursor() failed");
    }

    return PSMouseCursor::createInstance(cursor);

}

static Handle<Value> getMouseCursorPosition(const Arguments& args) {

    HandleScope _;

    int x, y;
    if (al_get_mouse_cursor_position(&x, &y)) {

        Handle<Object> pos = Object::New();
        pos->Set(key_mouse_x, Integer::New(x));
        pos->Set(key_mouse_y, Integer::New(y));

        return _.Close(pos);

    } else {
        return _.Close(Null());
    }

}

void PSMouseCursor::init(Handle<Object> target) {
    
    HandleScope _;

    NODE_SET_METHOD(target, "createMouseCursor",        createMouseCursor);
    NODE_SET_METHOD(target, "getMouseCursorPosition",   getMouseCursorPosition);

    Local<FunctionTemplate> ft = FunctionTemplate::New();
    ft->SetClassName(String::NewSymbol("PSMouseCursor"));

    Handle<ObjectTemplate> it = ft->InstanceTemplate();
    it->SetInternalFieldCount(1);

    PSMouseCursor::tpl = Persistent<FunctionTemplate>::New(ft);

}

Handle<Object> PSMouseCursor::createInstance(ALLEGRO_MOUSE_CURSOR *cursor) {
    
    HandleScope _;

    Local<Object> instance = PSMouseCursor::tpl->InstanceTemplate()->NewInstance();
    
    PSMouseCursor *theCursor = new PSMouseCursor(cursor);
    theCursor->Wrap(instance);
    
    return _.Close(instance);

}
