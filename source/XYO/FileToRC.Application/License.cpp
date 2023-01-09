// File to RC
// Copyright (c) 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/FileToRC.Application/License.hpp>

namespace XYO::FileToRC::Application::License {

	const char *license() {
		return XYO::FileToRC::License::license();
	};

	const char *shortLicense() {
		return XYO::FileToRC::License::shortLicense();
	};

};
