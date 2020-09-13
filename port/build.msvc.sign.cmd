@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> sign file-to-rc

pushd bin
for /r %%i in (*.exe) do call grigore-stefan.sign "File to RC Source" "%%i"
popd
