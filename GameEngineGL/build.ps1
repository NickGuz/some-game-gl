$originalDirectory = Get-Location

cd build

$currentDirectory = Get-Location
if ($currentDirectory -notmatch "build") {
    Write-Host "Error: The current directory is not 'build'."
    return
}

msbuild .\GameEngineGL.sln
Debug\GameEngineGL.exe

Set-Location $originalDirectory
