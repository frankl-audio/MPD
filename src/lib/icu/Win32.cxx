/*
 * Copyright 2003-2016 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "Win32.hxx"
#include "util/AllocatedString.hxx"

#include <windows.h>

AllocatedString<char>
WideCharToMultiByte(unsigned code_page, const wchar_t *src)
{
	int length = WideCharToMultiByte(code_page, 0, src, -1, nullptr, 0,
					 nullptr, nullptr);
	if (length <= 0)
		return nullptr;

	char *buffer = new char[length];
	length = WideCharToMultiByte(code_page, 0, src, -1, buffer, length,
				     nullptr, nullptr);
	if (length <= 0) {
		delete[] buffer;
		return nullptr;
	}

	return AllocatedString<char>::Donate(buffer);
}

AllocatedString<wchar_t>
MultiByteToWideChar(unsigned code_page, const char *src)
{
	int length = MultiByteToWideChar(code_page, 0, src, -1, nullptr, 0);
	if (length <= 0)
		return nullptr;

	wchar_t *buffer = new wchar_t[length];
	length = MultiByteToWideChar(code_page, 0, src, -1, buffer, length);
	if (length <= 0) {
		delete[] buffer;
		return nullptr;
	}

	return AllocatedString<wchar_t>::Donate(buffer);
}
