/* Logger.h - A generic interface for the logging needs of the application.
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

#ifndef LOGGER_H_
#define LOGGER_H_

#pragma once

#include <string>

/**
 * General purpose logging API.
 * Specific OS implementations are provided in other projects.
 */
extern void LogInfo(const std::string& message);
extern void LogWarning(const std::string& message);
extern void LogErrno();

#endif /* LOGGER_H_ */
