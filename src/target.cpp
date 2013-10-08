#include "target.h"
#include "helpers.h"
#include "bitmap.h"

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

    HandleScope _;
    
    if (!PSTarget::setTargetPSBitmap(args[0])) {
        THROW("setTargetBitmap() requires either a PSBitmap object or null as an argument");
    }
    
    return _.Close(Undefined());

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

        x = obj->Get(key_target_x)->ToNumber()->Value();
        y = obj->Get(key_target_y)->ToNumber()->Value();
        w = obj->Get(key_target_width)->ToNumber()->Value();
        h = obj->Get(key_target_height)->ToNumber()->Value();

    } else if (args.Length() == 4) {

        x = args[0]->ToNumber()->Value();
        y = args[1]->ToNumber()->Value();
        w = args[2]->ToNumber()->Value();
        h = args[3]->ToNumber()->Value();

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

        int op      = args[0]->ToNumber()->Value(),
            src     = args[1]->ToNumber()->Value(),
            dest    = args[2]->ToNumber()->Value();

        al_set_blender(op, src, dest);

    } else if (args.Length() == 6) {

        int op      = args[0]->ToNumber()->Value(),
            src     = args[1]->ToNumber()->Value(),
            dest    = args[2]->ToNumber()->Value(),
            a_op    = args[3]->ToNumber()->Value(),
            a_src   = args[4]->ToNumber()->Value(),
            a_dest  = args[5]->ToNumber()->Value();
            
        al_set_separate_blender(op, src, dest, a_op, a_src, a_dest);

    } else {

        THROW("setBlender() requires either 3 or 6 arguments");
    
    }

    return _.Close(Undefined());

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

    NODE_SET_METHOD(target,     "getTargetBitmap",      getTargetBitmap);
    NODE_SET_METHOD(target,     "setTargetBitmap",      setTargetBitmap);
    NODE_SET_METHOD(target,     "getClippingRect",      getClippingRectangle);
    NODE_SET_METHOD(target,     "setClippingRect",      setClippingRectangle);
    NODE_SET_METHOD(target,     "resetClippingRect",    resetClippingRectangle);
    NODE_SET_METHOD(target,     "setBlender",           setBlender);
}