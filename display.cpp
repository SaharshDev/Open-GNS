#include "display.h"
#include <math.h>
#include <acfutils/glew.h>
#include <acfutils/log.h>
#include <acfutils/mt_cairo_render.h>
#include <acfutils/glutils.h>
#include <acfutils/helpers.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>
#include <XPLMDataAccess.h>
#include <XPLMProcessing.h>

gns_display gns430;

int draw_loop(XPLMDeviceID inDeviceID, int inIsBefore, void* inRefcon) {
    int vp[4];
    mat4 pvm;

    XPLMGetDatavi(gns430.viewportDRef, vp, 0, 4);
    glm_ortho(vp[0], vp[2], vp[1], vp[3], 0, 1, pvm);
    mt_cairo_render_draw_pvm(gns430.cairo_render, { 0, 0 }, { gns_w_xp, gns_h_xp }, (GLfloat*)pvm);

    return 0;
}

void gns_render(cairo_t* cr, unsigned w, unsigned h, void* data) {
    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, 1, 1, 55, 43);
    cairo_set_source_rgb(cr, 0, 1, 1);
    cairo_stroke(cr);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_rectangle(cr, 1, 1, 55, 43);
    cairo_fill(cr);

    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, 1, 46, 55, 43);
    cairo_set_source_rgb(cr, 0, 1, 1);
    cairo_stroke(cr);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_rectangle(cr, 1, 46, 55, 43);
    cairo_fill(cr);

    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, 1, 93, 55, 32);
    cairo_set_source_rgb(cr, 0, 1, 1);
    cairo_stroke(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 1, 93, 55, 32);
    cairo_fill(cr);
}

void enable() {
    gns430.viewportDRef = XPLMFindDataRef("sim/graphics/view/viewport");
    gns430.cairo_render = mt_cairo_render_init(gns_w, gns_h, DISPLAY_FPS, NULL, gns_render, NULL, NULL);
    mt_cairo_render_set_texture_filter(gns430.cairo_render, GL_NEAREST);

    gns430.gns430_d.deviceId = xplm_device_GNS430_2;
    gns430.gns430_d.drawCallbackBefore = draw_loop;
    gns430.gns430_d.drawCallbackAfter = NULL;
    gns430.gns430_d.refcon = NULL;
    gns430.gns430_d.structSize = sizeof(XPLMCustomizeAvionics_t);
    gns430.gns430_id = XPLMRegisterAvionicsCallbacksEx(&gns430.gns430_d);
}

void disable() {
    XPLMUnregisterDrawCallback(draw_loop, xplm_Phase_Gauges, 1, NULL);
    mt_cairo_render_fini(gns430.cairo_render);
    gns430.cairo_render = NULL;
    XPLMUnregisterAvionicsCallbacks(gns430.gns430_id);
}