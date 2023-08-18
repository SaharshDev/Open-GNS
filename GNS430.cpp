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
#include "display.h"

extern gns_display gns430;

#define PLUGIN_NAME "Saharsh's GNS"
#define PLUGIN_SIG "org.saharsh.gns"
#define PLUGIN_DESC "GNS430"

PLUGIN_API int XPluginStart(char* name, char* sig, char* desc) {
    XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);
    strcpy(name, PLUGIN_NAME);
    strcpy(sig, PLUGIN_SIG);
    strcpy(desc, PLUGIN_DESC);

    log_init(XPLMDebugString, "Saharsh's GNS");
    glewInit();
    mt_cairo_render_glob_init(true);

    return 1;
}

PLUGIN_API void XPluginStop(void) {

}

PLUGIN_API int XPluginEnable(void) {
    logMsg("Creating FL...");
    enable();
    logMsg("GNS Registered");
    
    return 1;
}

PLUGIN_API void XPluginDisable(void) {
    logMsg("Destroying FL...");
    disable();
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID from, int msg, void* param) {
    UNUSED(from);
    UNUSED(msg);
    UNUSED(param);
}
