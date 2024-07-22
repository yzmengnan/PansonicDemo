#run catch program
Write-Output "run the catch strategy program"
Set-Location build
cmake .. -G Ninja
& './Catch.exe' '0'
Write-Output  "the return code is $LASTEXITCODE"
