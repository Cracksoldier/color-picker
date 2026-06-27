/****************************************************************************
** Meta object code from reading C++ file 'WaylandPortalPicker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/WaylandPortalPicker.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WaylandPortalPicker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN19WaylandPortalPickerE_t {};
} // unnamed namespace

template <> constexpr inline auto WaylandPortalPicker::qt_create_metaobjectdata<qt_meta_tag_ZN19WaylandPortalPickerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "WaylandPortalPicker",
        "colorPicked",
        "",
        "QColor",
        "color",
        "pickCanceled",
        "onResponse",
        "code",
        "QVariantMap",
        "results"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'colorPicked'
        QtMocHelpers::SignalData<void(QColor)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'pickCanceled'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onResponse'
        QtMocHelpers::SlotData<void(uint, QVariantMap)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 7 }, { 0x80000000 | 8, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<WaylandPortalPicker, qt_meta_tag_ZN19WaylandPortalPickerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject WaylandPortalPicker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19WaylandPortalPickerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19WaylandPortalPickerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19WaylandPortalPickerE_t>.metaTypes,
    nullptr
} };

void WaylandPortalPicker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<WaylandPortalPicker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->colorPicked((*reinterpret_cast<std::add_pointer_t<QColor>>(_a[1]))); break;
        case 1: _t->pickCanceled(); break;
        case 2: _t->onResponse((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (WaylandPortalPicker::*)(QColor )>(_a, &WaylandPortalPicker::colorPicked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (WaylandPortalPicker::*)()>(_a, &WaylandPortalPicker::pickCanceled, 1))
            return;
    }
}

const QMetaObject *WaylandPortalPicker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WaylandPortalPicker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19WaylandPortalPickerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WaylandPortalPicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void WaylandPortalPicker::colorPicked(QColor _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void WaylandPortalPicker::pickCanceled()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
