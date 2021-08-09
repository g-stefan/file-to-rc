@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> version-dependency file-to-rc

xyo-cc --bump-version-minor-if-version-dependency file-to-rc
