#run pv to get position
Write-Output "run pv program"
& './build/Jogging.exe'
Write-Output  "the return code is $LASTEXITCODE"

