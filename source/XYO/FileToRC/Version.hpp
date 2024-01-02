// File to RC
// Copyright (c) 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_FILETORC_VERSION_HPP
#define XYO_FILETORC_VERSION_HPP

#ifndef XYO_FILETORC_DEPENDENCY_HPP
#	include <XYO/FileToRC/Dependency.hpp>
#endif

namespace XYO::FileToRC::Version {

	XYO_FILETORC_EXPORT const char *version();
	XYO_FILETORC_EXPORT const char *build();
	XYO_FILETORC_EXPORT const char *versionWithBuild();
	XYO_FILETORC_EXPORT const char *datetime();

};

#endif
