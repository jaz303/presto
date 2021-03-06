#include "bitmap.h"
#include "helpers.h"
#include "target.h"
#include "api.h"
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

    int width   = I_ARG(0, width),
        height  = I_ARG(1, height),
        format  = I_ARG(2, format),
        flags   = I_ARG(3, flags);

    al_set_new_bitmap_format(format);
    al_set_new_bitmap_flags(flags);

    ALLEGRO_BITMAP *bitmap = al_create_bitmap(width, height);
    if (!bitmap)
        THROW("create bitmap failed!");

    return PSBitmap::createInstance(bitmap);

}

Handle<Value> loadBitmap(const Arguments &args) {

    if (args.Length() < 1)
        THROW("loadBitmap() requires one argument");

    const char *filename = *String::AsciiValue(args[0]);
    
    ALLEGRO_BITMAP *bitmap = al_load_bitmap(filename);
    if (!bitmap)
        THROW("load bitmap failed!");

    return PSBitmap::createInstance(bitmap);

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

    it->SetAccessor(String::New("flags"),                       GetFlags);
    it->SetAccessor(String::New("format"),                      GetFormat);
    it->SetAccessor(String::New("width"),                       GetWidth);
    it->SetAccessor(String::New("height"),                      GetHeight);
    
    NODE_SET_PROTOTYPE_METHOD(ft, "createSubBitmap",            CreateSubBitmap);
    NODE_SET_PROTOTYPE_METHOD(ft, "clone",                      Clone);

    NODE_SET_PROTOTYPE_METHOD(ft, "save",                       Save);
    NODE_SET_PROTOTYPE_METHOD(ft, "use",                        Use);

    NODE_SET_PROTOTYPE_METHOD(ft, "isLocked",                   IsLocked);
    NODE_SET_PROTOTYPE_METHOD(ft, "isSubBitmap",                IsSubBitmap);

    NODE_SET_PROTOTYPE_METHOD(ft, "getPixel",                   GetPixel);
    NODE_SET_PROTOTYPE_METHOD(ft, "getPixelF",                  GetPixelF);

    NODE_SET_PROTOTYPE_METHOD(ft, "draw",                       Draw);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawFast",                   DrawFast);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawTinted",                 DrawTinted);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawRegion",                 DrawRegion);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawTintedRegion",           DrawTintedRegion);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawRotated",                DrawRotated);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawTintedRotated",          DrawTintedRotated);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawScaledRotated",          DrawScaledRotated);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawTintedScaledRotated",    DrawTintedScaledRotated);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawScaled",                 DrawScaled);
    NODE_SET_PROTOTYPE_METHOD(ft, "drawTintedScaled",           DrawTintedScaled);
    
    NODE_SET_PROTOTYPE_METHOD(ft, "convertMaskToAlpha",         ConvertMaskToAlpha);

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

//
// Properties

Handle<Value> PSBitmap::GetFlags(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_bitmap_flags(self->bitmap_)));
}
    
Handle<Value> PSBitmap::GetFormat(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_bitmap_format(self->bitmap_)));
}

Handle<Value> PSBitmap::GetWidth(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_bitmap_width(self->bitmap_)));
}

Handle<Value> PSBitmap::GetHeight(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_bitmap_height(self->bitmap_)));
}

//
// Lifecycle

Handle<Value> PSBitmap::CreateSubBitmap(const Arguments& args)
{
    UNWRAP_SELF;
    ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(self->bitmap_,
                                                     I_ARG(0, x),
                                                     I_ARG(1, y),
                                                     I_ARG(2, w),
                                                     I_ARG(3, h));

    if (!subBitmap) {
        THROW("create sub-bitmap failed!");
    }

    return createInstance(subBitmap);
}

Handle<Value> PSBitmap::Clone(const Arguments& args)
{
    UNWRAP_SELF;

    int format = (args.Length() >= 0 && args[0]->IsNumber())
                    ? args[0]->ToInteger()->Value()
                    : al_get_bitmap_format(self->bitmap_);

    int flags  = (args.Length() >= 1 && args[1]->IsNumber())
                    ? args[1]->ToInteger()->Value()
                    : al_get_bitmap_flags(self->bitmap_);

    al_set_new_bitmap_format(format);
    al_set_new_bitmap_flags(flags);

    ALLEGRO_BITMAP *clone = al_clone_bitmap(self->bitmap_);
    if (!clone) {
        THROW("clone bitmap failed!");
    }

    return createInstance(clone);
}

//
//

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
    PSTarget::setTargetPSBitmap(args.This());
    return UNDEFINED();
}

Handle<Value> PSBitmap::IsLocked(const Arguments& args)
{
    UNWRAP_SELF;
    return HandleScope().Close(al_is_bitmap_locked(self->bitmap_) ? True() : False());
}

Handle<Value> PSBitmap::IsSubBitmap(const Arguments& args)
{
    UNWRAP_SELF;
    return HandleScope().Close(al_is_sub_bitmap(self->bitmap_) ? True() : False());
}

Handle<Value> PSBitmap::GetPixel(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    return unmapColor(al_get_pixel(self->bitmap_, I_ARG(0, x), I_ARG(1, y)), false);
}

Handle<Value> PSBitmap::GetPixelF(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    return unmapColor(al_get_pixel(self->bitmap_, I_ARG(0, x), I_ARG(1, y)), true);
}

//
// Drawing

Handle<Value> PSBitmap::Draw(const Arguments &args)
{
    HandleScope _;

    bool scaling    = false,
         tinting    = false,
         rotating   = false;

    Handle<Object> opts = args[0]->ToObject();

    float dx = opts->Get(key_draw_dx)->NumberValue();
    float dy = opts->Get(key_draw_dy)->NumberValue();

    float cx, cy;
    float dw, dh, sx, sy, sw, sh;
    float xscale, yscale;
    float angle;
    ALLEGRO_COLOR color;

    if (opts->Has(key_draw_tint)) {
        tinting = true;
        color = mapColor(opts->Get(key_draw_tint));
    }

    if (opts->Has(key_draw_rotate)) {
        rotating = true;
        angle = opts->Get(key_draw_rotate)->NumberValue();
        cx = opts->Get(key_draw_cx)->NumberValue();
        cy = opts->Get(key_draw_cy)->NumberValue();
        if (opts->Has(key_draw_scale)) {
            scaling = true;
            Handle<Value> scale = opts->Get(key_draw_scale);
            if (scale->IsNumber()) {
                xscale = yscale = scale->NumberValue();
            } else {
                Handle<Object> scaleObj = Handle<Object>::Cast(scale);
                xscale = scaleObj->Get(key_draw_x)->NumberValue();
                yscale = scaleObj->Get(key_draw_y)->NumberValue();
            }
        }
    } else if (opts->Has(key_draw_sx)) {
        
        scaling = true;
        
        sx = opts->Get(key_draw_sx)->NumberValue();
        sy = opts->Get(key_draw_sy)->NumberValue();
        sw = opts->Get(key_draw_sw)->NumberValue();
        sh = opts->Get(key_draw_sh)->NumberValue();

        dw = opts->Has(key_draw_dw) ? opts->Get(key_draw_dw)->NumberValue() : sw;
        dh = opts->Has(key_draw_dh) ? opts->Get(key_draw_dh)->NumberValue() : sh;

    }

    int flags = opts->Get(key_draw_flags)->IntegerValue();

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
                al_draw_tinted_bitmap(self->bitmap_, color, dx, dy, flags);
            } else {
                al_draw_tinted_rotated_bitmap(self->bitmap_, color, cx, cy, dx, dy, angle, flags);
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
                al_draw_tinted_scaled_bitmap(self->bitmap_, color, sx, sy, sw, sh, dx, dy, dw, dh, flags);
            } else {
                al_draw_tinted_scaled_rotated_bitmap(self->bitmap_, color, cx, cy, dx, dy, xscale, yscale, angle, flags);
            }
        }
    }

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawFast(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_bitmap(self->bitmap_, F_ARG(0, x), F_ARG(1, y), I_ARG(2, flags));

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawTinted(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_tinted_bitmap(  self->bitmap_,
                            C_ARG(0, tint),
                            F_ARG(1, dx),
                            F_ARG(2, dy),
                            I_ARG(3, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawRegion(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_bitmap_region(  self->bitmap_,
                            F_ARG(0, sx),
                            F_ARG(1, sy),
                            F_ARG(2, sw),
                            F_ARG(3, sh),
                            F_ARG(4, dx),
                            F_ARG(5, dy),
                            I_ARG(6, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawTintedRegion(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_tinted_bitmap_region(   self->bitmap_,
                                    C_ARG(0, tint),
                                    F_ARG(1, sx),
                                    F_ARG(2, sy),
                                    F_ARG(3, sw),
                                    F_ARG(4, sh),
                                    F_ARG(5, dx),
                                    F_ARG(6, dy),
                                    I_ARG(7, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawRotated(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_rotated_bitmap( self->bitmap_,
                            F_ARG(0, cx),
                            F_ARG(1, cy),
                            F_ARG(2, dx),
                            F_ARG(3, dy),
                            F_ARG(4, angle),
                            I_ARG(5, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawTintedRotated(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_tinted_rotated_bitmap(  self->bitmap_,
                                    C_ARG(0, tint),
                                    F_ARG(1, cx),
                                    F_ARG(2, cy),
                                    F_ARG(3, dx),
                                    F_ARG(4, dy),
                                    F_ARG(5, angle),
                                    I_ARG(6, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawScaledRotated(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_scaled_rotated_bitmap(  self->bitmap_,
                                    F_ARG(0, cx),
                                    F_ARG(1, cy),
                                    F_ARG(2, dx),
                                    F_ARG(3, dy),
                                    F_ARG(4, xscale),
                                    F_ARG(5, yscale),
                                    F_ARG(6, angle),
                                    I_ARG(7, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawTintedScaledRotated(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_tinted_scaled_rotated_bitmap(   self->bitmap_,
                                            C_ARG(0, tint),
                                            F_ARG(1, cx),
                                            F_ARG(2, cy),
                                            F_ARG(3, dx),
                                            F_ARG(4, dy),
                                            F_ARG(5, xscale),
                                            F_ARG(6, yscale),
                                            F_ARG(7, angle),
                                            I_ARG(8, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawScaled(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_scaled_bitmap(  self->bitmap_,
                            F_ARG(0, sx),
                            F_ARG(1, sy),
                            F_ARG(2, sw),
                            F_ARG(3, sh),
                            F_ARG(4, dx),
                            F_ARG(5, dy),
                            F_ARG(6, dw),
                            F_ARG(7, dh),
                            I_ARG(8, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::DrawTintedScaled(const Arguments &args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_draw_tinted_scaled_bitmap(   self->bitmap_,
                                    C_ARG(0, tint),
                                    F_ARG(1, sx),
                                    F_ARG(2, sy),
                                    F_ARG(3, sw),
                                    F_ARG(4, sh),
                                    F_ARG(5, dx),
                                    F_ARG(6, dy),
                                    F_ARG(7, dw),
                                    F_ARG(8, dh),
                                    I_ARG(9, flags) );

    return _.Close(Undefined());
}

Handle<Value> PSBitmap::ConvertMaskToAlpha(const Arguments &args)
{
    UNWRAP_SELF;
    ALLEGRO_COLOR color = mapColor(args[0]);

    al_convert_mask_to_alpha(self->bitmap_, color);

    return UNDEFINED();
}