@echo off

del /F /Q /S *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res 
del /F /Q /S *.ncb *.opt *.suo *.manifest *.dep *.sdf *.tlog *.log *.ipch *.lastbuildstate
del /F /Q "Flappy Bird\.vs\Flappy Bird\v15\Browse.VC.db"
del /F /Q /S "x64"
del /F /Q /S "Flappy Bird\x64"

pause
