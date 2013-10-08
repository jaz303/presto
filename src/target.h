#ifndef __PS_TARGET_H__
#define __PS_TARGET_H__

#include <v8.h>
#include <node.h>

using namespace v8;

class PSTarget {
public:
    static bool setTargetPSBitmap(Handle<Value>);
    static void init(Handle<Object> target);
};

#endif