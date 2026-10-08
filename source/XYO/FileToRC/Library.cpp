// File to RC
// Copyright (c) 2007-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2026 Grigore Stefan <g_stefan@yahoo.com>
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
		size_t k;
		int index;
		int first;

		input = fopen(fileNameIn, "rb");
		if (input != nullptr) {
			output = fopen(fileNameOut, append ? "ab" : "wb");
			if (output != nullptr) {

				fprintf(output, "%s RCDATA {", stringName);

				// rc.exe splits long lines and breaks tokens at the split,
				// keep 16 words (32 bytes) per line
				index = 0;
				first = 1;
				do {
					ch = 0x0A0A;
					k = fread(&ch, 1, 2, input);
					if (k == 0) {
						break;
					};

					if (first) {
						first = 0;
						fprintf(output, "\n\t");
					} else {
						fprintf(output, ",");
						if (index == 0) {
							fprintf(output, "\n\t");
						};
					};

					fprintf(output, "0x%04X", ch);

					++index;
					index %= 16;
				} while (k == 2);

				fprintf(output, "\n}\n");

				fclose(output);
				fclose(input);
				return true;
			};
			fclose(input);
		};
		return false;
	};

};
