// File to RC
// Copyright (c) 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_FILETORC_COPYRIGHT_HPP
#define XYO_FILETORC_COPYRIGHT_HPP

#ifndef XYO_FILETORC_DEPENDENCY_HPP
#	include <XYO/FileToRC/Dependency.hpp>
#endif

namespace XYO::FileToRC::Copyright {
	XYO_FILETORC_EXPORT const char *copyright();
	XYO_FILETORC_EXPORT const char *publisher();
	XYO_FILETORC_EXPORT const char *company();
	XYO_FILETORC_EXPORT const char *contact();
};

#endif
