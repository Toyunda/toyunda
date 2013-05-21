@echo off
set distDir=X:\qttoyunda-dev-deploy
set baseBuildDir=C:\toyunda\build

mkdir %distDir%
mkdir %distDir%\gsttoyunda
echo Copying proj File

FOR %%p IN (GuiliGuili.exe toyunda-player.exe toyunda.tga VeraMoBd.ttf) DO xcopy /y %baseBuildDir%\%%p %distDir%

xcopy /y %baseBuildDir%\gsttoyunda\gsttoyunda.dll %distDir%\gsttoyunda

echo Copying plugins file

mkdir %distDir%\plugins

FOR %%p IN (plugins\qtoyunda_debugrendererd.dll plugins\qtoyunda_qgstaudioplayerd.dll plugins\qtoyunda_fakeplayerd.dll plugins\qtoyunda_qosdrendererd.dll) DO xcopy /y %baseBuildDir%\%%p %distDir%\plugins


echo Copying Qt File

FOR %%p IN (QtGstreamerUi-0.10.dll QtGstreamer-0.10.dll QtGLib-2.0.dll QtCored4.dll QtGuid4.dll QtSvgd4.dll QtXmld4.dll) DO xcopy /y %baseBuildDir%\%%p %distDir%

mkdir %distDir%\iconengines
mkdir %distDir%\imageformats

xcopy /y /e %baseBuildDir%\iconengines %distDir%\iconengines
xcopy /y /e %baseBuildDir%\imageformats %distDir%\imageformats

echo Copying MSV file

FOR %%p IN (msvcp100d.dll msvcr100d.dll) DO xcopy /y %baseBuildDir%\%%p %distDir%



