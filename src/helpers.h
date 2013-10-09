#ifndef __PS_HELPERS_H__
#define __PS_HELPERS_H__

#define THROW(message) \
    return ThrowException(Exception::Error(String::New(message)))

#define PS_DECLARE_KEY(name) \
    static Persistent<String> key_##name

#define PS_INIT_KEY(name, value) \
    key_##name = Persistent<String>::New(String::New(value))

#define F_ARG(ix, name) \
    (args[ix]->NumberValue())

#define I_ARG(ix, name) \
    (args[ix]->IntegerValue())

#define C_ARG(ix, name) \
    (mapColor(args[ix]))

#define UNDEFINED() \
    (HandleScope().Close(Undefined()))

#define BOOL(exp) \
    (HandleScope().Close((exp) ? True() : False()))

#endif