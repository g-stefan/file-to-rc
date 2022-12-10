// File to RC
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_FILETORC_LIBRARY_HPP
#define XYO_FILETORC_LIBRARY_HPP

#ifndef XYO_FILETORC_DEPENDENCY_HPP
#	include <XYO/FileToRC/Dependency.hpp>
#endif

namespace XYO::FileToRC {

	XYO_FILETORC_EXPORT bool fileToRC(const char *stringName, const char *fileNameIn, const char *fileNameOut, bool append);

};

#endif