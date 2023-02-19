#include "nit1.h"

Nit1::Nit1(Dialog* d,int bn) {

    dia =d;
    but_num = bn;
}


Nit1::~Nit1() {}

    // oboji must emit oboji_finished when it is done.
void Nit1:: oboji(){



    dia->oboji_notu(but_num);
    dia->sviraj(but_num); //svira i boji i pali z led
    dia->upali_diodu(1);
    delay(1000);
    dia->izbledi_note();
    dia->ugasi_diodu(1);

    connect (dia,&Dialog::button_pressed,dia,&Dialog::do_on_press);

    emit(oboji_finished());

}
