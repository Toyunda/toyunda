/****************************************************************************
** Meta object code from reading C++ file 'toyundarender.h'
**
** Created: Thu Mar 26 19:02:11 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "toyundarender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toyundarender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ToyundaRender[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ToyundaRender[] = {
    "ToyundaRender\0\0renderUpdate()\0"
};

const QMetaObject ToyundaRender::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ToyundaRender,
      qt_meta_data_ToyundaRender, 0 }
};

const QMetaObject *ToyundaRender::metaObject() const
{
    return &staticMetaObject;
}

void *ToyundaRender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToyundaRender))
	return static_cast<void*>(const_cast< ToyundaRender*>(this));
    return QObject::qt_metacast(_clname);
}

int ToyundaRender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: renderUpdate(); break;
        }
        _id -= 1;
    }
    return _id;
}
