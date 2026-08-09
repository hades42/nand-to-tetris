@echo off
setlocal

set "PROJECT_DIR=%~dp0"
set "BUILD_DIR=%PROJECT_DIR%build"
set "VCVARSALL=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"

if not exist "%VCVARSALL%" (
    echo ERROR: vcvarsall.bat not found at "%VCVARSALL%"
    exit /b 1
)

if not defined VSCMD_ARG_TGT_ARCH (
    call "%VCVARSALL%" x64 >nul
    if errorlevel 1 (
        echo ERROR: failed to initialize MSVC environment.
        exit /b 1
    )
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cl /nologo /std:c11 /W4 /Zi /Od /RTC1 /MDd /D_DEBUG ^
    /D_CRT_SECURE_NO_WARNINGS /Dstrcasecmp=_stricmp ^
    /Fo"%BUILD_DIR%\\" /Fd"%BUILD_DIR%\VMTranslator.pdb" ^
    "%PROJECT_DIR%*.c" ^
    /link /DEBUG /OUT:"%BUILD_DIR%\VMTranslator.exe"

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Built %BUILD_DIR%\VMTranslator.exe
endlocal
