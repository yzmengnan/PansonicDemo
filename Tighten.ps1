#run catch program
Write-Output "run the catch strategy program"
Set-Location build
cmake .. -G Ninja
ninja
& './Wrench.exe' '1' 'catch' '../config/catch.json' '854'
Write-Output  "the return code is $LASTEXITCODE"