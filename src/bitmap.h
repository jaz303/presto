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

    inline ALLEGRO_BITMAP* allegroBitmap() const { return bitmap_; }

    static Persistent<FunctionTemplate> tpl;
    static void init(Handle<Object> target);
    static Handle<Object> createInstance(ALLEGRO_BITMAP *bitmap);

    static Handle<Value> Save(const Arguments& args);
    static Handle<Value> Use(const Arguments& args);

    //
    // Drawing

    static Handle<Value> ClearToColor(const Arguments &args);

    static Handle<Value> Draw(const Arguments &args);
    static Handle<Value> DrawFast(const Arguments &args);
    static Handle<Value> DrawRegion(const Arguments &args);
    static Handle<Value> DrawPixel(const Arguments &args);
    static Handle<Value> PutPixel(const Arguments &args);
    static Handle<Value> PutBlendedPixel(const Arguments &args);

private:
    ALLEGRO_BITMAP *bitmap_;

    PSBitmap(const PSBitmap&);
    void operator=(const PSBitmap&);
};

#endif