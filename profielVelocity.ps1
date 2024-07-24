#run pv to get position
Write-Output "run pv program"
Set-Location build
cmake .. -G Ninja
ninja
$jsonName = Read-Host "set json file name"
Write-Output "the file name is" $jsonName
& './Jogging.exe' $jsonName
Write-Output  "the return code is $LASTEXITCODE"

