@echo off

set qtgstreamerPath=c:\qt-gstreamer\install\
set QtPath=C:\Qt\4.8.4\
set deployPath=C:\toyunda\build\
set basePath=C:\toyunda\


echo Echoing some variable

echo GStreamer path %GSTREAMER_SDK_ROOT_X86%

echo Deploy Path %deployPath%

echo Qt Path %QtPath%

echo QtGstreamer path %qtgstreamerPath%

echo Starting copying file

xcopy /y %qtgstreamerPath%\lib\QtGstreamer-0.10.dll %deployPath%
xcopy /y %qtgstreamerPath%\lib\QtGLib-2.0.dll %deployPath%

echo Copying Qt's file

FOR %%p IN (QtCored4.dll QtGuid4.dll QtSvgd4.dll QtXmld4.dll) DO xcopy %QtPath%\bin\%%p %deployPath%

mkdir %deployPath%\iconengines
mkdir %deployPath%\imageformats

xcopy /y %QtPath%\plugins\iconengines\qsvgicond4.dll %deployPath%\iconengines
xcopy /y %QtPath%\plugins\imageformats\qsvgd4.dll %deployPath%\imageformats\

echo Copying gstreamertoyunda file

xcopy /y %basePath%\gstreamer-toyunda\Debug\toyunda-player.exe %deployPath%
xcopy /y %basePath%\gstreamer-toyunda\Debug\gsttoyunda.dll %deployPath%

