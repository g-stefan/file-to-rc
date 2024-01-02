// File to RC
// Copyright (c) 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/FileToRC/Copyright.hpp>
#include <XYO/FileToRC/Copyright.rh>

namespace XYO::FileToRC::Copyright {

	static const char *copyright_ = XYO_FILETORC_COPYRIGHT;
	static const char *publisher_ = XYO_FILETORC_PUBLISHER;
	static const char *company_ = XYO_FILETORC_COMPANY;
	static const char *contact_ = XYO_FILETORC_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
