//
// File to RC
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyo.hpp"
#include "file-to-rc.hpp"
#include "file-to-rc-copyright.hpp"
#include "file-to-rc-license.hpp"
#ifndef FILE_TO_RC_NO_VERSION
#include "file-to-rc-version.hpp"
#endif

namespace FileToRC {

	using namespace XYO;

	void Application::showUsage() {
		printf("file-to-rc - Convert file to RC source\n");
		showVersion();
		printf("%s\n\n", FileToRC::Copyright::fullCopyright());

		printf("%s",
			"options:\n"
			"    --usage             this info\n"
			"    --license           show license\n"
			"    --version           show version\n"
			"    --file-in=file      input file\n"
			"    --file-out=file     output file\n"
			"    --touch=file        touch file if changed input file\n"
			"    --append            append content\n"

		);
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", FileToRC::License::content());
	};

	void Application::showVersion() {
#ifndef FILE_TO_RC_NO_VERSION
		printf("version %s build %s [%s]\n", FileToRC::Version::version(), FileToRC::Version::build(), FileToRC::Version::datetime());
#endif
	};


	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		String opt;
		size_t optIndex;
		String optValue;

		String stringName;
		String fileNameIn;
		String fileNameOut;
		bool append = false;
		bool isTouch = false;
		String touchFileName;

		for (i = 1; i < cmdN; ++i) {
			if (StringCore::beginWith(cmdS[i], "--")) {
				opt = &cmdS[i][2];
				optValue = "";
				if(String::indexOf(opt, "=", 0, optIndex)) {
					optValue = String::substring(opt, optIndex + 1);
					opt = String::substring(opt, 0, optIndex);
				};				
				if (opt == "usage") {
					showUsage();
					return 0;
				};
				if (opt == "license") {
					showLicense();
					return 0;
				};
				if (opt == "version") {
					showVersion();
					return 0;
				};
				if (opt == "name") {
					stringName = optValue;
					if(stringName.length() == 0) {
						printf("Error: name is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "file-in") {
					fileNameIn = optValue;
					if(fileNameIn.length() == 0) {
						printf("Error: file-in is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "file-out") {
					fileNameOut = optValue;
					if(fileNameOut.length() == 0) {
						printf("Error: file-out is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "touch") {
					isTouch = true;
					touchFileName = optValue;
					if(touchFileName.length() == 0) {
						printf("Error: touch filename is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "append") {
					append = true;
					continue;
				};
			};
		};

		if(stringName.length() == 0 ||
			fileNameIn.length() == 0 ||
			fileNameOut.length() == 0) {
			showUsage();
			return 1;
		};

		if(isTouch) {
			if(Shell::fileExists(touchFileName)) {
				if(Shell::compareLastWriteTime(touchFileName, fileNameIn) >= 0) {
					return 0;
				};
			};
		};

		if(!Util::fileToRc(
				stringName,
				fileNameIn,
				fileNameOut,
				append)) {
			return 1;
		};

		if(isTouch) {
			Shell::touchIfExists(touchFileName);
		};

		return 0;
	};

};

#ifndef FILE_TO_RC_LIBRARY
XYO_APPLICATION_MAIN_STD(FileToRC::Application);
#endif

