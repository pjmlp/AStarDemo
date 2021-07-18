/* Dialogs.ixx - OS specific functionality for displaying windows and dialogs.
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

#define NOMINMAX
#include <windows.h>

#include <SFML/Graphics.hpp>

#include "Resource.h"

#include <string>
#include <optional>

export module Dialogs;

/**
 * @brief Displays an OS specific file open dialog for the maps to be loaded.
 * 
 * @param handle the SFML OS specific handle for the current window.
 */
export std::optional<std::wstring> openFile(sf::WindowHandle handle)
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = handle;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE) {
		return std::wstring(ofn.lpstrFile);
	}

	return std::nullopt;
}

/**
 * @brief Takes care of setting the windows icon, displayed on the top left.
 * 
 * @param handle the SFML OS specific handle for the current window.
 */
export void setApplicationIcon(sf::WindowHandle handle)
{
	auto iconHandle = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ASTARDEMO), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);

	SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)iconHandle);
	SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)iconHandle);
}