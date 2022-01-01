//
// File to RC
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef FILE_TO_RC_VERSION_HPP
#define FILE_TO_RC_VERSION_HPP

#define FILE_TO_RC_VERSION_ABCD                1,11,0,23
#define FILE_TO_RC_VERSION_STR                 "1.11.0"
#define FILE_TO_RC_VERSION_STR_BUILD           "23"
#define FILE_TO_RC_VERSION_STR_DATETIME        "2022-01-01 21:19:32"

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

