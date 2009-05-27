/****************************************************************************
** Meta object code from reading C++ file 'qx11osdrender.h'
**
** Created: Fri Mar 6 20:06:41 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qx11osdrender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qx11osdrender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QX11OSDRender[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QX11OSDRender[] = {
    "QX11OSDRender\0"
};

const QMetaObject QX11OSDRender::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QX11OSDRender,
      qt_meta_data_QX11OSDRender, 0 }
};

const QMetaObject *QX11OSDRender::metaObject() const
{
    return &staticMetaObject;
}

void *QX11OSDRender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QX11OSDRender))
	return static_cast<void*>(const_cast< QX11OSDRender*>(this));
    if (!strcmp(_clname, "ToyundaRender"))
	return static_cast< ToyundaRender*>(const_cast< QX11OSDRender*>(this));
    return QWidget::qt_metacast(_clname);
}

int QX11OSDRender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
