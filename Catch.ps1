#run catch program
Write-Output "run the catch strategy program"
#TODO: set correct location
Set-Location build
$env:USE_SOCKET_COMMAND  = "TRUE"
cmake .. -G Ninja
ninja
& './Catch.exe' '1' 'catch' '../config/catch.json' '853'
Write-Output  "the return code is $LASTEXITCODE"