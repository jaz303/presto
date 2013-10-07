#include <v8.h>
#include <allegro5/allegro.h>

using namespace v8;

#define K_INT(k) \
    obj->Set(Handle<String>(String::New(#k)), Integer::New(k))
    
Handle<Value> installConstants(const Arguments& args) {

    HandleScope _;

    Handle<Object> obj = Handle<Object>::Cast(args[0]);

    //
    // Draw flags

    K_INT(ALLEGRO_FLIP_HORIZONTAL);
    K_INT(ALLEGRO_FLIP_VERTICAL);

    //
    // Display flags

    K_INT(ALLEGRO_WINDOWED);
    K_INT(ALLEGRO_FULLSCREEN);
    K_INT(ALLEGRO_FULLSCREEN_WINDOW);
    K_INT(ALLEGRO_RESIZABLE);
    K_INT(ALLEGRO_OPENGL);
    K_INT(ALLEGRO_OPENGL_3_0);
    K_INT(ALLEGRO_OPENGL_FORWARD_COMPATIBLE);
#ifdef ALLEGRO_DIRECT3D
    K_INT(ALLEGRO_DIRECT3D);
#endif
    K_INT(ALLEGRO_NOFRAME);
    K_INT(ALLEGRO_GENERATE_EXPOSE_EVENTS);

    //
    // Keys

    K_INT(ALLEGRO_KEY_A);
    K_INT(ALLEGRO_KEY_B);
    K_INT(ALLEGRO_KEY_C);
    K_INT(ALLEGRO_KEY_D);
    K_INT(ALLEGRO_KEY_E);
    K_INT(ALLEGRO_KEY_F);
    K_INT(ALLEGRO_KEY_G);
    K_INT(ALLEGRO_KEY_H);
    K_INT(ALLEGRO_KEY_I);
    K_INT(ALLEGRO_KEY_J);
    K_INT(ALLEGRO_KEY_K);
    K_INT(ALLEGRO_KEY_L);
    K_INT(ALLEGRO_KEY_M);
    K_INT(ALLEGRO_KEY_N);
    K_INT(ALLEGRO_KEY_O);
    K_INT(ALLEGRO_KEY_P);
    K_INT(ALLEGRO_KEY_Q);
    K_INT(ALLEGRO_KEY_R);
    K_INT(ALLEGRO_KEY_S);
    K_INT(ALLEGRO_KEY_T);
    K_INT(ALLEGRO_KEY_U);
    K_INT(ALLEGRO_KEY_V);
    K_INT(ALLEGRO_KEY_W);
    K_INT(ALLEGRO_KEY_X);
    K_INT(ALLEGRO_KEY_Y);
    K_INT(ALLEGRO_KEY_Z);
    K_INT(ALLEGRO_KEY_0);
    K_INT(ALLEGRO_KEY_1);
    K_INT(ALLEGRO_KEY_2);
    K_INT(ALLEGRO_KEY_3);
    K_INT(ALLEGRO_KEY_4);
    K_INT(ALLEGRO_KEY_5);
    K_INT(ALLEGRO_KEY_6);
    K_INT(ALLEGRO_KEY_7);
    K_INT(ALLEGRO_KEY_8);
    K_INT(ALLEGRO_KEY_9);
    K_INT(ALLEGRO_KEY_PAD_0);
    K_INT(ALLEGRO_KEY_PAD_1);
    K_INT(ALLEGRO_KEY_PAD_2);
    K_INT(ALLEGRO_KEY_PAD_3);
    K_INT(ALLEGRO_KEY_PAD_4);
    K_INT(ALLEGRO_KEY_PAD_5);
    K_INT(ALLEGRO_KEY_PAD_6);
    K_INT(ALLEGRO_KEY_PAD_7);
    K_INT(ALLEGRO_KEY_PAD_8);
    K_INT(ALLEGRO_KEY_PAD_9);
    K_INT(ALLEGRO_KEY_F1);
    K_INT(ALLEGRO_KEY_F2);
    K_INT(ALLEGRO_KEY_F3);
    K_INT(ALLEGRO_KEY_F4);
    K_INT(ALLEGRO_KEY_F5);
    K_INT(ALLEGRO_KEY_F6);
    K_INT(ALLEGRO_KEY_F7);
    K_INT(ALLEGRO_KEY_F8);
    K_INT(ALLEGRO_KEY_F9);
    K_INT(ALLEGRO_KEY_F10);
    K_INT(ALLEGRO_KEY_F11);
    K_INT(ALLEGRO_KEY_F12);
    K_INT(ALLEGRO_KEY_ESCAPE);
    K_INT(ALLEGRO_KEY_TILDE);
    K_INT(ALLEGRO_KEY_MINUS);
    K_INT(ALLEGRO_KEY_EQUALS);
    K_INT(ALLEGRO_KEY_BACKSPACE);
    K_INT(ALLEGRO_KEY_TAB);
    K_INT(ALLEGRO_KEY_OPENBRACE);
    K_INT(ALLEGRO_KEY_CLOSEBRACE);
    K_INT(ALLEGRO_KEY_ENTER);
    K_INT(ALLEGRO_KEY_SEMICOLON);
    K_INT(ALLEGRO_KEY_QUOTE);
    K_INT(ALLEGRO_KEY_BACKSLASH);
    K_INT(ALLEGRO_KEY_BACKSLASH2);
    K_INT(ALLEGRO_KEY_COMMA);
    K_INT(ALLEGRO_KEY_FULLSTOP);
    K_INT(ALLEGRO_KEY_SLASH);
    K_INT(ALLEGRO_KEY_SPACE);
    K_INT(ALLEGRO_KEY_INSERT);
    K_INT(ALLEGRO_KEY_DELETE);
    K_INT(ALLEGRO_KEY_HOME);
    K_INT(ALLEGRO_KEY_END);
    K_INT(ALLEGRO_KEY_PGUP);
    K_INT(ALLEGRO_KEY_PGDN);
    K_INT(ALLEGRO_KEY_LEFT);
    K_INT(ALLEGRO_KEY_RIGHT);
    K_INT(ALLEGRO_KEY_UP);
    K_INT(ALLEGRO_KEY_DOWN);
    K_INT(ALLEGRO_KEY_PAD_SLASH);
    K_INT(ALLEGRO_KEY_PAD_ASTERISK);
    K_INT(ALLEGRO_KEY_PAD_MINUS);
    K_INT(ALLEGRO_KEY_PAD_PLUS);
    K_INT(ALLEGRO_KEY_PAD_DELETE);
    K_INT(ALLEGRO_KEY_PAD_ENTER);
    K_INT(ALLEGRO_KEY_PRINTSCREEN);
    K_INT(ALLEGRO_KEY_PAUSE);
    K_INT(ALLEGRO_KEY_ABNT_C1);
    K_INT(ALLEGRO_KEY_YEN);
    K_INT(ALLEGRO_KEY_KANA);
    K_INT(ALLEGRO_KEY_CONVERT);
    K_INT(ALLEGRO_KEY_NOCONVERT);
    K_INT(ALLEGRO_KEY_AT);
    K_INT(ALLEGRO_KEY_CIRCUMFLEX);
    K_INT(ALLEGRO_KEY_COLON2);
    K_INT(ALLEGRO_KEY_KANJI);
    K_INT(ALLEGRO_KEY_LSHIFT);
    K_INT(ALLEGRO_KEY_RSHIFT);
    K_INT(ALLEGRO_KEY_LCTRL);
    K_INT(ALLEGRO_KEY_RCTRL);
    K_INT(ALLEGRO_KEY_ALT);
    K_INT(ALLEGRO_KEY_ALTGR);
    K_INT(ALLEGRO_KEY_LWIN);
    K_INT(ALLEGRO_KEY_RWIN);
    K_INT(ALLEGRO_KEY_MENU);
    K_INT(ALLEGRO_KEY_SCROLLLOCK);
    K_INT(ALLEGRO_KEY_NUMLOCK);
    K_INT(ALLEGRO_KEY_CAPSLOCK);
    K_INT(ALLEGRO_KEY_PAD_EQUALS);
    K_INT(ALLEGRO_KEY_BACKQUOTE);
    K_INT(ALLEGRO_KEY_SEMICOLON2);
    K_INT(ALLEGRO_KEY_COMMAND);

    //
    // Modifiers

    K_INT(ALLEGRO_KEYMOD_SHIFT);
    K_INT(ALLEGRO_KEYMOD_CTRL);
    K_INT(ALLEGRO_KEYMOD_ALT);
    K_INT(ALLEGRO_KEYMOD_LWIN);
    K_INT(ALLEGRO_KEYMOD_RWIN);
    K_INT(ALLEGRO_KEYMOD_MENU);
    K_INT(ALLEGRO_KEYMOD_ALTGR);
    K_INT(ALLEGRO_KEYMOD_COMMAND);
    K_INT(ALLEGRO_KEYMOD_SCROLLLOCK);
    K_INT(ALLEGRO_KEYMOD_NUMLOCK);
    K_INT(ALLEGRO_KEYMOD_CAPSLOCK);
    K_INT(ALLEGRO_KEYMOD_INALTSEQ);
    K_INT(ALLEGRO_KEYMOD_ACCENT1);
    K_INT(ALLEGRO_KEYMOD_ACCENT2);
    K_INT(ALLEGRO_KEYMOD_ACCENT3);
    K_INT(ALLEGRO_KEYMOD_ACCENT4);

    return _.Close(obj);

}
