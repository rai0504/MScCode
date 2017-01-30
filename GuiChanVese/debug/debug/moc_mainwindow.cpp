/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 15), // "onCycleComplete"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 1), // "i"
QT_MOC_LITERAL(4, 30, 21), // "onParametersEstimated"
QT_MOC_LITERAL(5, 52, 2), // "pe"
QT_MOC_LITERAL(6, 55, 1), // "h"
QT_MOC_LITERAL(7, 57, 5), // "alpha"
QT_MOC_LITERAL(8, 63, 2), // "mu"
QT_MOC_LITERAL(9, 66, 2), // "l0"
QT_MOC_LITERAL(10, 69, 2), // "l1"
QT_MOC_LITERAL(11, 72, 22), // "onSegmentationComplete"
QT_MOC_LITERAL(12, 95, 21), // "onClassificationValue"
QT_MOC_LITERAL(13, 117, 18), // "on_pushImg_clicked"
QT_MOC_LITERAL(14, 136, 17), // "on_pushGt_clicked"
QT_MOC_LITERAL(15, 154, 34), // "on_comboMethod_currentIndexCh..."
QT_MOC_LITERAL(16, 189, 5), // "index"
QT_MOC_LITERAL(17, 195, 22) // "on_pushSegment_clicked"

    },
    "MainWindow\0onCycleComplete\0\0i\0"
    "onParametersEstimated\0pe\0h\0alpha\0mu\0"
    "l0\0l1\0onSegmentationComplete\0"
    "onClassificationValue\0on_pushImg_clicked\0"
    "on_pushGt_clicked\0on_comboMethod_currentIndexChanged\0"
    "index\0on_pushSegment_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    6,   57,    2, 0x0a /* Public */,
      11,    0,   70,    2, 0x0a /* Public */,
      12,    1,   71,    2, 0x0a /* Public */,
      13,    0,   74,    2, 0x08 /* Private */,
      14,    0,   75,    2, 0x08 /* Private */,
      15,    1,   76,    2, 0x08 /* Private */,
      17,    0,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,    8,    9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onCycleComplete((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->onParametersEstimated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        case 2: _t->onSegmentationComplete(); break;
        case 3: _t->onClassificationValue((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->on_pushImg_clicked(); break;
        case 5: _t->on_pushGt_clicked(); break;
        case 6: _t->on_comboMethod_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_pushSegment_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
