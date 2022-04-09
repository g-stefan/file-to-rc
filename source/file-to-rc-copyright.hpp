//
// File to RC
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef FILE_TO_RC_COPYRIGHT_HPP
#define FILE_TO_RC_COPYRIGHT_HPP

#define FILE_TO_RC_COPYRIGHT "Copyright (c) Grigore Stefan"
#define FILE_TO_RC_PUBLISHER "Grigore Stefan"
#define FILE_TO_RC_COMPANY FILE_TO_RC_PUBLISHER
#define FILE_TO_RC_CONTACT "g_stefan@yahoo.com"
#define FILE_TO_RC_FULL_COPYRIGHT FILE_TO_RC_COPYRIGHT " <" FILE_TO_RC_CONTACT ">"

#ifndef XYO_RC

namespace FileToRC {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
