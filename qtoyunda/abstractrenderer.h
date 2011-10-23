#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include <qplugin.h>

class ToyundaRenderer;

class   AbstractRenderer
{
       public:
        virtual ToyundaRenderer*  getMe() = 0;
};

Q_DECLARE_INTERFACE(AbstractRenderer, "qtoyunda.AbstractRenderer")

#endif // ABSTRACTRENDERER_H
