/* Logger.ixx - A generic interface for the logging needs of the application.
 * Copyright (C) 2021 Paulo Pinto
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
module;

#include <windows.h>

export module Logger;

import <string>;
import <format>;

/**
 * @brief Helper function to actually send the C++ string into the Windows debug console
 * @param message the message to send into the debug console
 */
void OutputMessage(const std::string& message);

export namespace AStarLib {
	/**
	 * @brief logs the provided string with INFO level
	 * @param message the message to send into the debug console
	 */
	void LogInfo(const std::string& message)
	{
		OutputMessage(std::format("[INFO] {}", message));
	}

	/**
     * @brief logs the provided string with WARNING level
     * @param message the message to send into the debug console
     */
	void LogWarning(const std::string& message)
	{
		OutputMessage(std::format("[WARNING] {}", message));
	}

	/**
     * @brief logs the last errno value
     * @param message the message to send into the debug console
     */
	void LogErrno()
	{
		constexpr size_t errmsglen = 1024; // 1KB
		wchar_t errmsg[errmsglen] = { 0 };
		_wcserror_s(errmsg, errno);
		
		// just use the Windows API directly in this case
		OutputDebugString(L"[ERROR] ");
		OutputDebugString(errmsg);
		OutputDebugString(L"\n");
	}
}


module :private;

/**
 * @brief Helper function to convert strings into the Windows string format.
 * @param message the message to send into the debug console
 */
void OutputMessage(const std::string& message)
{
	const std::wstring buffer(message.begin(), message.end());
	OutputDebugString(buffer.c_str());
	OutputDebugString(L"\n");
}

