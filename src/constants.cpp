#include <v8.h>
#include <allegro5/allegro.h>

using namespace v8;

#define K_INT(k) \
    obj->Set(Handle<String>(String::New(#k)), Integer::New(ALLEGRO_##k))
    
Handle<Value> installConstants(const Arguments& args) {

    HandleScope _;

    Handle<Object> obj = Handle<Object>::Cast(args[0]);

    //
    // Draw flags

    K_INT(FLIP_HORIZONTAL);
    K_INT(FLIP_VERTICAL);

    //
    // Display flags

    K_INT(WINDOWED);
    K_INT(FULLSCREEN);
    K_INT(FULLSCREEN_WINDOW);
    K_INT(RESIZABLE);
    K_INT(OPENGL);
    K_INT(OPENGL_3_0);
    K_INT(OPENGL_FORWARD_COMPATIBLE);
#ifdef DIRECT3D
    K_INT(DIRECT3D);
#endif
    K_INT(NOFRAME);
    K_INT(GENERATE_EXPOSE_EVENTS);

    //
    // Keys

    K_INT(KEY_A);
    K_INT(KEY_B);
    K_INT(KEY_C);
    K_INT(KEY_D);
    K_INT(KEY_E);
    K_INT(KEY_F);
    K_INT(KEY_G);
    K_INT(KEY_H);
    K_INT(KEY_I);
    K_INT(KEY_J);
    K_INT(KEY_K);
    K_INT(KEY_L);
    K_INT(KEY_M);
    K_INT(KEY_N);
    K_INT(KEY_O);
    K_INT(KEY_P);
    K_INT(KEY_Q);
    K_INT(KEY_R);
    K_INT(KEY_S);
    K_INT(KEY_T);
    K_INT(KEY_U);
    K_INT(KEY_V);
    K_INT(KEY_W);
    K_INT(KEY_X);
    K_INT(KEY_Y);
    K_INT(KEY_Z);
    K_INT(KEY_0);
    K_INT(KEY_1);
    K_INT(KEY_2);
    K_INT(KEY_3);
    K_INT(KEY_4);
    K_INT(KEY_5);
    K_INT(KEY_6);
    K_INT(KEY_7);
    K_INT(KEY_8);
    K_INT(KEY_9);
    K_INT(KEY_PAD_0);
    K_INT(KEY_PAD_1);
    K_INT(KEY_PAD_2);
    K_INT(KEY_PAD_3);
    K_INT(KEY_PAD_4);
    K_INT(KEY_PAD_5);
    K_INT(KEY_PAD_6);
    K_INT(KEY_PAD_7);
    K_INT(KEY_PAD_8);
    K_INT(KEY_PAD_9);
    K_INT(KEY_F1);
    K_INT(KEY_F2);
    K_INT(KEY_F3);
    K_INT(KEY_F4);
    K_INT(KEY_F5);
    K_INT(KEY_F6);
    K_INT(KEY_F7);
    K_INT(KEY_F8);
    K_INT(KEY_F9);
    K_INT(KEY_F10);
    K_INT(KEY_F11);
    K_INT(KEY_F12);
    K_INT(KEY_ESCAPE);
    K_INT(KEY_TILDE);
    K_INT(KEY_MINUS);
    K_INT(KEY_EQUALS);
    K_INT(KEY_BACKSPACE);
    K_INT(KEY_TAB);
    K_INT(KEY_OPENBRACE);
    K_INT(KEY_CLOSEBRACE);
    K_INT(KEY_ENTER);
    K_INT(KEY_SEMICOLON);
    K_INT(KEY_QUOTE);
    K_INT(KEY_BACKSLASH);
    K_INT(KEY_BACKSLASH2);
    K_INT(KEY_COMMA);
    K_INT(KEY_FULLSTOP);
    K_INT(KEY_SLASH);
    K_INT(KEY_SPACE);
    K_INT(KEY_INSERT);
    K_INT(KEY_DELETE);
    K_INT(KEY_HOME);
    K_INT(KEY_END);
    K_INT(KEY_PGUP);
    K_INT(KEY_PGDN);
    K_INT(KEY_LEFT);
    K_INT(KEY_RIGHT);
    K_INT(KEY_UP);
    K_INT(KEY_DOWN);
    K_INT(KEY_PAD_SLASH);
    K_INT(KEY_PAD_ASTERISK);
    K_INT(KEY_PAD_MINUS);
    K_INT(KEY_PAD_PLUS);
    K_INT(KEY_PAD_DELETE);
    K_INT(KEY_PAD_ENTER);
    K_INT(KEY_PRINTSCREEN);
    K_INT(KEY_PAUSE);
    K_INT(KEY_ABNT_C1);
    K_INT(KEY_YEN);
    K_INT(KEY_KANA);
    K_INT(KEY_CONVERT);
    K_INT(KEY_NOCONVERT);
    K_INT(KEY_AT);
    K_INT(KEY_CIRCUMFLEX);
    K_INT(KEY_COLON2);
    K_INT(KEY_KANJI);
    K_INT(KEY_LSHIFT);
    K_INT(KEY_RSHIFT);
    K_INT(KEY_LCTRL);
    K_INT(KEY_RCTRL);
    K_INT(KEY_ALT);
    K_INT(KEY_ALTGR);
    K_INT(KEY_LWIN);
    K_INT(KEY_RWIN);
    K_INT(KEY_MENU);
    K_INT(KEY_SCROLLLOCK);
    K_INT(KEY_NUMLOCK);
    K_INT(KEY_CAPSLOCK);
    K_INT(KEY_PAD_EQUALS);
    K_INT(KEY_BACKQUOTE);
    K_INT(KEY_SEMICOLON2);
    K_INT(KEY_COMMAND);

    //
    // Modifiers

    K_INT(KEYMOD_SHIFT);
    K_INT(KEYMOD_CTRL);
    K_INT(KEYMOD_ALT);
    K_INT(KEYMOD_LWIN);
    K_INT(KEYMOD_RWIN);
    K_INT(KEYMOD_MENU);
    K_INT(KEYMOD_ALTGR);
    K_INT(KEYMOD_COMMAND);
    K_INT(KEYMOD_SCROLLLOCK);
    K_INT(KEYMOD_NUMLOCK);
    K_INT(KEYMOD_CAPSLOCK);
    K_INT(KEYMOD_INALTSEQ);
    K_INT(KEYMOD_ACCENT1);
    K_INT(KEYMOD_ACCENT2);
    K_INT(KEYMOD_ACCENT3);
    K_INT(KEYMOD_ACCENT4);

    // Blending

    K_INT(ADD);
    K_INT(DEST_MINUS_SRC);
    K_INT(SRC_MINUS_DEST);
    K_INT(ZERO);
    K_INT(ONE);
    K_INT(ALPHA);
    K_INT(INVERSE_ALPHA);

    return _.Close(obj);

}
