
It doesn't work in release mode. Maybe a bug in QtGstreamer? (QTimer prob?)

For windows Only

Add QtGui and QtCore, and msvxx.dll (xx is the version of your msvc++)

For SVG support (icon in GuiliGuili)
add :
QtXML, imageformats/sgv4 et iconeengines/sgv4 for SVG