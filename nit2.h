#ifndef NIT2_H
#define NIT2_H

#include <QObject>
#include "dialog.h"

class Nit2 : public QObject  //boji
{
    Q_OBJECT
public:
    Nit2(Dialog *d);
    ~Nit2();
public slots:
    // oboji must emit nit2_finished when it is done.
    void sr_vr_pot();


signals:
    void nit2_finished();


private:
    Dialog *dia;

};
#endif // NIT2_H
