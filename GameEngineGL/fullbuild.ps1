cd build

$currentDirectory = Get-Location
if ($currentDirectory -notmatch "build") {
    Write-Host "Error: The current directory is not 'build'."
    return
}

cmake ..
msbuild .\GameEngineGL.sln
Debug\GameEngineGL.exe
