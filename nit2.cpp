#include "nit2.h"

Nit2::Nit2(Dialog* d) {

    dia =d;

}


Nit2::~Nit2() {}

    // oboji must emit oboji_finished when it is done.
void Nit2:: sr_vr_pot(){

const int n = 50;          // sto je vece to je sporiji program
    //qDebug()<<analogread();
int sr_vrednost_pot[n];

while (1)
{
     //1000ms = 1s
    int sr_vr = 0;
    int i= 0;

    while (i<=n)      // posto se vrednosti jako menjaju, citati ih i onda vratiti sredinu za neki vr. period 1s npr
        {
                //qDebug()<< "Trenutna vr. pot. je" << dia->analogread();
                sr_vrednost_pot [++i]= dia->analogread();

        }


    for (int j=0;j<n;j++)
        sr_vr += sr_vrednost_pot[j]; //suma svih el.
    sr_vr = sr_vr / n;
    //qDebug()<<  sr_vr;

// osam vrednosti

    //if (sr_vr < )
    /*

        double out = 0.0;

        while(1)
        {
            if (sr_vr < dia->pot_min + dia->pot_step /2) {out = 0; break;}



                    double temp = 1.5;
                    while (temp< (dia->bit -1) )
                    {
                        if((sr_vr < dia->pot_min + temp*dia->pot_step ) || (sr_vr > dia->pot_min + (temp-1)*dia->pot_step)) {out = temp-0.5; break; }
                        temp++;
                    }
                  /*  if      (sr_vr < dia->pot_min + temp*dia->pot_step) {sr_vr = i; temp++; i++;} // temp mora biti manje od step-1 (7 u ovom slucaju) i mora biti manje od step
                    else if (sr_vr < dia->pot_min + 2.5*dia->pot_step i pot_min + 1.5*pot_step) sr_vr = 2;
                    else if (sr_vr < dia->pot_min + 3.5*dia->pot_step) sr_vr = 3;
                    else if (sr_vr < dia->pot_min + 4.5*dia->pot_step) sr_vr = 4;
                    else if (sr_vr < dia->pot_min + 5.5*dia->pot_step) sr_vr = 5;
                    else if (sr_vr < dia->pot_min + 6.5*dia->pot_step) sr_vr = 6;   */

                  //  if (sr_vr > dia->pot_min + (bit-1)*dia->pot_step) {out = temp+0.5; break;}
      //  }






       // qDebug()<<"Slajder "<<out;




    emit(dia->vrednost_pot(sr_vr));

}
    emit(nit2_finished());

}
