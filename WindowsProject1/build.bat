@echo off
setlocal enabledelayedexpansion
cls
echo [BUILD] Auto-detecting files and compiling with wxWidgets...

:: Set your wxWidgets path
set WX=C:\wxWidgets
set WXVER=32u  :: Change if needed

:: Find all .cpp files
set CPP_FILES=
for /r %%f in (*.cpp) do (
    set CPP_FILES=!CPP_FILES! "%%f"
)

:: Find all header directories
set INCLUDE_PATHS=
for /r %%d in (.) do (
    if exist "%%d\*.h" (
        set INCLUDE_PATHS=!INCLUDE_PATHS! -I"%%d"
    )
)

:: Create build folder if missing
if not exist build mkdir build

:: Compile
g++ !CPP_FILES! !INCLUDE_PATHS! ^
-I"%WX%\include" -I"%WX%\include\msw" ^
-L"%WX%\lib\gcc_lib" ^
-lwxmsw%WXVER%_core -lwxbase%WXVER% -lwxpng -lwxzlib -lwxregex -lwxscintilla ^
-D__WXMSW__ -mwindows -std=c++17 ^
-o build\app.exe

:: Result
if %errorlevel% neq 0 (
    echo [❌ ERROR] Build failed!
) else (
    echo [✅ SUCCESS] Build complete! See: build\app.exe
)

pause
