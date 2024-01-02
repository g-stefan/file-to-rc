// File to RC
// Copyright (c) 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/FileToRC.Application/License.hpp>

namespace XYO::FileToRC::Application::License {

	std::string license() {
		return XYO::FileToRC::License::license();
	};

	std::string shortLicense() {
		return XYO::FileToRC::License::shortLicense();
	};

};
