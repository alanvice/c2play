/*
*
* Copyright (C) 2016 OtherCrashOverride@users.noreply.github.com.
* All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
*/
 #ifdef WAYLAND
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>
#include <wayland-server.h>
#include <wayland-client-protocol.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "WaylandWindow.h"


struct wl_display *display;
struct wl_compositor *compositor;
struct wl_surface *surface;
struct wl_egl_window *egl_window;
struct wl_region *region;
struct wl_shell *shell;
struct wl_shell_surface *shell_surface;

/*
EGLDisplay egl_display;
EGLConfig egl_conf;
EGLSurface egl_surface;
EGLContext egl_context;
*/
static void
global_registry_handler(void *data, struct wl_registry *registry, uint32_t id,
           const char *interface, uint32_t version)
{
    printf("Got a registry event for %s id %d\n", interface, id);
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry,
                      id,
                      &wl_compositor_interface,
                      1);
    } else if (strcmp(interface, "wl_shell") == 0) {
    shell = wl_registry_bind(registry, id,
                 &wl_shell_interface, 1);

    }
}

static void
global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
{
    printf("Got a registry losing event for %d\n", id);
}

static const struct wl_registry_listener registry_listener = {
    global_registry_handler,
    global_registry_remover
};

static void
create_opaque_region() {

}

static void
get_server_references(void) {

    display = wl_display_connect(NULL);
    if (display == NULL) {
    fprintf(stderr, "Can't connect to display\n");
    exit(1);
    }
    printf("connected to display\n");

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    if (compositor == NULL || shell == NULL) {
    fprintf(stderr, "Can't find compositor or shell\n");
    exit(1);
    } else {
    fprintf(stderr, "Found compositor and shell\n");
    }
}

WayAmlWindow::WayAmlWindow()
	: AmlWindow()
{
    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGHT;

    get_server_references();

    surface = wl_compositor_create_surface(compositor);
    if (surface == NULL) {
    fprintf(stderr, "Can't create surface\n");
    exit(1);
    } else {
    fprintf(stderr, "Created surface\n");
    }

    shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_toplevel(shell_surface);




	// Egl
	eglDisplay = Egl::Intialize((NativeDisplayType)display);

	EGLConfig eglConfig = Egl::FindConfig(eglDisplay, 8, 8, 8, 8, 24, 8);	
	if (eglConfig == 0)
		throw Exception("Compatible EGL config not found.");


	// Get the native visual id associated with the config
	int xVisual;	
	eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &xVisual);


    // Opaque Region
    region = wl_compositor_create_region(compositor);
    wl_region_add(region, 0, 0, width, height);
    wl_surface_set_opaque_region(surface, region);

    // Window
    egl_window = wl_egl_window_create(surface, width, height);
    if (egl_window == EGL_NO_SURFACE) {
        fprintf(stderr, "Can't create egl window\n");
    exit(1);
    } else {
        fprintf(stderr, "Created egl window\n");
    }

	EGLint windowAttr[] = {
		EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
		EGL_NONE };

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (NativeWindowType)egl_window, windowAttr);

    if (eglSurface == EGL_NO_SURFACE)
	{
		Egl::CheckError();
	}



	// Create a context
	eglBindAPI(EGL_OPENGL_ES_API);

	EGLint contextAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE };

    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttributes);
    if (eglContext == EGL_NO_CONTEXT)
	{
		Egl::CheckError();
	}

    EGLBoolean success = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (success != EGL_TRUE)
	{
		Egl::CheckError();
	}


	// experimental workaround for wrong sized gl surface
	glViewport(0, 0, width, height);


	success = eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	if (success != EGL_TRUE)
	{
		Egl::CheckError();
	}
}

WayAmlWindow::~WayAmlWindow()
{
    wl_display_disconnect(display);
    printf("disconnected from display\n");
}

/*
void WayAmlWindow::WaitForMessage()
{

}
*/
/*
bool WayAmlWindow::ProcessMessages()
{
	bool run = true;

    while (wl_display_dispatch(display) != -1) {
    // Gestire il caso di uscita
    }

	return run;
}
*/
void WayAmlWindow::HideMouse()
{

}

void WayAmlWindow::UnHideMouse()
{

}

 #endif
