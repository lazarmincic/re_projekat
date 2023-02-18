#ifndef NIT1_H
#define NIT1_H

#include <QObject>
#include "dialog.h"

class Nit1 : public QObject  //boji
{
    Q_OBJECT
public:
    Nit1(Dialog *d,int bn);
    ~Nit1();
public slots:
    // oboji must emit oboji_finished when it is done.
    void oboji();


signals:
    void oboji_finished();


private:
    Dialog* dia;
    int but_num;
};

#endif // NIT1_H








