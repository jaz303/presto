#include "display.h"
#include "helpers.h"
#include <v8.h>
#include <allegro5/allegro.h>

#define UNWRAP_SELF \
    PSDisplay *self = node::ObjectWrap::Unwrap<PSDisplay>(args.This())

#define UNWRAP_SELF_PROP \
    PSDisplay *self = node::ObjectWrap::Unwrap<PSDisplay>(info.Holder())

using namespace v8;

static const int MAX_DISPLAYS = 16;
struct {
    bool inUse;
    ALLEGRO_DISPLAY *alDisplay;
    Persistent<Object> psDisplay;
} displays[MAX_DISPLAYS];

Handle<Value> createDisplay(const Arguments &args) {

    int width = args[0]->ToInteger()->Value();
    int height = args[1]->ToInteger()->Value();
    int flags = args[2]->ToInteger()->Value();

    int x = INT_MAX;
    int y = INT_MAX;

    if (args.Length() >= 5) {
        x = args[3]->ToInteger()->Value();
        y = args[4]->ToInteger()->Value();
    }

    HandleScope _;

    for (int i = 0; i < MAX_DISPLAYS; ++i) {
        if (!displays[i].inUse) {
            al_set_new_window_position(x, y);
            al_set_new_display_flags(flags);
            ALLEGRO_DISPLAY *display = al_create_display(width, height);
            if (!display) {
                break;
            }
            Handle<Object> wrapped = PSDisplay::createInstance(display);
            displays[i].inUse = true;
            displays[i].alDisplay = display;
            displays[i].psDisplay = Persistent<Object>::New(wrapped);
            return _.Close(wrapped);
        }
    }

    return _.Close(Null());

}

Handle<Value> lookupDisplay(ALLEGRO_DISPLAY *display) {
    for (int i = 0; i < MAX_DISPLAYS; ++i) {
        if (displays[i].alDisplay == display) {
            return displays[i].psDisplay;
        }
    }
    return HandleScope().Close(Null());
}

void unmapDisplay(ALLEGRO_DISPLAY *display) {
    for (int i = 0; i < MAX_DISPLAYS; ++i) {
        if (displays[i].alDisplay == display) {
            displays[i].inUse = false;
            displays[i].alDisplay = NULL;
            displays[i].psDisplay.Dispose();
            return;
        }
    }
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

ALLEGRO_DISPLAY* PSDisplay::allegroDisplay() const
{
    return display_;
}

void PSDisplay::init(Handle<Object> target)
{
    HandleScope _;

    //
    // Display pool

    for (int i = 0; i < MAX_DISPLAYS; ++i) {
        displays[i].inUse = false;
    }

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
    NODE_SET_PROTOTYPE_METHOD(ft, "use", Use);
    NODE_SET_PROTOTYPE_METHOD(ft, "resize", Resize);
    NODE_SET_PROTOTYPE_METHOD(ft, "destroy", Destroy);


    PSDisplay::tpl = Persistent<FunctionTemplate>::New(ft);
}

Handle<Object> PSDisplay::createInstance(ALLEGRO_DISPLAY *display)
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

Handle<Value> PSDisplay::Use(const Arguments& args)
{
    HandleScope _;

    UNWRAP_SELF;
    al_set_target_backbuffer(self->display_);
    
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
    return UNDEFINED();
}

void PSDisplay::destroy()
{
    if (display_ != NULL) {
        unmapDisplay(display_);
        al_destroy_display(display_);
        display_ = NULL;
    }
}