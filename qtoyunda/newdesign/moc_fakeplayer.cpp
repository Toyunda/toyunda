/****************************************************************************
** Meta object code from reading C++ file 'fakeplayer.h'
**
** Created: Thu Mar 26 19:02:09 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "fakeplayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fakeplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FakePlayer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      26,   11,   11,   11, 0x0a,
      33,   11,   11,   11, 0x0a,
      40,   11,   11,   11, 0x0a,
      50,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FakePlayer[] = {
    "FakePlayer\0\0open(QString)\0play()\0"
    "stop()\0seek(int)\0newTick()\0"
};

const QMetaObject FakePlayer::staticMetaObject = {
    { &FilePlayer::staticMetaObject, qt_meta_stringdata_FakePlayer,
      qt_meta_data_FakePlayer, 0 }
};

const QMetaObject *FakePlayer::metaObject() const
{
    return &staticMetaObject;
}

void *FakePlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FakePlayer))
	return static_cast<void*>(const_cast< FakePlayer*>(this));
    return FilePlayer::qt_metacast(_clname);
}

int FakePlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = FilePlayer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: open((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: play(); break;
        case 2: stop(); break;
        case 3: seek((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: newTick(); break;
        }
        _id -= 5;
    }
    return _id;
}
