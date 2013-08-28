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

    HandleScope _;

    return _.Close(Null());

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