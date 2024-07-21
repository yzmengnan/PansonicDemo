#run catch program
echo "run the catch strategy program"
& './build/Catch.exe' '1'

Write-Output  "the return code is $LASTEXITCODE"