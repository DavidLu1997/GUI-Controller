/****************************************************************************
** Meta object code from reading C++ file 'benchtop.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../benchtop.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'benchtop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Benchtop[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      18,    9,    9,    9, 0x08,
      25,    9,    9,    9, 0x08,
      39,    9,    9,    9, 0x08,
      51,    9,    9,    9, 0x08,
      63,    9,    9,    9, 0x08,
      71,    9,    9,    9, 0x08,
      80,    9,    9,    9, 0x08,
      86,    9,    9,    9, 0x08,
      94,    9,    9,    9, 0x08,
     105,    9,    9,    9, 0x08,
     117,    9,    9,    9, 0x08,
     125,    9,    9,    9, 0x08,
     138,    9,    9,    9, 0x08,
     153,    9,    9,    9, 0x08,
     166,    9,    9,    9, 0x08,
     178,    9,    9,    9, 0x08,
     192,    9,    9,    9, 0x08,
     204,    9,    9,    9, 0x08,
     218,    9,    9,    9, 0x08,
     228,    9,    9,    9, 0x08,
     237,    9,    9,    9, 0x08,
     250,    9,    9,    9, 0x08,
     262,    9,    9,    9, 0x08,
     278,    9,    9,    9, 0x08,
     291,    9,    9,    9, 0x08,
     299,    9,    9,    9, 0x08,
     315,    9,    9,    9, 0x08,
     333,    9,    9,    9, 0x08,
     354,    9,    9,    9, 0x08,
     361,    9,    9,    9, 0x08,
     376,    9,    9,    9, 0x08,
     395,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Benchtop[] = {
    "Benchtop\0\0reset()\0move()\0adjustSpeed()\0"
    "setLambda()\0setBregma()\0leftM()\0"
    "rightM()\0upM()\0downM()\0forwardM()\0"
    "backwardM()\0xboxM()\0checkTimer()\0"
    "updateMotors()\0macroStart()\0macroStop()\0"
    "macroRecord()\0macroPlay()\0macroUpdate()\0"
    "homeSet()\0homeGo()\0machineSet()\0"
    "machineGo()\0updateDisplay()\0checkPanic()\0"
    "panic()\0applySettings()\0defaultSettings()\0"
    "setDefaultSettings()\0copy()\0showSettings()\0"
    "showInstructions()\0showAbout()\0"
};

void Benchtop::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Benchtop *_t = static_cast<Benchtop *>(_o);
        switch (_id) {
        case 0: _t->reset(); break;
        case 1: _t->move(); break;
        case 2: _t->adjustSpeed(); break;
        case 3: _t->setLambda(); break;
        case 4: _t->setBregma(); break;
        case 5: _t->leftM(); break;
        case 6: _t->rightM(); break;
        case 7: _t->upM(); break;
        case 8: _t->downM(); break;
        case 9: _t->forwardM(); break;
        case 10: _t->backwardM(); break;
        case 11: _t->xboxM(); break;
        case 12: _t->checkTimer(); break;
        case 13: _t->updateMotors(); break;
        case 14: _t->macroStart(); break;
        case 15: _t->macroStop(); break;
        case 16: _t->macroRecord(); break;
        case 17: _t->macroPlay(); break;
        case 18: _t->macroUpdate(); break;
        case 19: _t->homeSet(); break;
        case 20: _t->homeGo(); break;
        case 21: _t->machineSet(); break;
        case 22: _t->machineGo(); break;
        case 23: _t->updateDisplay(); break;
        case 24: _t->checkPanic(); break;
        case 25: _t->panic(); break;
        case 26: _t->applySettings(); break;
        case 27: _t->defaultSettings(); break;
        case 28: _t->setDefaultSettings(); break;
        case 29: _t->copy(); break;
        case 30: _t->showSettings(); break;
        case 31: _t->showInstructions(); break;
        case 32: _t->showAbout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Benchtop::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Benchtop::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Benchtop,
      qt_meta_data_Benchtop, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Benchtop::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Benchtop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Benchtop::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Benchtop))
        return static_cast<void*>(const_cast< Benchtop*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Benchtop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
