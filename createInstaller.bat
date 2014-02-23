@echo off

bin\headergenerator.exe data data_tmp
bin\listgenerator.exe data_tmp data_tmp\patch.lst
xcopy res\data data_tmp /c /f /y /e /h
bin\mpqcreate.exe tmp.mpq data_tmp
bin\append.exe res\base.exe YourInstaller.exe tmp.mpq res\Installer.exe res\RichEd20.dll res\Unicows.dll

echo.
echo.
echo Press to delete temp files

pause

del tmp.mpq
rmdir data_tmp /Q /S

pause