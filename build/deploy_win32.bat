@echo off

set qtgstreamerPath=c:\qt-gstreamer\install\
set QtPath=C:\Qt\4.8.4\
set deployPath=C:\toyunda\build\
set basePath=C:\toyunda\
set visualPath="C:\Program Files\Microsoft Visual Studio 10.0\"

echo Echoing some variable

echo GStreamer path %GSTREAMER_SDK_ROOT_X86%

echo Deploy Path %deployPath%

echo Qt Path %QtPath%

echo QtGstreamer path %qtgstreamerPath%

echo Starting copying file

xcopy /y %qtgstreamerPath%\lib\QtGstreamer-0.10.dll %deployPath%
xcopy /y %qtgstreamerPath%\lib\QtGLib-2.0.dll %deployPath%
xcopy /y %qtgstreamerPath%\lib\QtGStreamerUi-0.10.dll %deployPath%

echo Copying Qt's file

FOR %%p IN (QtCored4.dll QtGuid4.dll QtSvgd4.dll QtXmld4.dll) DO xcopy /y %QtPath%\bin\%%p %deployPath%

mkdir %deployPath%\iconengines
mkdir %deployPath%\imageformats

xcopy /y %QtPath%\plugins\iconengines\qsvgicond4.dll %deployPath%\iconengines
xcopy /y %QtPath%\plugins\imageformats\qsvgd4.dll %deployPath%\imageformats\

echo Copying gstreamertoyunda file

mkdir %deployPath%\gsttoyunda

xcopy /y %basePath%\gstreamer-toyunda\Debug\toyunda-player.exe %deployPath%
xcopy /y %basePath%\gstreamer-toyunda\Debug\gsttoyunda.dll %deployPath%\gsttoyunda

echo Copying Visual C library

xcopy /y %visualPath%\vc\redist\debug_nonredist\x86\Microsoft.VC100.DebugCRT\msvcp100d.dll %deployPath%
xcopy /y %visualPath%\vc\redist\debug_nonredist\x86\Microsoft.VC100.DebugCRT\msvcr100d.dll %deployPath%


