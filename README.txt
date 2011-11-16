Welcome in this readme, you will find informations about the project structure
and requierement to build it.




1 - Structure

	The projet have 3 component, a Gui, a pseudo library, and plugins

	-GuiliGui is the Gui part of the project

	-QToyunda lib is a pseudo library that containt the logic of the project

	-QToyunda Plugins are the fonctional part of the project

2 - QToyunda lib

	qtoyunda-lib directory contains a pseudo library QToyunda, with a helper class QToyunda
that hide the details of player/renderer synchronysing and subparting. It's a pseudo library
as it's just built as a static library, it's not a big project so I don't see the point
of making it a real dynamic library.

	qtoyunda logic is :
	We have players that play the stream and provide synchronysation signals
	We have a toyundasubstream class that manage the subtitle stream
	we have renderer that render the toyundasubstream.
  
	The QToyunda class loads plugins, connect renderer/player and subparser and proxypass
 signal of the player (like playing, stopped..). A minimal qtoyunda application will look
like that :
	QToyunda *toyunda = new toyunda("qgstaudio", "qosd", "path/to/plugins");
	toyunda->loadPlugins();
	toyunda->init(); // Init and connect player/renderer/subparser
	toyunda->open("My little pony.txt", "My little pony.avi");
	toyunda->play();

3 - GuiliGuili

	GuiliGuili is a smal Gui for the project, the goal is to provide a simple and easy to use
interface for users


4 - QToyunda Plugins

	Plugins provide the player and renderer part of the project. As toyunda have only
a subtitle format, the subparser is in the core
There are many goals of having them in plugin and not directly in the QToyunda core :
	-Reduce dependancies of the base project, (only depend on QtCore and QtGui (fuking qcolor class))
	-provide different feature, eg : fakeplayer and debugrenderer for developpers, real player
	 and renderer for user
	-fit the target plateform, eg : maybe provide QuickTime based player for os X

	see plugins directory to have more informations on plugins



HOW TO COMPILE THIS FUCKING PROJECT

Needed library :

	In you just want to play with the source code and don't have a real player :
	Only Qt SDK, you will have a fake player, a debug renderer and the qosd renderer
	Don't forget to delete qgstaudio.pro in plugins.pro file located in plugins directory

	In you want all supported stuff :
	Qt SDK, gstreamer, qtgstreamer


Plateform notes :

	On Linux : install -devel package of library needed
		   For gstreamer, don't forget to check your distribution documentation to have non free codecs (gstreamer-plugin-bad)

	On Windows :
		If you just want a out of box working QToyunda, go to the qtoyunda website and download the installer
		
		You want to compile QToyunda and co on windows : ok you are crazy. DON'T MAKE RELEASE BUILD, QtGStreamer seems
		having a prob with timer (need futher investigation) 

		I use Qtcreator + MSVC 9 (Visual Studio 2008) as compiler
		
		1/Download and install Qtcreator, and Qt SDK compiled with Visual Studio 2008 (search on qt-nokia website)
		
		2/Go to Qt-gstreamer wikipage to know how compile GStreamer and QtGstreamer http://gstreamer.freedesktop.org/wiki/QtGStreamer/BuildingOnWindows

		3/Copy the the ossbuild/Shared/Windows/Win32/Include content in ossbuild/Build/Windows/Win32/Realease/include/gstreamer-0.10
		Or find the good CMake variable that add Include prefix path (-DCMAKE_INCLUDE doesn't seems to work with qtgstreamer), This
		avoid missing include compile error.

		4/ Ok you got gstreamer and qt-gstreamer build. Open toyunda.pro with QtCreator (check if it use the good Qt version (msvc) in
		you have the MinGW version also installed, edit plugins/qgstaudio.pro include/lib variable if you install qt-gstreamer in different
		location from the documentation
		
		5/ Copy QtGstreamer's dll in the toyunda build/ directory
		
		6/ ONLY IF YOU WANT TO PROVIDE STANDALONE VERSION : 
			-Copy QtCore, QtGui, QtSVG, QtXml dll (debug version, with d in the end of the name),
			-Qt plugins for SVG : make a iconengines and imagesformat in toyunda/build and copy SVG related DLL
			-If you have a complete version of Visual Studio 2008, open QToyunda Installer.sln to build the installer
		 Open QToyunda installer.sln file if you want 