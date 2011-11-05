#include <QDebug>
#include "qdebugerrorhandler.h"


QDebugErrorHandler::QDebugErrorHandler()
{
}

void QDebugErrorHandler::addError(SQError err)
{
    qDebug() << "ADD ERROR";
    if (err.level == SQError::Critical)
        qCritical() << err.message << err.info;
    if (err.level == SQError::Fatal)
        qCritical() << "FATAL : " << err.message << err.info;
    if (err.level == SQError::Warning)
        qWarning() <<  err.message << err.info;
}
