cmake_minimum_required(VERSION 3.24)

# ======== Global options =========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
# Highly suggested for making the compiler extra pedantic about
# anything suspicious in your code:
# set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ======== Project Setup =========
project(GNS430)

# "add_library", because we're building a shared library, which is what all
# X-Plane addons are. "add_executable" is for building a standalone app.
add_library(GNS430 SHARED
    display.cpp
	GNS430.cpp
)

# Set up v3 fat plugin with "<plat>_x64/GNS430.xpl" output binary.
set_target_properties(GNS430 PROPERTIES PREFIX "")
set_target_properties(GNS430 PROPERTIES SUFFIX ".xpl")
set_target_properties(GNS430 PROPERTIES OUTPUT_NAME "GNS430")
set_target_properties(GNS430 PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_BIN_OUTDIR}" )
set_target_properties(GNS430 PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_BIN_OUTDIR}" )

# ======= Platform auto-switching =======
if(WIN32)
	set(PLAT_SHORT "win64")
	set(PLAT_ACFUTILS "mingw64")
	set(PLUGIN_BIN_OUTDIR "win_x64")
	add_definitions(-DIBM=1 -DAPL=0 -DLIN=0)
elseif(APPLE)
	set(PLAT_SHORT "mac64")
	set(PLAT_ACFUTILS "mac64")
	set(PLUGIN_BIN_OUTDIR "mac_x64")
	add_definitions(-DIBM=0 -DAPL=1 -DLIN=0)
	# Build universal binaries on macOS
	set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
else()
	set(PLAT_SHORT "lin64")
	set(PLAT_ACFUTILS "lin64")
	set(PLUGIN_BIN_OUTDIR "lin_x64")
	add_definitions(-DIBM=0 -DAPL=0 -DLIN=1)
endif()

# Setup some path variables, which we'll be relying on
file(GLOB XPSDK "${CMAKE_CURRENT_SOURCE_DIR}/SDK")
file(GLOB LIBACFUTILS "${CMAKE_CURRENT_SOURCE_DIR}/libacfutils")
file(GLOB LIBACFUTILS_PLATFORM "${LIBACFUTILS}/${PLAT_ACFUTILS}")

# ======= Compiler setup =======
add_definitions(
    -DXPLM200
    -DXPLM210
    -DXPLM300
    -DXPLM301
    -DXPLM302
    -DXPLM303
    -DXPLM400
    -DGLEW_STATIC
    -D_USE_MATH_DEFINES
)

include_directories(GNS430
    "${XPSDK}/CHeaders/XPLM"
    "${XPSDK}/CHeaders/Widgets"
    "${LIBACFUTILS}/include"
    "${LIBACFUTILS_PLATFORM}/include"
)

# ======= Linker setup =======
if(WIN32)
	find_library(XPLM_LIBRARY XPLM_64 "${XPSDK}/Libraries/Win")
	find_library(XPWIDGETS_LIBRARY XPWidgets_64 "${XPSDK}/Libraries/Win")
	find_library(LIBACFUTILS_LIBRARY acfutils "${LIBACFUTILS_PLATFORM}/lib")
	find_package(OpenGL REQUIRED)
	set(PLATFORM_LIBS
	    ${OPENGL_LIBRARIES}
		${XPLM_LIBRARY}
	    ${XPWIDGETS_LIBRARY}
		${LIBACFUTILS_LIBRARY}
	    "opengl32"
	    "imagehlp"
	    "Psapi"
	    "stdc++"
	    )
elseif(APPLE)
	find_library(XPLM_FRAMEWORK XPLM "${XPSDK}/Libraries/Mac")
	find_library(XPWIDGETS_FRAMEWORK XPWidgets "${XPSDK}/Libraries/Mac")
	find_library(OPENGL_FRAMEWORK OpenGL)
	set(PLATFORM_LIBS
	    ${XPLM_FRAMEWORK}
	    ${XPWIDGETS_FRAMEWORK}
	    ${OPENGL_FRAMEWORK}
	    )
endif()

target_link_libraries(GNS430
    "${LIBACFUTILS_PLATFORM}/lib/libacfutils.a"
	"${LIBACFUTILS_PLATFORM}/lib/libcairo.a"
	"${LIBACFUTILS_PLATFORM}/lib/libGLEWmx.a"
	"${LIBACFUTILS_PLATFORM}/lib/libpixman-1.a"
	"${LIBACFUTILS_PLATFORM}/lib/libfreetype.a"
	"${LIBACFUTILS_PLATFORM}/lib/libz.a"
    ${PLATFORM_LIBS}
)