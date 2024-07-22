#run pv to get position
Write-Output "run pv program"
Set-Location build
& './Jogging.exe'
Write-Output  "the return code is $LASTEXITCODE"

