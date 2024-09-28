#run catch program
Write-Output "run the wrench strategy program"
#TODO: set correct location
Set-Location "C:\Users\LR\Desktop\additional_axis\PansonicDemo\build"
$env:USE_SOCKET_COMMAND  = "TRUE"
cmake .. -G Ninja
ninja Wrench
& './Wrench.exe' '1' 'wrench' '../config/wrench.json' '854'
Write-Output  "the return code is $LASTEXITCODE"