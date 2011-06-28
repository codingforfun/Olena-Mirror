/****************************************************************************
** Meta object code from reading C++ file 'image_viewer.hh'
**
** Created: Thu Jun 23 16:15:08 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/image_viewer.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'image_viewer.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mln__display__image_viewer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   28,   27,   27, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   57,   27,   27, 0x08,
      86,   80,   27,   27, 0x08,
     113,   80,   27,   27, 0x08,
     142,   28,   27,   27, 0x08,
     170,   27,   27,   27, 0x08,
     190,   27,   27,   27, 0x08,
     211,   27,   27,   27, 0x08,
     234,   27,   27,   27, 0x08,
     266,  260,   27,   27, 0x0a,
     298,  294,   27,   27, 0x0a,
     319,  294,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_mln__display__image_viewer[] = {
    "mln::display::image_viewer\0\0sli\0"
    "slider_valueChanged(int)\0b\0"
    "visible_slider(bool)\0value\0"
    "move_vertical_sliders(int)\0"
    "move_horizontal_sliders(int)\0"
    "on_slider_valueChanged(int)\0"
    "on_zoomIn_clicked()\0on_zoomOut_clicked()\0"
    "on_zoomFixed_clicked()\0on_zoomOriginal_clicked()\0"
    "nslis\0set_image_layer_count(uint)\0ima\0"
    "update_image(QImage)\0push_image(QImage)\0"
};

const QMetaObject mln::display::image_viewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_mln__display__image_viewer,
      qt_meta_data_mln__display__image_viewer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mln::display::image_viewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mln::display::image_viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mln::display::image_viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mln__display__image_viewer))
        return static_cast<void*>(const_cast< image_viewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int mln::display::image_viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: visible_slider((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: move_vertical_sliders((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: move_horizontal_sliders((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: on_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: on_zoomIn_clicked(); break;
        case 6: on_zoomOut_clicked(); break;
        case 7: on_zoomFixed_clicked(); break;
        case 8: on_zoomOriginal_clicked(); break;
        case 9: set_image_layer_count((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 10: update_image((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 11: push_image((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void mln::display::image_viewer::slider_valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
