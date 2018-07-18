/****************************************************************************
** Meta object code from reading C++ file 'highway_gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../graphic-highway/Graphics/highway_gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'highway_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HighwayGui_t {
    QByteArrayData data[11];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HighwayGui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HighwayGui_t qt_meta_stringdata_HighwayGui = {
    {
QT_MOC_LITERAL(0, 0, 10), // "HighwayGui"
QT_MOC_LITERAL(1, 11, 13), // "standardStart"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "customStart"
QT_MOC_LITERAL(4, 38, 17), // "displayCustomForm"
QT_MOC_LITERAL(5, 56, 11), // "moveVehicle"
QT_MOC_LITERAL(6, 68, 10), // "vehicle_id"
QT_MOC_LITERAL(7, 79, 4), // "lane"
QT_MOC_LITERAL(8, 84, 5), // "x_pos"
QT_MOC_LITERAL(9, 90, 5), // "y_pos"
QT_MOC_LITERAL(10, 96, 13) // "to_be_tracked"

    },
    "HighwayGui\0standardStart\0\0customStart\0"
    "displayCustomForm\0moveVehicle\0vehicle_id\0"
    "lane\0x_pos\0y_pos\0to_be_tracked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HighwayGui[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    5,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    6,    7,    8,    9,   10,

       0        // eod
};

void HighwayGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HighwayGui *_t = static_cast<HighwayGui *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->standardStart(); break;
        case 1: _t->customStart(); break;
        case 2: _t->displayCustomForm(); break;
        case 3: _t->moveVehicle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObject HighwayGui::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_HighwayGui.data,
      qt_meta_data_HighwayGui,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HighwayGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HighwayGui::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HighwayGui.stringdata0))
        return static_cast<void*>(const_cast< HighwayGui*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int HighwayGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
