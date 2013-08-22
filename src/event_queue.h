#ifndef __PS_EVENT_QUEUE_H__
#define __PS_EVENT_QUEUE_H__

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>

using namespace v8;

class PSEventQueue : node::ObjectWrap {
public:
    PSEventQueue();
    ~PSEventQueue();

private:
    ALLEGRO_EVENT_QUEUE *queue_;

public:
    static Persistent<FunctionTemplate> tpl;
    static void init(Handle<Object> target);
    static Handle<Value> create_instance();

    static Handle<Value> InstallMouseEventSource(const Arguments& args);
    static Handle<Value> InstallKeyboardEventSource(const Arguments& args);
    static Handle<Value> InstallJoystickEventSource(const Arguments& args);

    static Handle<Value> IsEmpty(const Arguments& args);
    static Handle<Value> GetNextEvent(const Arguments& args);
    static Handle<Value> PeekNextEvent(const Arguments& args);
    static Handle<Value> DropNextEvent(const Arguments& args);
    static Handle<Value> Flush(const Arguments& args);
    static Handle<Value> WaitForEvent(const Arguments& args);
    static Handle<Value> WaitForEventTimed(const Arguments& args);
    static Handle<Value> WaitForEventUntil(const Arguments& args);

private:
    static Handle<Value> wrapEvent(ALLEGRO_EVENT *evt);
};

#endif