#include "event_queue.h"

#include "helpers.h"
#include "display.h"

#define UNWRAP_SELF \
    PSEventQueue *self = node::ObjectWrap::Unwrap<PSEventQueue>(args.This()); \
    (void)self

Persistent<FunctionTemplate> PSEventQueue::tpl;

PS_DECLARE_KEY(ev_type);
PS_DECLARE_KEY(ev_x);
PS_DECLARE_KEY(ev_y);
PS_DECLARE_KEY(ev_z);
PS_DECLARE_KEY(ev_w);
PS_DECLARE_KEY(ev_dx);
PS_DECLARE_KEY(ev_dy);
PS_DECLARE_KEY(ev_dz);
PS_DECLARE_KEY(ev_dw);
PS_DECLARE_KEY(ev_width);
PS_DECLARE_KEY(ev_height);
PS_DECLARE_KEY(ev_button);
PS_DECLARE_KEY(ev_stick);
PS_DECLARE_KEY(ev_axis);
PS_DECLARE_KEY(ev_pos);
PS_DECLARE_KEY(ev_orientation);
PS_DECLARE_KEY(ev_keycode);
PS_DECLARE_KEY(ev_unichar);
PS_DECLARE_KEY(ev_repeat);
PS_DECLARE_KEY(ev_modifiers);
PS_DECLARE_KEY(ev_metaKey);
PS_DECLARE_KEY(ev_shiftKey);
PS_DECLARE_KEY(ev_controlKey);
PS_DECLARE_KEY(ev_altKey);
PS_DECLARE_KEY(ev_display);

Handle<Value> createEventQueue(const Arguments& args)
{
    return PSEventQueue::createInstance();
}

PSEventQueue::PSEventQueue()
{
    queue_ = al_create_event_queue();
}

PSEventQueue::~PSEventQueue()
{
    al_destroy_event_queue(queue_);
}

void PSEventQueue::init(Handle<Object> target)
{
    HandleScope _;
    
    Local<FunctionTemplate> ft = FunctionTemplate::New();
    ft->SetClassName(String::NewSymbol("PSEventQueue"));
    
    Handle<ObjectTemplate> it = ft->InstanceTemplate();
    it->SetInternalFieldCount(1);
    
    NODE_SET_PROTOTYPE_METHOD(ft, "installMouseEventSource", InstallMouseEventSource);
    NODE_SET_PROTOTYPE_METHOD(ft, "installKeyboardEventSource", InstallKeyboardEventSource);
    NODE_SET_PROTOTYPE_METHOD(ft, "installJoystickEventSource", InstallJoystickEventSource);
    NODE_SET_PROTOTYPE_METHOD(ft, "uninstallMouseEventSource", UninstallMouseEventSource);
    NODE_SET_PROTOTYPE_METHOD(ft, "uninstallKeyboardEventSource", UninstallKeyboardEventSource);
    NODE_SET_PROTOTYPE_METHOD(ft, "uninstallJoystickEventSource", UninstallJoystickEventSource);
    NODE_SET_PROTOTYPE_METHOD(ft, "isEmpty", IsEmpty);
    NODE_SET_PROTOTYPE_METHOD(ft, "getNextEvent", GetNextEvent);
    NODE_SET_PROTOTYPE_METHOD(ft, "peekNextEvent", PeekNextEvent);
    NODE_SET_PROTOTYPE_METHOD(ft, "dropNextEvent", DropNextEvent);
    NODE_SET_PROTOTYPE_METHOD(ft, "flush", Flush);
    NODE_SET_PROTOTYPE_METHOD(ft, "waitForEvent", WaitForEvent);
    NODE_SET_PROTOTYPE_METHOD(ft, "waitForEventTimed", WaitForEventTimed);
    NODE_SET_PROTOTYPE_METHOD(ft, "waitForEventUntil", WaitForEventUntil);

    PSEventQueue::tpl = Persistent<FunctionTemplate>::New(ft);
    
    PS_INIT_KEY(ev_x, "type");
    PS_INIT_KEY(ev_x, "x");
    PS_INIT_KEY(ev_y, "y");
    PS_INIT_KEY(ev_z, "z");
    PS_INIT_KEY(ev_w, "w");
    PS_INIT_KEY(ev_dx, "dx");
    PS_INIT_KEY(ev_dy, "dy");
    PS_INIT_KEY(ev_dz, "dz");
    PS_INIT_KEY(ev_dw, "dw");
    PS_INIT_KEY(ev_width, "width");
    PS_INIT_KEY(ev_height, "height");
    PS_INIT_KEY(ev_button, "button");
    PS_INIT_KEY(ev_stick, "stick");
    PS_INIT_KEY(ev_axis, "axis");
    PS_INIT_KEY(ev_pos, "pos");
    PS_INIT_KEY(ev_orientation, "orientation");
    PS_INIT_KEY(ev_keycode, "keycode");
    PS_INIT_KEY(ev_unichar, "unichar");
    PS_INIT_KEY(ev_repeat, "repeat");
    PS_INIT_KEY(ev_modifiers, "modifiers");
    PS_INIT_KEY(ev_metaKey, "metaKey");
    PS_INIT_KEY(ev_shiftKey, "shiftKey");
    PS_INIT_KEY(ev_controlKey, "controlKey");
    PS_INIT_KEY(ev_altKey, "altKey");
    PS_INIT_KEY(ev_display, "display");
}

Handle<Value> PSEventQueue::createInstance()
{
    HandleScope _;
    
    Local<Object> instance = PSEventQueue::tpl->InstanceTemplate()->NewInstance();
    
    PSEventQueue *theQueue = new PSEventQueue();
    theQueue->Wrap(instance);
    
    return _.Close(instance);
}

Handle<Value> PSEventQueue::InstallDisplayEventSource(const Arguments& args)
{
    HandleScope _;
    
    if (args.Length() == 0) {
        THROW("PSEventQueue.installDisplayEventSource() requires display argument");
    }

    if (!args[0]->IsObject()) {
        THROW("PSEventQueue.installDisplayEventSource() argument must be object");   
    }

    PSDisplay *display = node::ObjectWrap::Unwrap<PSDisplay>(Handle<Object>::Cast(args[0]));
    if (display == NULL) {
        THROW("PSEventQueue.installDisplayEventSource() argument must be PSDisplay instance");
    }

    UNWRAP_SELF;
    al_register_event_source(self->queue_, al_get_display_event_source(display->allegroDisplay()));

    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::InstallMouseEventSource(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    al_register_event_source(self->queue_, al_get_mouse_event_source());
    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::InstallKeyboardEventSource(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    al_register_event_source(self->queue_, al_get_keyboard_event_source());
    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::InstallJoystickEventSource(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    al_register_event_source(self->queue_, al_get_joystick_event_source());
    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::UninstallDisplayEventSource(const Arguments& args)
{
    HandleScope _;
    
    if (args.Length() == 0) {
        THROW("PSEventQueue.uninstallDisplayEventSource() requires display argument");
    }

    if (!args[0]->IsObject()) {
        THROW("PSEventQueue.uninstallDisplayEventSource() argument must be object");   
    }

    PSDisplay *display = node::ObjectWrap::Unwrap<PSDisplay>(Handle<Object>::Cast(args[0]));
    if (display == NULL) {
        THROW("PSEventQueue.uninstallDisplayEventSource() argument must be PSDisplay instance");
    }

    UNWRAP_SELF;
    al_unregister_event_source(self->queue_, al_get_display_event_source(display->allegroDisplay()));

    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::UninstallMouseEventSource(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    al_unregister_event_source(self->queue_, al_get_mouse_event_source());
    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::UninstallKeyboardEventSource(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    al_unregister_event_source(self->queue_, al_get_keyboard_event_source());
    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::UninstallJoystickEventSource(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    al_unregister_event_source(self->queue_, al_get_joystick_event_source());
    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::IsEmpty(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;

    return _.Close(al_is_event_queue_empty(self->queue_) ? True() : False());
}

Handle<Value> PSEventQueue::GetNextEvent(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;

    ALLEGRO_EVENT evt;
    if (al_get_next_event(self->queue_, &evt)) {
        return _.Close(wrapEvent(&evt));
    } else {
        return _.Close(Null());
    }
}

Handle<Value> PSEventQueue::PeekNextEvent(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;

    ALLEGRO_EVENT evt;
    if (al_peek_next_event(self->queue_, &evt)) {
        return _.Close(wrapEvent(&evt));
    } else {
        return _.Close(Null());
    }
}

Handle<Value> PSEventQueue::DropNextEvent(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    return _.Close(al_drop_next_event(self->queue_) ? True() : False());
}

Handle<Value> PSEventQueue::Flush(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;
    al_flush_event_queue(self->queue_);
    return _.Close(Undefined());
}

Handle<Value> PSEventQueue::WaitForEvent(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;

    ALLEGRO_EVENT evt;
    al_wait_for_event(self->queue_, &evt);
    return _.Close(wrapEvent(&evt));
}

Handle<Value> PSEventQueue::WaitForEventTimed(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;

    if (args.Length() < 1)
        THROW("waitForEventTimed() requires an argument");
        
    double waitTime = args[0]->ToNumber()->Value();
    
    ALLEGRO_EVENT evt;
    if (al_wait_for_event_timed(self->queue_, &evt, (float)waitTime)) {
        return _.Close(wrapEvent(&evt));
    } else {
        return _.Close(Null());
    }
}

Handle<Value> PSEventQueue::WaitForEventUntil(const Arguments& args)
{
    HandleScope _;
    UNWRAP_SELF;

    if (args.Length() < 1)
        THROW("waitForEventUntil() requires an argument");

    ALLEGRO_TIMEOUT timeout;
    al_init_timeout(&timeout, args[0]->ToNumber()->Value() / 1000.0);

    ALLEGRO_EVENT evt;
    if (al_wait_for_event_until(self->queue_, &evt, &timeout)) {
        return _.Close(wrapEvent(&evt));
    } else {
        return _.Close(Null());
    }
}

#define EV_SET_INT(k, v)    eo->Set(key_ev_##k, Integer::New(v))
#define EV_SET_UINT(k, v)   eo->Set(key_ev_##k, Integer::NewFromUnsigned(v))
#define EV_SET_FLOAT(k, v)  eo->Set(key_ev_##k, Number::New(v))
#define EV_SET_BOOL(k, v)   eo->Set(key_ev_##k, v ? True() : False())
#define EV_SET_DISPLAY(v)   eo->Set(key_ev_display, lookupDisplay(v))

Handle<Value> PSEventQueue::wrapEvent(ALLEGRO_EVENT *evt)
{
    HandleScope _;

    Handle<Object> eo = Object::New();
    int m;

    eo->Set(key_ev_type, Integer::NewFromUnsigned(evt->type));

    switch (evt->type) {
        case ALLEGRO_EVENT_JOYSTICK_AXIS:
            // TODO: joystick ID
            EV_SET_INT(stick, evt->joystick.stick);
            EV_SET_INT(axis, evt->joystick.axis);
            EV_SET_FLOAT(pos, evt->joystick.pos);
            break;
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
            // TODO: joystick ID
            EV_SET_INT(button, evt->joystick.button);
            break;
        case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_KEY_UP:
            EV_SET_INT(keycode, evt->keyboard.keycode);
            EV_SET_DISPLAY(evt->keyboard.display);
            break;
        case ALLEGRO_EVENT_KEY_CHAR:
            EV_SET_INT(keycode, evt->keyboard.keycode);
            EV_SET_INT(unichar, evt->keyboard.unichar);
            // TODO: decode character to string
            EV_SET_INT(modifiers, evt->keyboard.modifiers);

            m = evt->keyboard.modifiers;            
            EV_SET_BOOL(altKey, m & ALLEGRO_KEYMOD_ALT);
            EV_SET_BOOL(shiftKey, m & ALLEGRO_KEYMOD_SHIFT);
            EV_SET_BOOL(controlKey, m & ALLEGRO_KEYMOD_CTRL);
            EV_SET_BOOL(metaKey, m & ALLEGRO_KEYMOD_COMMAND);

            EV_SET_INT(repeat, evt->keyboard.repeat);
            EV_SET_DISPLAY(evt->keyboard.display);
            break;
        case ALLEGRO_EVENT_MOUSE_AXES:
        case ALLEGRO_EVENT_MOUSE_WARPED:
            EV_SET_INT(x, evt->mouse.x);
            EV_SET_INT(y, evt->mouse.y);
            EV_SET_INT(z, evt->mouse.z);
            EV_SET_INT(w, evt->mouse.w);
            EV_SET_INT(dx, evt->mouse.dx);
            EV_SET_INT(dy, evt->mouse.dy);
            EV_SET_INT(dz, evt->mouse.dz);
            EV_SET_INT(dw, evt->mouse.dw);
            EV_SET_DISPLAY(evt->mouse.display);
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            EV_SET_INT(x, evt->mouse.x);
            EV_SET_INT(y, evt->mouse.y);
            EV_SET_INT(z, evt->mouse.z);
            EV_SET_INT(w, evt->mouse.w);
            EV_SET_UINT(button, evt->mouse.button);
            EV_SET_DISPLAY(evt->mouse.display);
            break;
        case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
        case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
            EV_SET_INT(x, evt->mouse.x);
            EV_SET_INT(y, evt->mouse.y);
            EV_SET_INT(z, evt->mouse.z);
            EV_SET_INT(w, evt->mouse.w);
            EV_SET_DISPLAY(evt->mouse.display);
            break;
        case ALLEGRO_EVENT_DISPLAY_EXPOSE:
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            EV_SET_INT(x, evt->display.x);
            EV_SET_INT(y, evt->display.y);
            EV_SET_INT(width, evt->display.width);
            EV_SET_INT(height, evt->display.height);
            EV_SET_DISPLAY(evt->display.source);
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
        case ALLEGRO_EVENT_DISPLAY_LOST:
        case ALLEGRO_EVENT_DISPLAY_FOUND:
        case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
        case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
            EV_SET_DISPLAY(evt->display.source);
            break;
        case ALLEGRO_EVENT_DISPLAY_ORIENTATION:
            EV_SET_INT(orientation, evt->display.orientation);
            EV_SET_DISPLAY(evt->display.source);
            break;
    }

    return _.Close(eo);
}
