#include "display.h"
#include "helpers.h"
#include <v8.h>
#include <allegro5/allegro.h>

#define UNWRAP_SELF \
    PSDisplay *self = node::ObjectWrap::Unwrap<PSDisplay>(args.This())

#define UNWRAP_SELF_PROP \
    PSDisplay *self = node::ObjectWrap::Unwrap<PSDisplay>(info.Holder())

using namespace v8;

Handle<Value> createDisplay(const Arguments &args) {
    // TODO: parse args
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    return HandleScope().Close(PSDisplay::createInstance(display));
}

Handle<Value> inhibitScreensaver(const Arguments &args) {
    HandleScope _;
    bool inhibit = (args.Length() == 0) || args[0]->ToBoolean()->IsTrue();
    return _.Close(al_inhibit_screensaver(inhibit) ? True() : False());
}

Handle<Value> waitForVSync(const Arguments& args) {
    return HandleScope().Close(al_wait_for_vsync() ? True() : False());
}

Persistent<FunctionTemplate> PSDisplay::tpl;

PSDisplay::PSDisplay(ALLEGRO_DISPLAY *display)
    : display_(display)
{
}

PSDisplay::~PSDisplay()
{
    destroy();
}

void PSDisplay::init(Handle<Object> target)
{
    HandleScope _;

    //
    // Functions

    NODE_SET_METHOD(target, "createDisplay", createDisplay);
    NODE_SET_METHOD(target, "inhibitScreensaver", inhibitScreensaver);
    NODE_SET_METHOD(target, "waitForVSync", waitForVSync);

    //
    // PSDisplay

    Local<FunctionTemplate> ft = FunctionTemplate::New();
    ft->SetClassName(String::NewSymbol("PSDisplay"));

    Handle<ObjectTemplate> it = ft->InstanceTemplate();
    it->SetInternalFieldCount(1);

    it->SetAccessor(String::New("backBuffer"), GetBackBuffer);
    it->SetAccessor(String::New("width"), GetWidth);
    it->SetAccessor(String::New("height"), GetHeight);
    it->SetAccessor(String::New("refreshRate"), GetRefreshRate);
    it->SetAccessor(String::New("x"), GetX);
    it->SetAccessor(String::New("y"), GetY);
    it->SetAccessor(String::New("pixelFormat"), GetPixelFormat);
    
    NODE_SET_PROTOTYPE_METHOD(ft, "setIcon", SetIcon);
    NODE_SET_PROTOTYPE_METHOD(ft, "setPosition", SetPosition);
    NODE_SET_PROTOTYPE_METHOD(ft, "setTitle", SetTitle);
    NODE_SET_PROTOTYPE_METHOD(ft, "setFullscreen", SetFullscreen);
    NODE_SET_PROTOTYPE_METHOD(ft, "setNoFrame", SetNoFrame);

    NODE_SET_PROTOTYPE_METHOD(ft, "acknowledgeResize", AcknowledgeResize);
    NODE_SET_PROTOTYPE_METHOD(ft, "flip", Flip);
    NODE_SET_PROTOTYPE_METHOD(ft, "resize", Resize);
    NODE_SET_PROTOTYPE_METHOD(ft, "destroy", Destroy);


    PSDisplay::tpl = Persistent<FunctionTemplate>::New(ft);
}

Handle<Value> PSDisplay::createInstance(ALLEGRO_DISPLAY *display)
{
    HandleScope _;
    
    Local<Object> instance = PSDisplay::tpl->InstanceTemplate()->NewInstance();
    
    PSDisplay *theDisplay = new PSDisplay(display);
    theDisplay->Wrap(instance);
    
    return _.Close(instance);
}

Handle<Value> PSDisplay::GetBackBuffer(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    // TODO: we don't have graphics yet
    return _.Close(Undefined());
}

Handle<Value> PSDisplay::GetWidth(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_display_width(self->display_)));
}

Handle<Value> PSDisplay::GetHeight(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_display_height(self->display_)));
}

Handle<Value> PSDisplay::GetRefreshRate(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_display_refresh_rate(self->display_)));
}

Handle<Value> PSDisplay::GetX(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    int x, y;
    al_get_window_position(self->display_, &x, &y);
    return _.Close(Integer::New(x));
}

Handle<Value> PSDisplay::GetY(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    int x, y;
    al_get_window_position(self->display_, &x, &y);
    return _.Close(Integer::New(y));
}

Handle<Value> PSDisplay::GetPixelFormat(Local<String> prop, const AccessorInfo &info)
{
    HandleScope _;
    UNWRAP_SELF_PROP;
    return _.Close(Integer::New(al_get_display_format(self->display_)));
}

Handle<Value> PSDisplay::SetIcon(const Arguments& args)
{
    HandleScope _;
    // TODO: need bitmap support
    return _.Close(Undefined());
}

Handle<Value> PSDisplay::SetPosition(const Arguments& args)
{
    HandleScope _;

    if (args.Length() < 2)
        THROW("PSDisplay.setPosition() requires two arguments");

    int x = args[0]->ToInteger()->Value(),
        y = args[1]->ToInteger()->Value();

    UNWRAP_SELF;
    al_set_window_position(self->display_, x, y);

    return _.Close(Undefined());
}

Handle<Value> PSDisplay::SetTitle(const Arguments& args)
{
    HandleScope _;

    if (args.Length() < 1)
        THROW("PSDisplay.setTitle() requires one argument");

    UNWRAP_SELF;
    al_set_window_title(self->display_, *String::AsciiValue(args[0]));

    return _.Close(Undefined());
}

Handle<Value> PSDisplay::SetFullscreen(const Arguments& args)
{
    HandleScope _;

    bool fullScreen = true;

    if (args.Length() > 0) {
        fullScreen = args[0]->ToBoolean()->Value();
    }

    UNWRAP_SELF;
    bool result = al_toggle_display_flag(self->display_, ALLEGRO_FULLSCREEN_WINDOW, fullScreen);

    return _.Close(result ? True() : False());
}

Handle<Value> PSDisplay::SetNoFrame(const Arguments& args)
{
    HandleScope _;

    bool noFrame = true;

    if (args.Length() > 0) {
        noFrame = args[0]->ToBoolean()->Value();
    }

    UNWRAP_SELF;
    bool result = al_toggle_display_flag(self->display_, ALLEGRO_NOFRAME, noFrame);

    return _.Close(result ? True() : False());
}

Handle<Value> PSDisplay::AcknowledgeResize(const Arguments &args)
{
    HandleScope _;
    UNWRAP_SELF;
    return _.Close(al_acknowledge_resize(self->display_) ? True() : False());
}

Handle<Value> PSDisplay::Flip(const Arguments& args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_set_target_backbuffer(self->display_);
    al_flip_display();

    return _.Close(Undefined());
}

Handle<Value> PSDisplay::Resize(const Arguments& args)
{
    HandleScope _;

    if (args.Length() < 2)
        THROW("PSDisplay.resize() requires two arguments");

    int width   = args[0]->ToInteger()->Value(),
        height  = args[1]->ToInteger()->Value();

    if (width <= 0 || height <= 0)
        THROW("arguments to PSDisplay.resize() must be positive integers");

    UNWRAP_SELF;
    return _.Close(al_resize_display(self->display_, width, height) ? True() : False());
}

Handle<Value> PSDisplay::Destroy(const Arguments& args)
{
    UNWRAP_SELF;
    self->destroy();
    return HandleScope().Close(Undefined());
}

void PSDisplay::destroy()
{
    if (display_ != NULL) {
        al_destroy_display(display_);
        display_ = NULL;
    }
}