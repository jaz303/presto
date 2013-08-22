#include "event_queue.h"

#define UNWRAP_SELF \
    PSEventQueue *self = node::ObjectWrap::Unwrap<PSEventQueue>(args.This()); \
    (void)self

#define THROW(message) \
    return ThrowException(Exception::Error(String::New(message)))

PSEventQueue::PSEventQueue()
    : queue_(al_create_event_queue())
{
}

PSEventQueue::~PSEventQueue()
{
    al_destroy_event_queue(queue_);
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

Handle<Value> PSEventQueue::wrapEvent(ALLEGRO_EVENT *evt)
{
    // TODO: mojo!
    HandleScope _;
    return _.Close(Undefined());
}
