/****************************************************************************
** Meta object code from reading C++ file 'interactive_scene.hh'
**
** Created: Thu Jun 23 14:07:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/internal/interactive_scene.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interactive_scene.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mln__display__internal__interactiveScene[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   42,   41,   41, 0x05,
      65,   42,   41,   41, 0x05,
      86,   42,   41,   41, 0x05,
     109,   42,   41,   41, 0x05,
     138,  133,   41,   41, 0x05,
     160,   41,   41,   41, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_mln__display__internal__interactiveScene[] = {
    "mln::display::internal::interactiveScene\0"
    "\0p\0mouse_click(QPointF)\0mouse_moved(QPointF)\0"
    "mouse_pressed(QPointF)\0mouse_released(QPointF)\0"
    "rect\0new_selection(QRectF)\0"
    "selection_cleared()\0"
};

const QMetaObject mln::display::internal::interactiveScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_mln__display__internal__interactiveScene,
      qt_meta_data_mln__display__internal__interactiveScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mln::display::internal::interactiveScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mln::display::internal::interactiveScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mln::display::internal::interactiveScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mln__display__internal__interactiveScene))
        return static_cast<void*>(const_cast< interactiveScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int mln::display::internal::interactiveScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mouse_click((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 1: mouse_moved((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 2: mouse_pressed((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 3: mouse_released((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 4: new_selection((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 5: selection_cleared(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void mln::display::internal::interactiveScene::mouse_click(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void mln::display::internal::interactiveScene::mouse_moved(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void mln::display::internal::interactiveScene::mouse_pressed(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void mln::display::internal::interactiveScene::mouse_released(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void mln::display::internal::interactiveScene::new_selection(const QRectF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void mln::display::internal::interactiveScene::selection_cleared()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
