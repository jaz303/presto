#ifndef __PS_DISPLAY_H__
#define __PS_DISPLAY_H__

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

using namespace v8;

Handle<Value> createDisplay(const Arguments &args);
Handle<Value> inhibitScreensaver(const Arguments &args);
Handle<Value> waitForVSync(const Arguments& args);

// private internal function
Handle<Value> lookupDisplay(ALLEGRO_DISPLAY *display);

class PSDisplay : node::ObjectWrap {
public:
    PSDisplay(ALLEGRO_DISPLAY *display);
    ~PSDisplay();

    ALLEGRO_DISPLAY *allegroDisplay() const;

    static Persistent<FunctionTemplate> tpl;
    static void init(Handle<Object> target);
    static Handle<Object> createInstance(ALLEGRO_DISPLAY *display);

    static Handle<Value> GetBackBuffer(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetWidth(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetHeight(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetRefreshRate(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetX(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetY(Local<String> prop, const AccessorInfo &info);
    static Handle<Value> GetPixelFormat(Local<String> prop, const AccessorInfo &info);
    
    static Handle<Value> SetIcon(const Arguments& args);
    static Handle<Value> SetPosition(const Arguments& args);
    static Handle<Value> SetTitle(const Arguments& args);
    static Handle<Value> SetFullscreen(const Arguments& args);
    static Handle<Value> SetNoFrame(const Arguments& args);

    static Handle<Value> AcknowledgeResize(const Arguments &args);
    static Handle<Value> Use(const Arguments& args);
    static Handle<Value> Resize(const Arguments& args);
    static Handle<Value> Destroy(const Arguments& args);

private:
    ALLEGRO_DISPLAY *display_;
    Persistent<Object> backBuffer_;

    PSDisplay(const PSDisplay&);
    void operator=(const PSDisplay&);

    void destroy();
};

#endif