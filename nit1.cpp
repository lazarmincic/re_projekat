#include "nit1.h"

Nit1::Nit1(Dialog* d,int bn) {

    dia =d;
    but_num = bn;
}


Nit1::~Nit1() {}

    // oboji must emit oboji_finished when it is done.
void Nit1:: oboji(){

    //disconnect (dia,&Dialog::button_pressed,dia,&Dialog::dugme_pritisnuto);

    int dioda = 1; // podr, zelena
    dia->oboji_notu(but_num);
    dia->sviraj(but_num); //svira i boji i pali z led

    dia->pisi_u_mem( dia->rec_mode_on,but_num);

    if (dia->rec_mode_on) dioda = 0;
    else if (dia->test_mode_on) dioda = 2;
    else if (dia->play_mode_on) dioda = 1;

    dia->upali_diodu(dioda);
    delay(1000);
    dia->izbledi_note();
    dia->ugasi_diodu(dioda);

    connect (dia,&Dialog::button_pressed,dia,&Dialog::do_on_press); //dok jedna svira druga nece moci da se pritisne
    //connect (dia,&Dialog::button_pressed,dia,&Dialog::dugme_pritisnuto);

    emit(oboji_finished());

}
