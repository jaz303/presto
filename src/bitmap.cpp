#include "bitmap.h"
#include "helpers.h"
#include <v8.h>
#include <allegro5/allegro.h>

using namespace v8;

#define UNWRAP_SELF \
    PSBitmap *self = node::ObjectWrap::Unwrap<PSBitmap>(args.This())

#define UNWRAP_SELF_PROP \
    PSBitmap *self = node::ObjectWrap::Unwrap<PSBitmap>(info.Holder())

PS_DECLARE_KEY(draw_scale);
PS_DECLARE_KEY(draw_tint);
PS_DECLARE_KEY(draw_rotate);
PS_DECLARE_KEY(draw_region);
PS_DECLARE_KEY(draw_flags);

PS_DECLARE_KEY(draw_x);
PS_DECLARE_KEY(draw_y);
PS_DECLARE_KEY(draw_dx);
PS_DECLARE_KEY(draw_dy);
PS_DECLARE_KEY(draw_dw);
PS_DECLARE_KEY(draw_dh);
PS_DECLARE_KEY(draw_sx);
PS_DECLARE_KEY(draw_sy);
PS_DECLARE_KEY(draw_sw);
PS_DECLARE_KEY(draw_sh);
PS_DECLARE_KEY(draw_cx);
PS_DECLARE_KEY(draw_cy);

//
// Static

Handle<Value> createBitmap(const Arguments &args) {

    HandleScope _;

    return _.Close(Null());

}

Handle<Value> loadBitmap(const Arguments &args) {

    if (args.Length() < 1)
        THROW("loadBitmap() requires one argument");

    const char *filename = *String::AsciiValue(args[0]);
    ALLEGRO_BITMAP *bitmap = al_load_bitmap(filename);

    if (bitmap) {
        return PSBitmap::createInstance(bitmap);
    } else {
        return HandleScope().Close(Null());    
    }

}

Persistent<FunctionTemplate> PSBitmap::tpl;

//
//

PSBitmap::PSBitmap(ALLEGRO_BITMAP *bitmap)
    : bitmap_(bitmap)
{
}

PSBitmap::~PSBitmap()
{
    al_destroy_bitmap(bitmap_);
}

void PSBitmap::init(Handle<Object> target)
{
    HandleScope _;

    NODE_SET_METHOD(target, "createBitmap", createBitmap);
    NODE_SET_METHOD(target, "loadBitmap", loadBitmap);

    Local<FunctionTemplate> ft = FunctionTemplate::New();
    ft->SetClassName(String::NewSymbol("PSBitmap"));

    Handle<ObjectTemplate> it = ft->InstanceTemplate();
    it->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(ft, "save", Save);
    NODE_SET_PROTOTYPE_METHOD(ft, "use", Use);

    NODE_SET_PROTOTYPE_METHOD(ft, "clearToColor",       ClearToColor);
    NODE_SET_PROTOTYPE_METHOD(ft, "draw",               Draw);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawFast",           DrawFast);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawRegion",         DrawRegion);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawPixel",          DrawPixel);
    NODE_SET_PROTOTYPE_METHOD(ft, "putPixel",           PutPixel);
    NODE_SET_PROTOTYPE_METHOD(ft, "putBlendedPixel",    PutBlendedPixel);

    PS_INIT_KEY(draw_scale,         "scale");
    PS_INIT_KEY(draw_tint,          "tint");
    PS_INIT_KEY(draw_rotate,        "rotate");
    PS_INIT_KEY(draw_region,        "region");
    PS_INIT_KEY(draw_flags,         "flags");

    PS_INIT_KEY(draw_x,             "x");
    PS_INIT_KEY(draw_y,             "y");
    PS_INIT_KEY(draw_dx,            "dx");
    PS_INIT_KEY(draw_dy,            "dy");
    PS_INIT_KEY(draw_dw,            "dw");
    PS_INIT_KEY(draw_dh,            "dh");
    PS_INIT_KEY(draw_sx,            "sx");
    PS_INIT_KEY(draw_sy,            "sy");
    PS_INIT_KEY(draw_sw,            "sw");
    PS_INIT_KEY(draw_sh,            "sh");
    PS_INIT_KEY(draw_cx,            "cx");
    PS_INIT_KEY(draw_cy,            "cy");

    PSBitmap::tpl = Persistent<FunctionTemplate>::New(ft);
}

Handle<Object> PSBitmap::createInstance(ALLEGRO_BITMAP *bitmap)
{
    HandleScope _;
    
    Local<Object> instance = PSBitmap::tpl->InstanceTemplate()->NewInstance();
    
    PSBitmap *theBitmap = new PSBitmap(bitmap);
    theBitmap->Wrap(instance);
    
    return _.Close(instance);
}

Handle<Value> PSBitmap::Save(const Arguments& args)
{
    HandleScope _;

    if (args.Length() < 1)
            THROW("PSBitmap.save() requires one argument");

    UNWRAP_SELF;
    const char *filename = *String::AsciiValue(args[0]);
    bool success = al_save_bitmap(filename, self->bitmap_);

    return _.Close(success ? True() : False());
}

Handle<Value> PSBitmap::Use(const Arguments& args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_set_target_bitmap(self->bitmap_);

    return _.Close(Undefined());
}

//
// Drawing

Handle<Value> PSBitmap::ClearToColor(const Arguments &args)
{
    HandleScope _;
    return _.Close(Undefined());
}

Handle<Value> PSBitmap::Draw(const Arguments &args)
{
    HandleScope _;

    bool scaling    = false,
         tinting    = false,
         rotating   = false;

    Handle<Object> opts = args[0]->ToObject();

    float dx = opts->Get(key_draw_dx)->ToNumber()->Value();
    float dy = opts->Get(key_draw_dy)->ToNumber()->Value();

    float cx, cy;
    float dw, dh, sx, sy, sw, sh;
    float xscale, yscale;
    float angle;

    if (opts->Has(key_draw_tint)) {
        tinting = true;
        // TODO: extract color
    }

    if (opts->Has(key_draw_rotate)) {
        rotating = true;
        angle = opts->Get(key_draw_rotate)->ToNumber()->Value();
        cx = opts->Get(key_draw_cx)->ToNumber()->Value();
        cy = opts->Get(key_draw_cy)->ToNumber()->Value();
        if (opts->Has(key_draw_scale)) {
            scaling = true;
            Handle<Value> scale = opts->Get(key_draw_scale);
            if (scale->IsNumber()) {
                xscale = yscale = Handle<Number>::Cast((scale))->Value();
            } else {
                Handle<Object> scaleObj = scale->ToObject();
                xscale = scaleObj->Get(key_draw_x)->ToNumber()->Value();
                yscale = scaleObj->Get(key_draw_y)->ToNumber()->Value();
            }
        }
    } else if (opts->Has(key_draw_sx)) {
        
        scaling = true;
        
        sx = opts->Get(key_draw_sx)->ToNumber()->Value();
        sy = opts->Get(key_draw_sy)->ToNumber()->Value();
        sw = opts->Get(key_draw_sw)->ToNumber()->Value();
        sh = opts->Get(key_draw_sh)->ToNumber()->Value();

        dw = opts->Has(key_draw_dw) ? opts->Get(key_draw_dw)->ToNumber()->Value() : sw;
        dh = opts->Has(key_draw_dh) ? opts->Get(key_draw_dh)->ToNumber()->Value() : sh;

    }

    int flags = opts->Get(key_draw_flags)->ToInteger()->Value();

    UNWRAP_SELF;

    if (!scaling) {
        if (!tinting) {
            if (!rotating) {
                al_draw_bitmap(self->bitmap_, dx, dy, flags);
            } else {
                al_draw_rotated_bitmap(self->bitmap_, cx, cy, dx, dy, angle, flags);
            }
        } else {
            if (!rotating) {
                // TODO: colors
                // al_draw_tinted_bitmap(self->bitmap_, ..., x, y, flags);
            } else {
                // TODO: colors
                // al_draw_tinted_rotated_bitmap(self->bitmap_, ... cx, cy, x, y, angle, flags);
            }
        }
    } else {
        if (!tinting) {
            if (!rotating) {
                al_draw_scaled_bitmap(self->bitmap_, sx, sy, sw, sh, dx, dy, dw, dh, flags);
            } else {
                al_draw_scaled_rotated_bitmap(self->bitmap_, cx, cy, dx, dy, xscale, yscale, angle, flags);
            }
        } else {
            if (!rotating) {
                // TODO: colors
                // al_draw_tinted_scaled_bitmap(self->bitmap_, ..., sx, sy, sw, sh, dx, dy, dw, dh, flags);
            } else {
                // TODO: colors
                // al_draw_tinted_scaled_rotated_bitmap(self->bitmap_, ..., cx, cy, dx, dy, xscale, yscale, angle, flags);
            }
        }
    }

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawFast(const Arguments &args)
{
    HandleScope _;

    float x     = args[0]->ToNumber()->Value(),
          y     = args[1]->ToNumber()->Value();
    
    int flags   = args[2]->ToInteger()->Value();

    UNWRAP_SELF;
    al_draw_bitmap(self->bitmap_, x, y, flags);

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawRegion(const Arguments &args)
{
    HandleScope _;

    float sx    = args[0]->ToNumber()->Value(),
          sy    = args[1]->ToNumber()->Value(),
          sw    = args[2]->ToNumber()->Value(),
          sh    = args[3]->ToNumber()->Value(),
          dx    = args[4]->ToNumber()->Value(),
          dy    = args[5]->ToNumber()->Value();

    int flags   = args[6]->ToInteger()->Value();

    UNWRAP_SELF;
    al_draw_bitmap_region(self->bitmap_, sx, sy, sw, sh, dx, dy, flags);

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawPixel(const Arguments &args)
{
    HandleScope _;
    return _.Close(Undefined());
}

Handle<Value> PSBitmap::PutPixel(const Arguments &args)
{
    HandleScope _;
    return _.Close(Undefined());
}

Handle<Value> PSBitmap::PutBlendedPixel(const Arguments &args)
{
    HandleScope _;
    return _.Close(Undefined());
}
