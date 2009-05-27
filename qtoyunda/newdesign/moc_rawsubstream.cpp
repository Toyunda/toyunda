/****************************************************************************
** Meta object code from reading C++ file 'rawsubstream.h'
**
** Created: Thu Mar 26 19:02:11 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "rawsubstream.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rawsubstream.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_RawSubStream[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_RawSubStream[] = {
    "RawSubStream\0"
};

const QMetaObject RawSubStream::staticMetaObject = {
    { &ToyundaSubStream::staticMetaObject, qt_meta_stringdata_RawSubStream,
      qt_meta_data_RawSubStream, 0 }
};

const QMetaObject *RawSubStream::metaObject() const
{
    return &staticMetaObject;
}

void *RawSubStream::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RawSubStream))
	return static_cast<void*>(const_cast< RawSubStream*>(this));
    return ToyundaSubStream::qt_metacast(_clname);
}

int RawSubStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ToyundaSubStream::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
