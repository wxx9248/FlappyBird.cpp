@echo off
setlocal enabledelayedexpansion

echo ===== FlappyBird.cpp Windows Build Script =====

REM Check if vcpkg exists in environment
if not defined VCPKG_ROOT (
    echo VCPKG_ROOT environment variable is not set.
    echo Please set it to your vcpkg installation directory or install vcpkg.
    echo.
    echo Example:
    echo   git clone https://github.com/microsoft/vcpkg.git
    echo   cd vcpkg
    echo   bootstrap-vcpkg.bat
    echo   setx VCPKG_ROOT "%%CD%%"
    echo.
    goto :error
)

echo Using vcpkg at: %VCPKG_ROOT%

REM Check if vcpkg is properly bootstrapped
if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo vcpkg executable not found. Bootstrapping vcpkg...
    pushd "%VCPKG_ROOT%"
    call bootstrap-vcpkg.bat
    if %ERRORLEVEL% neq 0 (
        echo Failed to bootstrap vcpkg.
        goto :error
    )
    popd
)

REM Install required packages
echo Installing required packages...
"%VCPKG_ROOT%\vcpkg" install sdl2:x64-windows sdl2-image:x64-windows sdl2-mixer:x64-windows sdl2-ttf:x64-windows
if %ERRORLEVEL% neq 0 goto :error

REM Create build directory
if not exist build mkdir build
cd build

REM Generate Visual Studio solution
echo.
echo Generating Visual Studio solution...
cmake .. -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DCMAKE_GENERATOR_PLATFORM=x64
if %ERRORLEVEL% neq 0 goto :error

REM Build the project
echo.
echo Building the project...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 goto :error

REM Copy asset files to output directory
echo.
echo Copying asset files...
if not exist "Release\assets" (
    mkdir "Release\assets"
    xcopy /E /I /Y "..\assets" "Release\assets"
)

echo.
echo Build completed successfully!
echo You can find the executable at: build\Release\FlappyBird.exe
goto :end

:error
echo.
echo An error occurred during the build process.
exit /b 1

:end
echo.
echo Press any key to exit...
pause > nul
exit /b 0 