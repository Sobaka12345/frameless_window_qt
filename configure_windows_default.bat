@ECHO off
SETLOCAL EnableDelayedExpansion

SET build_conf=%1

WHERE python
IF %ERRORLEVEL% NEQ 0 ( 
	ECHO "python could not be found"
	EXIT /b %ERRORLEVEL%
)

IF /I NOT "%build_conf%"=="debug" IF /I NOT "%build_conf%"=="release" (
	SET build_conf=debug
	ECHO "Using default build type: !build_conf!"
)

ECHO %build_conf%

MKDIR build
CD build
MKDIR %build_conf%
CD %build_conf%

python -m venv ./ venv
CALL ./venv/Scripts/activate.bat

pip3 install conan

cmake ../.. -DCMAKE_BUILD_TYPE=$build_conf -DPROFILE_PATH="../../conan/conan_profile_windows_"%build_conf%".txt"
