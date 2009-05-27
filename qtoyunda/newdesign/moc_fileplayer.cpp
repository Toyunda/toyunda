/****************************************************************************
** Meta object code from reading C++ file 'fileplayer.h'
**
** Created: Thu Mar 26 19:02:10 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "fileplayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FilePlayer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      22,   11,   11,   11, 0x05,
      32,   11,   11,   11, 0x05,
      43,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   11,   11,   11, 0x0a,
      75,   11,   11,   11, 0x0a,
      82,   11,   11,   11, 0x0a,
      89,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FilePlayer[] = {
    "FilePlayer\0\0stopped()\0started()\0"
    "finished()\0frameChanged(int)\0open(QString)\0"
    "play()\0stop()\0seek(int)\0"
};

const QMetaObject FilePlayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FilePlayer,
      qt_meta_data_FilePlayer, 0 }
};

const QMetaObject *FilePlayer::metaObject() const
{
    return &staticMetaObject;
}

void *FilePlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FilePlayer))
	return static_cast<void*>(const_cast< FilePlayer*>(this));
    return QObject::qt_metacast(_clname);
}

int FilePlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stopped(); break;
        case 1: started(); break;
        case 2: finished(); break;
        case 3: frameChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: open((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: play(); break;
        case 6: stop(); break;
        case 7: seek((*reinterpret_cast< const int(*)>(_a[1]))); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void FilePlayer::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FilePlayer::started()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FilePlayer::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FilePlayer::frameChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
