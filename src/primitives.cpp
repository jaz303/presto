#include "primitives.h"
#include "helpers.h"
#include "bitmap.h"
#include "api.h"

#include <v8.h>
#include <node.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <alloca.h>

using namespace v8;

static Handle<Value> drawLine(const Arguments& args) {

    al_draw_line(F_ARG(0, x1),
                 F_ARG(1, y1),
                 F_ARG(2, x2),
                 F_ARG(3, y2),
                 C_ARG(4, color),
                 F_ARG(5, thickness));

    return UNDEFINED();

}

static Handle<Value> drawTriangle(const Arguments& args) {

    al_draw_triangle(F_ARG(0, x1),
                     F_ARG(1, y1),
                     F_ARG(2, x2),
                     F_ARG(3, y2),
                     F_ARG(4, x3),
                     F_ARG(5, y3),
                     C_ARG(6, color),
                     F_ARG(7, thickness));

    return UNDEFINED();

}

static Handle<Value> drawFilledTriangle(const Arguments& args) {

    al_draw_filled_triangle(F_ARG(0, x1),
                            F_ARG(1, y1),
                            F_ARG(2, x2),
                            F_ARG(3, y2),
                            F_ARG(4, x3),
                            F_ARG(5, y3),
                            C_ARG(6, color));

    return UNDEFINED();

}

static Handle<Value> drawRectangle(const Arguments& args) {

    al_draw_rectangle(F_ARG(0, x1),
                      F_ARG(1, y1),
                      F_ARG(2, x2),
                      F_ARG(3, y2),
                      C_ARG(4, color),
                      F_ARG(5, thickness));

    return UNDEFINED();
}

static Handle<Value> drawFilledRectangle(const Arguments& args) {

    al_draw_filled_rectangle(F_ARG(0, x1),
                             F_ARG(1, y1),
                             F_ARG(2, x2),
                             F_ARG(3, y2),
                             C_ARG(4, color));

    return UNDEFINED();

}

static Handle<Value> drawRoundedRectangle(const Arguments& args) {
    
    al_draw_rounded_rectangle(F_ARG(0, x1),
                              F_ARG(1, y1),
                              F_ARG(2, x2),
                              F_ARG(3, y2),
                              F_ARG(4, rx),
                              F_ARG(5, ry),
                              C_ARG(6, color),
                              F_ARG(7, thickness));

    return UNDEFINED();

}

static Handle<Value> drawFilledRoundedRectangle(const Arguments& args) {
    
    al_draw_filled_rounded_rectangle(F_ARG(0, x1),
                                     F_ARG(1, y1),
                                     F_ARG(2, x2),
                                     F_ARG(3, y2),
                                     F_ARG(4, rx),
                                     F_ARG(5, ry),
                                     C_ARG(6, color));

    return UNDEFINED();

}

static Handle<Value> drawEllipse(const Arguments& args) {

    al_draw_ellipse(F_ARG(0, cx),
                    F_ARG(1, cy),
                    F_ARG(2, rx),
                    F_ARG(3, ry),
                    C_ARG(4, color),
                    F_ARG(5, thickness));

    return UNDEFINED();

}

static Handle<Value> drawFilledEllipse(const Arguments& args) {

    al_draw_filled_ellipse(F_ARG(0, cx),
                           F_ARG(1, cy),
                           F_ARG(2, rx),
                           F_ARG(3, ry),
                           C_ARG(4, color));

    return UNDEFINED();

}

static Handle<Value> drawCircle(const Arguments& args) {

    al_draw_circle(F_ARG(0, cx),
                   F_ARG(1, cy),
                   F_ARG(2, r),
                   C_ARG(3, color),
                   F_ARG(4, thickness));

    return UNDEFINED();

}

static Handle<Value> drawFilledCircle(const Arguments& args) {

    al_draw_filled_circle(F_ARG(0, cx),
                          F_ARG(1, cy),
                          F_ARG(2, r),
                          C_ARG(3, color));

    return UNDEFINED();

}

static Handle<Value> drawArc(const Arguments& args) {

    al_draw_arc(F_ARG(0, cx),
                F_ARG(1, cy),
                F_ARG(2, r),
                F_ARG(3, start_theta),
                F_ARG(4, delta_theta),
                C_ARG(5, color),
                F_ARG(6, thickness));

    return UNDEFINED();
}

// TODO: should probably also accept array of objects with x,y keys
static Handle<Value> drawSpline(const Arguments& args) {

    HandleScope _;

    if (!args[0]->IsArray()) {
        THROW("drawSpline() requires array as its first argument");
    }

    Handle<Array> ary = Handle<Array>::Cast(args[0]);

    float cpoints[8] = {
        ary->Get(0)->NumberValue(),
        ary->Get(1)->NumberValue(),
        ary->Get(2)->NumberValue(),
        ary->Get(3)->NumberValue(),
        ary->Get(4)->NumberValue(),
        ary->Get(5)->NumberValue(),
        ary->Get(6)->NumberValue(),
        ary->Get(7)->NumberValue()
    };

    al_draw_spline(cpoints, C_ARG(1, color), F_ARG(2, thickness));

    return _.Close(Undefined());

}

static Handle<Value> drawRibbon(const Arguments& args) {

    HandleScope _;

    if (!args[0]->IsArray()) {
        THROW("drawRibbon() requires array as its first argument");
    }

    Handle<Array> ary = Handle<Array>::Cast(args[0]);
    
    int len = ary->Length();
    if (len == 0 || len & 1) {
        THROW("drawRibbon() point array must be non-empty with an even number of elements");
    }

    float *points = (float*) alloca(sizeof(float) * len);
    if (points) {
        for (int i = 0; i < len; ++i) {
            points[i] = ary->Get(i)->NumberValue();
        }
        al_draw_ribbon(points,
                       sizeof(float) * 2,
                       C_ARG(1, color),
                       F_ARG(2, thickness),
                       len / 2);
    }

    return UNDEFINED();
}

void PSPrimitives::init(Handle<Object> target)
{
    NODE_SET_METHOD(target, "drawLine",                     drawLine);
    NODE_SET_METHOD(target, "drawTriangle",                 drawTriangle);
    NODE_SET_METHOD(target, "drawFilledTriangle",           drawFilledTriangle);
    NODE_SET_METHOD(target, "drawRectangle",                drawRectangle);
    NODE_SET_METHOD(target, "drawFilledRectangle",          drawFilledRectangle);
    NODE_SET_METHOD(target, "drawRoundedRectangle",         drawRoundedRectangle);
    NODE_SET_METHOD(target, "drawFilledRoundedRectangle",   drawFilledRoundedRectangle);
    NODE_SET_METHOD(target, "drawEllipse",                  drawEllipse);
    NODE_SET_METHOD(target, "drawFilledEllipse",            drawFilledEllipse);
    NODE_SET_METHOD(target, "drawCircle",                   drawCircle);
    NODE_SET_METHOD(target, "drawFilledCircle",             drawFilledCircle);
    NODE_SET_METHOD(target, "drawArc",                      drawArc);
    NODE_SET_METHOD(target, "drawSpline",                   drawSpline);
    NODE_SET_METHOD(target, "drawRibbon",                   drawRibbon);
}