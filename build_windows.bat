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

REM Check if vcpkg.exe exists
if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo vcpkg.exe not found in %VCPKG_ROOT%.
    echo Please ensure vcpkg is properly installed.
    goto :error
)

REM Check if using manifest mode
if exist "vcpkg.json" (
    echo Detected vcpkg.json - Using manifest mode
    
    REM Install dependencies through manifest
    echo Installing dependencies through manifest...
    echo This might take a while for the first build...
    %VCPKG_ROOT%\vcpkg install --triplet=x64-windows
    if %ERRORLEVEL% neq 0 goto :vcpkg_error
) else (
    REM Install required packages using explicit triplet
    echo Installing required packages directly...
    echo This might take a while for the first build...
    %VCPKG_ROOT%\vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-mixer:x64-windows sdl2-ttf:x64-windows
    if %ERRORLEVEL% neq 0 goto :vcpkg_error
)

REM Create build directory
if not exist build mkdir build
cd build

REM Generate Visual Studio solution
echo Generating Visual Studio solution...
cmake .. -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 goto :cmake_error

REM Build the project
echo Building the project...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 goto :build_error

REM Copy asset files to output directory
echo.
echo Ensuring assets are copied...
if not exist "Release\assets" (
    mkdir "Release\assets" 2>nul
    xcopy /E /I /Y "..\assets" "Release\assets"
)

echo.
echo Build completed successfully!
echo You can find the executable at: build\Release\FlappyBird.exe
goto :end

:vcpkg_error
echo.
echo Error installing packages with vcpkg.
echo Please ensure you have the necessary permissions and internet connection.
exit /b 1

:cmake_error
echo.
echo Error generating CMake project.
echo Please check the error messages above.
exit /b 1

:build_error
echo.
echo Error building the project.
echo Please check the build logs for details.
exit /b 1

:error
echo.
echo An error occurred during the build process.
exit /b 1

:end
echo.
echo Press any key to exit...
pause > nul
exit /b 0 