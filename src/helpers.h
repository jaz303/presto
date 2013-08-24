#ifndef __PS_HELPERS_H__
#define __PS_HELPERS_H__

#define THROW(message) \
    return ThrowException(Exception::Error(String::New(message)))

#endif