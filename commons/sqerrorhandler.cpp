
#include <QDebug>
#include "sqerrorhandler.h"

SQErrorHandler::SQErrorHandler(QObject *parent) :
    QObject(parent)
{

}

void    SQErrorHandler::errorToQDebug(SQError err)
{
    if (err.level == SQError::Critical)
        qCritical() << err.message << err.info;
    if (err.level == SQError::Fatal)
        qCritical() << "FATAL : " << err.message << err.info;
    if (err.level == SQError::Warning)
        qWarning() <<  err.message << err.info;
}
