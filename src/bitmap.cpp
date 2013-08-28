#include "bitmap.h"
#include "helpers.h"
#include <v8.h>
#include <allegro5/allegro.h>

#define UNWRAP_SELF \
    PSBitmap *self = node::ObjectWrap::Unwrap<PSBitmap>(args.This())

#define UNWRAP_SELF_PROP \
    PSBitmap *self = node::ObjectWrap::Unwrap<PSBitmap>(info.Holder())

using namespace v8;

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

PSBitmap::PSBitmap(ALLEGRO_BITMAP *bitmap)
    : bitmap_(bitmap)
{
}

PSBitmap::~PSBitmap()
{
    al_destroy_bitmap(bitmap_);
}

ALLEGRO_BITMAP* PSBitmap::allegroBitmap() const
{
    return bitmap_;
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