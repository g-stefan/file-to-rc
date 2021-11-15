//
// File to RC
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef FILE_TO_RC_VERSION_HPP
#define FILE_TO_RC_VERSION_HPP

#define FILE_TO_RC_VERSION_ABCD                1,9,0,22
#define FILE_TO_RC_VERSION_STR                 "1.9.0"
#define FILE_TO_RC_VERSION_STR_BUILD           "22"
#define FILE_TO_RC_VERSION_STR_DATETIME        "2021-11-15 16:51:47"

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

