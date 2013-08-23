#include "event_queue.h"

#include <iostream>

#define UNWRAP_SELF \
    PSEventQueue *self = node::ObjectWrap::Unwrap<PSEventQueue>(args.This()); \
    (void)self

#define THROW(message) \
    return ThrowException(Exception::Error(String::New(message)))

Persistent<FunctionTemplate> PSEventQueue::tpl;

int ecount = 0;

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
    UNWRAP_SELF;
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
    UNWRAP_SELF;
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

    std::cout << "getting event..." << std::endl;

    ALLEGRO_EVENT evt;
    if (al_get_next_event(self->queue_, &evt)) {

    std::cout << "evt! " << (++ecount) << " type: " << evt.type << std::endl;

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

Handle<Value> PSEventQueue::wrapEvent(ALLEGRO_EVENT *evt)
{
    // TODO: mojo!
    HandleScope _;
    return _.Close(Undefined());
}
