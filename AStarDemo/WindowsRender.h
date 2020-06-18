/* WindowsRender.h - Render implementation for Windows apps.
* Copyright (C) 2020 Paulo Pinto
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*/

#ifndef WINDOWSRENDER_H
#define WINDOWSRENDER_H

#pragma once

#include <pch.h>

#include "Render.h"


/**
 * Windows implementation of the application render, based on Win2D CanvasRenderTarget.
 **/
class WindowsRender : public Render {
public:
	WindowsRender(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession dxRender, ::winrt::Windows::Foundation::Size size);
	virtual ~WindowsRender() = default;

	virtual void fill_rect(int x, int y, int w, int h, int r, int g, int b) override;
	virtual void draw_line(int x0, int y0, int x1, int y1) override;
	virtual int width() override;
	virtual int height() override;

private:
	::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession;
	::winrt::Windows::Foundation::Size size;
};

#endif /* WINDOWSRENDER_H */