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

#pragma once
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

#include "Exception.h"
#include "AmlWindow.h"


// from include/linux/amlogic/amports/amstream.h
//#define _A_M 'S'
//#define AMSTREAM_IOC_SET_VIDEO_AXIS _IOW((_A_M), 0x4c, int)
//extern "C"
//{	
//#include <codec.h>	// aml_lib
//}



// TODO: Figure out a way to disable screen savers

class WayAmlWindow : public AmlWindow
{
	const int DEFAULT_WIDTH = 1280;
	const int DEFAULT_HEIGHT = 720;
    const char* WINDOW_TITLE = "WaylandAmlWindow";
    int width;
    int height;

	//int video_fd = -1;
	EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;

public:

	virtual EGLDisplay EglDisplay() const override
	{
		return eglDisplay;
	}

	virtual EGLSurface Surface() const override
	{
        return eglSurface;
	}

	virtual EGLContext Context() const override
	{
        return eglContext;
	}



    WayAmlWindow();
    ~WayAmlWindow();


    virtual void WaitForMessage() override//;
    {

    }
    virtual bool ProcessMessages() override//;
    {

    }

	void HideMouse();

	void UnHideMouse();
};

#endif
