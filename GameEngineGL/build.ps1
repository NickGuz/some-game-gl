$originalDirectory = Get-Location

if (!(Test-Path "$originalDirectory/build/CMakeCache.txt" -PathType Leaf)) {
    cd build

    $currentDirectory = Get-Location
    if ($currentDirectory -notmatch "build") {
        Write-Host "Error: The current directory is not 'build'."
        return
    }

    cmake ..
    Set-Location $originalDirectory
    cp lib/*.dll build/Debug/
    return
}

cmake --build build/
cp lib/*.dll build/Debug/
