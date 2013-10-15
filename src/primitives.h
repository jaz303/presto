#ifndef __PS_PRIMITIVES_H__
#define __PS_PRIMITIVES_H__

#include <v8.h>
#include <node.h>

using namespace v8;

class PSPrimitives {
public:
    static void init(Handle<Object> target);
};

#endif