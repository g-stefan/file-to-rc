// File to RC
// Copyright (c) 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_FILETORC_DEPENDENCY_HPP
#define XYO_FILETORC_DEPENDENCY_HPP

#ifndef XYO_SYSTEM_HPP
#	include <XYO/System.hpp>
#endif

// -- Export

#ifdef FILE_TO_RC_INTERNAL
#	ifndef XYO_FILETORC_INTERNAL
#		define XYO_FILETORC_INTERNAL
#	endif
#endif

#ifdef FILE_TO_RC_DLL_INTERNAL
#	ifndef XYO_FILETORC_DLL_INTERNAL
#		define XYO_FILETORC_DLL_INTERNAL
#	endif
#endif

#ifdef XYO_FILETORC_DLL_INTERNAL
#	define XYO_FILETORC_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_FILETORC_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_FILETORC_LIBRARY
#	undef XYO_FILETORC_EXPORT
#	define XYO_FILETORC_EXPORT
#endif

namespace XYO::FileToRC {
	using namespace XYO::System;
};

#endif
