#run pv to get position
Write-Output "run pv program"
Set-Location build
cmake .. -G Ninja
ninja
$jsonName = Read-Host "set json file name"
Write-Output "the file name is" $jsonName
$port = Read-Host "set plc port: example:{853:hold machine;854:wrench tool;851:default}"
& './Jogging.exe' $jsonName $port
Write-Output  "the return code is $LASTEXITCODE"

