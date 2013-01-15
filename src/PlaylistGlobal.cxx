/*
 * Copyright (C) 2003-2013 The Music Player Daemon Project
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

/*
 * The manager of the global "struct playlist" instance (g_playlist).
 *
 */

#include "config.h"
#include "PlaylistGlobal.hxx"
#include "Playlist.hxx"
#include "Main.hxx"
#include "Partition.hxx"
#include "GlobalEvents.hxx"

static void
playlist_tag_event(void)
{
	global_partition->playlist.TagChanged();
}

static void
playlist_event(void)
{
	global_partition->playlist.SyncWithPlayer(global_partition->pc);
}

void
playlist_global_init()
{
	GlobalEvents::Register(GlobalEvents::TAG, playlist_tag_event);
	GlobalEvents::Register(GlobalEvents::PLAYLIST, playlist_event);
}