Welcome in this readme, you will find informations about the project structure
and requierement to build it.




1 - Structure

	The projet have 4 components, a Gui, a pseudo library, plugins for the library
	and a toyunda player implementation

	-GuiliGui is the Gui part of the project

	-QToyunda lib is a pseudo library that containt the logic of the project

	-QToyunda Plugins are the fonctional part of the project

	-qtoyunda-cmd a command line tool to have more advanced acces to option, see ./qtoyunda-cmd --help
	
	-Gstreamer-toyunda is a toyunda implementation using gstreamer
	
	Don't be confused, I use QToyunda for QToyunda lib + plugins, GuiliGuili for the Gui, and Gst-Toyunda
	for Gstreamer-toyunda

2 - QToyunda lib

	qtoyunda-lib directory contains a pseudo library QToyunda, with a helper class QToyunda
that hide the details of player/renderer synchronysing and subparsing. It's a pseudo library
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

	GuiliGuili is a small Gui for the project, the goal is to provide a simple and easy to use
interface for playing toyunda songs.


4 - QToyunda Plugins

	Plugins provide the player and renderer part for QToyunda lib. As toyunda have only
a subtitle format, the subparser is in the core
There are many goals of having them in plugin and not directly in the QToyunda core :
	-Reduce dependancies of the base project, (only depend on QtCore and QtGui (fucking qcolor class))
	-provide different feature, eg : fakeplayer and debugrenderer for developpers, real player
	 and renderer for user
	-fit the target plateform, eg : maybe provide QuickTime based player for os X

	see plugins directory to have more informations on plugins

5 - GStreamer Toyunda
 
	The Gstreamer Toyunda is a reimplementation of the toyunda using the gstreamer SDK (0.10.36)
There is a gstreamer plugin and a small gstreamer/gtk application that use the plugin in a common
gstreamer pipeline.

HOW TO COMPILE ALL OF THIS

Needed library :

	In you just want to play with the source code and don't have a real player :
	  Only Qt SDK, you will have a fake player, a debug renderer and the qosd renderer
	  Don't forget to delete qgstaudio.pro in plugins.pro file located in plugins directory

	In you want all supported stuff :
	  Qt SDK, gstreamer SDK, qtgstreamer


Plateform notes :

	On Linux : first, install the devel package of the library needed, you also need libxfixes-devel for qosd renderer
		   For gstreamer, don't forget to check your distribution's documentation to have non free codecs 
	           (usualy gstreamer-plugin-bad)

	           Building :
	           for QToyunda and the Gui
		    -run 'qmake-qt4' in the project directory (toyunda)
		    -then 'make'
		   for Gstreamer-toyunda :
		    go in the gstreamer-toyunda directory
		    -run 'cmake .' (you probably have to install some library to pass this)
		    -then 'make'
		    
		    For now you can't install (like make install), all binaries/.so go in the "build" directory
		    -run what you want from the build directory
		    

	On Windows :
		If you just want a "out of box" working QToyunda, go to the qtoyunda website and download the installer
		
		If You want to compile QToyunda and co on windows : ok you are crazy. 
		DON'T MAKE RELEASE BUILD, QtGStreamer seems having a prob with QTimer (need futher investigation) 

		I use Qtcreator + MSVC 10 (Visual Studio 2010) as compiler
		
		1/Download and install Qtcreator and Qt SDK compiled with Visual Studio 20010 (search on qt-project website)
		
		2/Install the gstreamer sdk from gstreamer.com (not .net), runtime and header
		
		3/Go to Qt-gstreamer wikipage to know how compile QtGStreamer
		http://gstreamer.freedesktop.org/wiki/QtGStreamer/BuildingOnWindows

		4/ Ok you got gstreamer and qt-gstreamer. Open toyunda.pro with QtCreator (check if he use the good Qt version (msvc) if
		you have the MinGW version also installed), edit plugins/qgstaudio.pro include/lib variable if you install qt-gstreamer
		in different location from the documentation.
		
		5/ Compile the project.
		
		6/ For gstreamer-toyunda open the gstreamer-toyunda Visual Studio solution, maybe you will have to fix the property file
		path (see gstreamer sdk documentation)
		
		7/ Edit the deploy_win32.bat in the build directory to change the path of the variable
		
		8/ Run deploy_win32.bat in the build directory
		
		9/ You should have a working toyunda project
		
		You have to run deploy_win32.bat each time you change something in gstreamer-toyundasub
		
		If you want to create the installer, open the qtoyunda-installer VS solution

