#ifndef GNS_DISP_H
#define GNS_DISP_H

#include "display.h"
#include <math.h>
#include <acfutils/glew.h>
#include <acfutils/log.h>
#include <acfutils/mt_cairo_render.h>
#include <acfutils/glutils.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>
#include <XPLMDataAccess.h>
#include <XPLMProcessing.h>

#define DISPLAY_FPS (5)

#define gns_w_xp   (430)
#define gns_h_xp   (240)

#define gns_w      (240)
#define gns_h	   (128)

struct gns_display {
	cairo_surface_t* load_image(const char* img_path);
	cairo_font_face_t* load_font(const char* font_path);
	cairo_surface_t* gnsGlobe;
	cairo_surface_t* gnsFlightTest;
	cairo_surface_t* gnsGarmin;
	cairo_font_face_t* gns430font;
	mt_cairo_render_t* cairo_render;

	XPLMCustomizeAvionics_t gns430_d;
	XPLMAvionicsID gns430_id;

	XPLMDataRef viewportDRef;

	char plugindir[MAX_PATH]{};
	char systemdir[MAX_PATH]{};
	char liverydir[MAX_PATH]{};
};

static FT_Library ft_library;
static FT_Face ft_face;
static FT_Face ft_legacy;
static FT_Error ft_error;

void gns_render(cairo_t* cr, unsigned w, unsigned h, void* data);
int draw_loop(XPLMDeviceID inDeviceID, int inIsBefore, void* inRefcon);
void enable();
void disable();
#endif