#include "proj_handle_path.h"

#include <QCoreApplication>


QString	proj_data_path()
{
#ifndef IS_INSTALL
    return qApp->applicationDirPath();
#else
    return PROJ_DATA_PATH;
#endif
}

QString	proj_gsttoyunda_plugin_path()
{
#ifndef IS_INSTALL
    return qApp->applicationDirPath() + "/gsttoyunda/";
#else
    return GSTTOYUNDA_PLUGIN_PATH;
#endif

}


QString	proj_plugins_path()
{
#ifndef IS_INSTALL
    return qApp->applicationDirPath() + "/plugins/";
#else
    return PROJ_PLUGINS_PATH;
#endif
}

QString	proj_logs_path()
{
#ifndef IS_INSTALL
    return qApp->applicationDirPath();
#else
    return QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/qtoyunda-logs/";
#endif
}


QString proj_translation_path()
{
#ifndef IS_INSTALL
    return qApp->applicationDirPath() + "/translations/";
#else
    return PROJ_TRANSLATION_PATH;
#endif
}

