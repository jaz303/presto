#include "target.h"
#include "helpers.h"
#include "bitmap.h"
#include "api.h"

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

using namespace v8;

PS_DECLARE_KEY(target_x);
PS_DECLARE_KEY(target_y);
PS_DECLARE_KEY(target_width);
PS_DECLARE_KEY(target_height);

//
// 

static Persistent<Value> targetBitmap;

static Handle<Value> getTargetBitmap(const Arguments& args) {
    return HandleScope().Close(targetBitmap);
}

static Handle<Value> setTargetBitmap(const Arguments& args) {

    if (!PSTarget::setTargetPSBitmap(args[0])) {
        THROW("setTargetBitmap() requires either a PSBitmap instance or null as an argument");
    }
    
    return UNDEFINED();

}

static Handle<Value> clear(const Arguments& args) {
    al_clear_to_color(mapColor(args[0]));
    return UNDEFINED();
}

static Handle<Value> drawPixel(const Arguments& args) {
    al_draw_pixel(  F_ARG(0, x),
                    F_ARG(1, y),
                    C_ARG(2, color) );
    return UNDEFINED();
}

static Handle<Value> putPixel(const Arguments& args) {
    al_put_pixel(   I_ARG(0, x),
                    I_ARG(1, y),
                    C_ARG(2, color) );
    return UNDEFINED();
}

static Handle<Value> putBlendedPixel(const Arguments& args) {
    al_put_blended_pixel(   I_ARG(0, x),
                            I_ARG(1, y),
                            C_ARG(2, color) );
    return UNDEFINED();
}

static Handle<Value> getClippingRectangle(const Arguments& args) {
    
    HandleScope _;
    
    Handle<Object> rect = Object::New();

    int x, y, w, h;
    al_get_clipping_rectangle(&x, &y, &w, &h);

    rect->Set(key_target_x, Number::New(x));
    rect->Set(key_target_y, Number::New(y));
    rect->Set(key_target_width, Number::New(w));
    rect->Set(key_target_height, Number::New(h));

    return _.Close(rect);

}

static Handle<Value> setClippingRectangle(const Arguments& args) {
    
    HandleScope _;

    float x, y, w, h;

    if (args.Length() == 1) {

        Handle<Object> obj = args[0]->ToObject();

        x = obj->Get(key_target_x)->NumberValue();
        y = obj->Get(key_target_y)->NumberValue();
        w = obj->Get(key_target_width)->NumberValue();
        h = obj->Get(key_target_height)->NumberValue();

    } else if (args.Length() == 4) {

        x = args[0]->NumberValue();
        y = args[1]->NumberValue();
        w = args[2]->NumberValue();
        h = args[3]->NumberValue();

    } else {

        THROW("setClippingRectangle() requires either 1 or 4 arguments");
    
    }

    al_set_clipping_rectangle(x, y, w, h);

    return _.Close(Undefined());

}

static Handle<Value> resetClippingRectangle(const Arguments& args) {

    HandleScope _;

    ALLEGRO_BITMAP *target = al_get_target_bitmap();
    
    int w = al_get_bitmap_width(target),
        h = al_get_bitmap_height(target);

    al_set_clipping_rectangle(0, 0, w, h);

    return _.Close(Undefined());

}

static Handle<Value> setBlender(const Arguments& args) {

    HandleScope _;

    if (args.Length() == 3) {

        int op      = args[0]->NumberValue(),
            src     = args[1]->NumberValue(),
            dest    = args[2]->NumberValue();

        al_set_blender(op, src, dest);

    } else if (args.Length() == 6) {

        int op      = args[0]->NumberValue(),
            src     = args[1]->NumberValue(),
            dest    = args[2]->NumberValue(),
            a_op    = args[3]->NumberValue(),
            a_src   = args[4]->NumberValue(),
            a_dest  = args[5]->NumberValue();
            
        al_set_separate_blender(op, src, dest, a_op, a_src, a_dest);

    } else {

        THROW("setBlender() requires either 3 or 6 arguments");
    
    }

    return _.Close(Undefined());

}

static Handle<Value> isBitmapCompatibleWithCurrentDisplay(const Arguments& args) {

    HandleScope _;

    PSBitmap *psb = node::ObjectWrap::Unwrap<PSBitmap>(args[0]->ToObject());
    if (psb == 0) {
        THROW("isBitmapCompatibleWithCurrentDisplay() requires a PSBitmap instance as an argument");
    }

    bool compat = al_is_compatible_bitmap(psb->allegroBitmap());

    return BOOL(compat);

}

static Handle<Value> deferDrawing(const Arguments& args) {

    if (args[0]->IsFunction()) {

        HandleScope _;

        bool drawingWasHeld = al_is_bitmap_drawing_held();
        al_hold_bitmap_drawing(true);

        Handle<Function> fun = Handle<Function>::Cast(args[0]);
        Handle<Value> args[0];

        TryCatch trycatch;
        Handle<Value> ret = fun->Call(Context::GetCurrent()->Global(), 0, args);

        al_hold_bitmap_drawing(drawingWasHeld);

        if (ret.IsEmpty()) {
            return trycatch.ReThrow();
        }

    } else {
        al_hold_bitmap_drawing(args[0]->BooleanValue());
    }

    return UNDEFINED();

}

//
//

bool PSTarget::setTargetPSBitmap(Handle<Value> bitmap)
{
    if (bitmap->IsObject()) {

        HandleScope _;

        Handle<Object> obj = bitmap->ToObject();
        PSBitmap *psb = node::ObjectWrap::Unwrap<PSBitmap>(obj);

        if (psb == 0)
            return false;

        al_set_target_bitmap(psb->allegroBitmap());
        targetBitmap = Persistent<Value>::New(obj);

        return true;

    } else if (bitmap->IsNull() || bitmap->IsUndefined()) {
        
        al_set_target_bitmap(NULL);
        targetBitmap = Persistent<Value>::New(Null());

        return true;
    
    } else {
        return false;
    }
}

void PSTarget::init(Handle<Object> target)
{
    targetBitmap = Persistent<Value>::New(Null());

    PS_INIT_KEY(target_x, "x");
    PS_INIT_KEY(target_x, "y");
    PS_INIT_KEY(target_x, "width");
    PS_INIT_KEY(target_x, "height");

    NODE_SET_METHOD(target, "getTargetBitmap",                      getTargetBitmap);
    NODE_SET_METHOD(target, "setTargetBitmap",                      setTargetBitmap);
    NODE_SET_METHOD(target, "clear",                                clear);
    NODE_SET_METHOD(target, "drawPixel",                            drawPixel);
    NODE_SET_METHOD(target, "putPixel",                             putPixel);
    NODE_SET_METHOD(target, "putBlendedPixel",                      putBlendedPixel);
    NODE_SET_METHOD(target, "getClippingRect",                      getClippingRectangle);
    NODE_SET_METHOD(target, "setClippingRect",                      setClippingRectangle);
    NODE_SET_METHOD(target, "resetClippingRect",                    resetClippingRectangle);
    NODE_SET_METHOD(target, "setBlender",                           setBlender);
    NODE_SET_METHOD(target, "isBitmapCompatibleWithCurrentDisplay", isBitmapCompatibleWithCurrentDisplay);
    NODE_SET_METHOD(target, "deferDrawing",                         deferDrawing);
}