/* WindowsRender.cpp - Render implementation for Windows apps.
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

#include "pch.h"
#include "WindowsRender.h"


WindowsRender::WindowsRender(::winrt::Microsoft::Graphics::Canvas::CanvasDrawingSession dxRender, ::winrt::Windows::Foundation::Size size) : drawingSession(dxRender), size(size)
{
}


void WindowsRender::fill_rect(int x, int y, int w, int h, int r, int g, int b)
{
	::winrt::Windows::Foundation::Rect rect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
	::winrt::Windows::UI::Color color;
	color.R = r;
	color.G = g;
	color.B = b;
	color.A = 255;

	drawingSession.FillRectangle(rect, color);
}

void WindowsRender::draw_line(int x0, int y0, int x1, int y1)
{
	::winrt::Windows::UI::Color color;
	color.R = 0;
	color.G = 0;
	color.B = 0;
	color.A = 255;

	drawingSession.DrawLine(static_cast<float>(x0), static_cast<float>(y0), static_cast<float>(x1), static_cast<float>(y1), color);
}

int WindowsRender::width()
{
	return size.Width;
}

int WindowsRender::height()
{
	return size.Height;
}
