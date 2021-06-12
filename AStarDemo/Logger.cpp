/* Logger.cpp - Windows specific logging implementation.
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

#include <string>

#include "Logger.h"

// Helper function for writing into the debug console from Visual Studio
static void OutputMessage(const std::string& message)
{
	std::wstring buffer(message.begin(), message.end());
	OutputDebugString(buffer.c_str());
	OutputDebugString(L"\n");
}

void LogInfo(const std::string& message)
{
	OutputMessage(message);
}


void LogWarning(const std::string& message)
{
	OutputMessage(message);
}

void LogErrno()
{
	const size_t errmsglen = 1024; // 1KB
	wchar_t errmsg[errmsglen] = { 0 };
	_wcserror_s(errmsg, errno);

	OutputDebugString(errmsg);
	OutputDebugString(L"\n");
}