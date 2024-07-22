#run catch program
Write-Output "run the catch strategy program"
Set-Location build
& './Catch.exe' '1'

Write-Output  "the return code is $LASTEXITCODE"