$ErrorActionPreference = "Stop"

# Use absolute paths based on our installation
$QtRoot = "C:\QtMinimal"
$QtVersion = "6.8.0"
$MingwArch = "mingw_64"

# Locate tools dynamically to be safe
$MingwToolPath = (Get-ChildItem -Path "$QtRoot\Tools" -Filter "mingw*" -Directory).FullName | Select-Object -First 1
$CMakePath = (Get-ChildItem -Path "$QtRoot\Tools" -Filter "CMake*" -Directory).FullName | Select-Object -First 1
$NinjaPath = "$QtRoot\Tools\Ninja"

# Add tools to PATH
$env:PATH = "$MingwToolPath\bin;$CMakePath\bin;$NinjaPath;$QtRoot\$QtVersion\$MingwArch\bin;$env:PATH"

# Verify tools
Write-Host "Verifying tools..."
cmake --version
ninja --version
g++ --version

# Create build directory
$BuildDir = "build_minimal"
if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

# Configure with CMake
Write-Host "Configuring project..."
cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$QtRoot\$QtVersion\$MingwArch" ..

# Build
Write-Host "Building project..."
ninja

# Run
Write-Host "Launching game..."
./myGatekeeper.exe
