#ifndef __PRESTO_API_H__
#define __PRESTO_API_H__

#include <v8.h>

using namespace v8;

Handle<Value> ps_install(const Arguments& args);
Handle<Value> ps_uninstall(const Arguments &args);
Handle<Value> ps_install_joystick(const Arguments& args);
Handle<Value> ps_uninstall_joystick(const Arguments &args);
Handle<Value> ps_install_keyboard(const Arguments& args);
Handle<Value> ps_uninstall_keyboard(const Arguments &args);
Handle<Value> ps_install_mouse(const Arguments& args);
Handle<Value> ps_uninstall_mouse(const Arguments &args);
Handle<Value> ps_install_audio(const Arguments& args);
Handle<Value> ps_uninstall_audio(const Arguments &args);
Handle<Value> ps_install_font(const Arguments& args);
Handle<Value> ps_uninstall_font(const Arguments &args);
Handle<Value> ps_install_image(const Arguments& args);
Handle<Value> ps_uninstall_image(const Arguments &args);
Handle<Value> ps_install_primitives(const Arguments& args);
Handle<Value> ps_uninstall_primitives(const Arguments &args);

#endif