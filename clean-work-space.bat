@ECHO OFF

DEL /F /Q /S *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res 
DEL /F /Q /S *.ncb *.opt *.suo *.manifest *.dep *.sdf *.tlog *.log *.ipch *.lastbuildstate
DEL /F /Q "Flappy Bird\.vs\Flappy Bird\v15\Browse.VC.db"
DEL /F /Q /S "x64"
DEL /F /Q /S "Flappy Bird\x64"

PAUSE
