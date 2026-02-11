$ErrorActionPreference = "Stop"

# Use absolute paths based on our installation
$QtRoot = "C:\QtMinimal"
$QtVersion = "6.8.0"
$MingwArch = "mingw_64"

# Locate tools dynamically
$MingwToolPath = (Get-ChildItem -Path "$QtRoot\Tools" -Filter "mingw*" -Directory).FullName | Select-Object -First 1
$CMakePath = (Get-ChildItem -Path "$QtRoot\Tools" -Filter "CMake*" -Directory).FullName | Select-Object -First 1
$NinjaPath = "$QtRoot\Tools\Ninja"

# Add tools to PATH
$env:PATH = "$MingwToolPath\bin;$CMakePath\bin;$NinjaPath;$QtRoot\$QtVersion\$MingwArch\bin;$env:PATH"

Write-Host "Verifying tools..."
cmake --version
ninja --version
g++ --version

# Create debug build directory
$BuildDir = "build_debug"
if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

# Configure with CMake in Debug mode
Write-Host "Configuring project for DEBUG..."
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="$QtRoot\$QtVersion\$MingwArch" ..

# Build
Write-Host "Building project..."
ninja

# Run
Write-Host "Launching game in DEBUG mode..."
./myGatekeeper.exe
