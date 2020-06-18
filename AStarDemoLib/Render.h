/* Render.h - A generic interface for the rendering needs of the application.
* Copyright (C) 2014 Paulo Pinto
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

#ifndef RENDER_H_
#define RENDER_H_

#pragma once

/**
 * Framebuffer abstraction class.
 */
class Render {
public:
	virtual ~Render() = default;
	virtual void fill_rect(int x, int y, int w, int h, int r, int g, int b) = 0;
	virtual void draw_line(int x0, int y0, int x1, int y1) = 0;
	virtual int width() = 0;
	virtual int height() = 0;
};


#endif /* RENDER_H_ */
