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

#include "config.h"
#include "ConfigPath.hxx"
#include "fs/Path.hxx"
#include "util/Error.hxx"
#include "ConfigGlobal.hxx"
#include "ConfigOption.hxx"

#include <glib.h>

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#ifndef WIN32
#include <pwd.h>
#endif

#if HAVE_IPV6 && WIN32
#include <winsock2.h>
#endif 

#if HAVE_IPV6 && ! WIN32
#include <sys/socket.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

Path
ParsePath(const char *path, Error &error)
{
	assert(path != nullptr);

	Path path2 = Path::FromUTF8(path);
	if (path2.IsNull()) {
		error.Format(path_domain,
			     "Failed to convert path to file system charset: %s",
			     path);
		return Path::Null();
	}

#ifndef WIN32
	if (!g_path_is_absolute(path) && path[0] != '~') {
		error.Format(path_domain,
			     "not an absolute path: %s", path);
		return Path::Null();
	} else if (path[0] == '~') {
		const char *home;

		if (path[1] == '/' || path[1] == '\0') {
			const char *user = config_get_string(CONF_USER, nullptr);
			if (user != nullptr) {
				struct passwd *passwd = getpwnam(user);
				if (!passwd) {
					error.Format(path_domain,
						     "no such user: %s", user);
					return Path::Null();
				}

				home = passwd->pw_dir;
			} else {
				home = g_get_home_dir();
				if (home == nullptr) {
					error.Set(path_domain,
						  "problems getting home "
						  "for current user");
					return Path::Null();
				}
			}

			++path;
		} else {
			++path;

			const char *slash = strchr(path, '/');
			char *user = slash != nullptr
				? g_strndup(path, slash - path)
				: g_strdup(path);

			struct passwd *passwd = getpwnam(user);
			if (!passwd) {
				error.Format(path_domain,
					     "no such user: %s", user);
				g_free(user);
				return Path::Null();
			}

			g_free(user);

			home = passwd->pw_dir;
			path = slash;
		}

		return Path::Build(home, path2);
	} else {
#endif
		return path2;
#ifndef WIN32
	}
#endif
}
