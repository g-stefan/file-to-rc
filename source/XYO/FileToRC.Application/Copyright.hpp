// File to RC
// Copyright (c) 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_FILETORC_APPLICATION_COPYRIGHT_HPP
#define XYO_FILETORC_APPLICATION_COPYRIGHT_HPP

#ifndef XYO_FILETORC_DEPENDENCY_HPP
#	include <XYO/FileToRC/Dependency.hpp>
#endif

namespace XYO::FileToRC::Application::Copyright {
	std::string copyright();
	std::string publisher();
	std::string company();
	std::string contact();
};

#endif
