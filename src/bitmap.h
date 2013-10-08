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

    //
    // Properties

    static Handle<Value> GetFlags(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetFormat(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetWidth(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetHeight(Local<String> prop, const AccessorInfo &info);

    //
    //

    static Handle<Value> Save(const Arguments& args);
    static Handle<Value> Use(const Arguments& args);

    static Handle<Value> IsLocked(const Arguments& args);
    static Handle<Value> IsSubBitmap(const Arguments& args);

    //
    // Drawing

    static Handle<Value> ClearToColor(const Arguments &args);
    static Handle<Value> Draw(const Arguments &args);
    static Handle<Value> DrawFast(const Arguments &args);
    static Handle<Value> DrawTinted(const Arguments &args);
    static Handle<Value> DrawRegion(const Arguments &args);
    static Handle<Value> DrawTintedRegion(const Arguments &args);
    static Handle<Value> DrawPixel(const Arguments &args);
    static Handle<Value> DrawRotated(const Arguments &args);
    static Handle<Value> DrawTintedRotated(const Arguments &args);
    static Handle<Value> DrawScaledRotated(const Arguments &args);
    static Handle<Value> DrawTintedScaledRotated(const Arguments &args);
    static Handle<Value> DrawScaled(const Arguments &args);
    static Handle<Value> DrawTintedScaled(const Arguments &args);
    static Handle<Value> PutPixel(const Arguments &args);
    static Handle<Value> PutBlendedPixel(const Arguments &args);

    //
    // Alpha

    static Handle<Value> ConvertMaskToAlpha(const Arguments &args);

private:
    ALLEGRO_BITMAP *bitmap_;

    PSBitmap(const PSBitmap&);
    void operator=(const PSBitmap&);
};

#endif