/*
 * Copyright (C) 2003-2014 The Music Player Daemon Project
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
#include "DatabaseSong.hxx"
#include "DatabaseGlue.hxx"
#include "DatabasePlugin.hxx"
#include "DetachedSong.hxx"
#include "Mapper.hxx"

DetachedSong *
DatabaseDetachSong(const char *uri, Error &error)
{
	const Database *db = GetDatabase(error);
	if (db == nullptr)
		return nullptr;

	const LightSong *tmp = db->GetSong(uri, error);
	if (tmp == nullptr)
		return nullptr;

	DetachedSong *song = new DetachedSong(map_song_detach(*tmp));
	db->ReturnSong(tmp);
	return song;
}