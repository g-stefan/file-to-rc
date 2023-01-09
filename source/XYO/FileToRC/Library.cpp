// File to RC
// Copyright (c) 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/FileToRC/Library.hpp>

namespace XYO::FileToRC {

	bool fileToRC(
	    const char *stringName,
	    const char *fileNameIn,
	    const char *fileNameOut,
	    bool append) {

		FILE *input;
		FILE *output;
		uint16_t ch;
		int flag;
		int k;

		input = fopen(fileNameIn, "rb");
		if (input != nullptr) {
			output = fopen(fileNameOut, append ? "ab" : "wb");
			if (output != nullptr) {

				fprintf(output, "%s RCDATA {", stringName);

				flag = 0;
				do {
					ch = 0x0A0A;
					k = fread(&ch, 1, 2, input);
					if (k == 2) {
						if (flag == 1) {
							fprintf(output, ",");
						};
						fprintf(output, "0x%04X", ch);
						flag = 1;
					};
				} while (k == 2);

				if (k == 1) {
					if (flag == 1) {
						fprintf(output, ",");
					};
					fprintf(output, "0x%04X}\n", ch);
				} else {
					fprintf(output, "}\n");
				};

				fclose(output);
				fclose(input);
				return true;
			};
			fclose(input);
		};
		return false;
	};

};
