//
// File to RC
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef FILE_TO_RC_VERSION_HPP
#define FILE_TO_RC_VERSION_HPP

#define FILE_TO_RC_VERSION_ABCD                1,0,0,2
#define FILE_TO_RC_VERSION_STR                 "1.0.0"
#define FILE_TO_RC_VERSION_STR_BUILD           "2"
#define FILE_TO_RC_VERSION_STR_DATETIME        "2020-02-24 03:06:44"

#ifndef XYO_RC

namespace FileToRc {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif
