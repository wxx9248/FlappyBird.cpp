@ECHO OFF

DEL /F /Q /S *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res 
DEL /F /Q /S *.ncb *.opt *.suo *.manifest *.dep *.sdf *.tlog *.log *.ipch *.lastbuildstate
DEL /F /Q "Flappy Bird\.vs\Flappy Bird\v15\Browse.VC.db"
rm -rf "Flappy Bird\x64"
rm -rf "Flappy Bird\Flappy Bird\x64"
rm -rf "Flappy Bird\.vs"

PAUSE
