
#include <QDebug>
#include "sqerror.h"

SQError::SQError()
{
}

SQError::SQError(SQError::Level lvl, QString msg, QVariant inf)
{
	level = lvl;
	message = msg;
	info = inf;
}
