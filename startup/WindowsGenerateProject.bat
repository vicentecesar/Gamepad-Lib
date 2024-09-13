@echo off
pushd ..\
call third\bin\premake\premake5.exe vs2022
popd
pause