#run catch program
Write-Output "run the catch strategy program"
Set-Location build
cmake .. -G Ninja
ninja
& './Catch.exe' '1' 'catch'
Write-Output  "the return code is $LASTEXITCODE"
Pause