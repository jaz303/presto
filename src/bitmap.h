#ifndef __PS_BITMAP_H__
#define __PS_BITMAP_H__

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

using namespace v8;

Handle<Value> createBitmap(const Arguments &args);
Handle<Value> loadBitmap(const Arguments &args);

class PSBitmap : node::ObjectWrap {
public:
    PSBitmap(ALLEGRO_BITMAP *bitmap);
    ~PSBitmap();

    ALLEGRO_BITMAP *allegroBitmap() const;

    static Persistent<FunctionTemplate> tpl;
    static void init(Handle<Object> target);
    static Handle<Object> createInstance(ALLEGRO_BITMAP *bitmap);

private:
    ALLEGRO_BITMAP *bitmap_;

    PSBitmap(const PSBitmap&);
    void operator=(const PSBitmap&);
};

#endif