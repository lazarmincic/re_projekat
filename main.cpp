#include "dialog.h"
#include <QApplication>



int main(int argc, char *argv[])
{


 /*   if(wiringPiSetup())  wiringpi ne moze na windows
        return 1;
*/
    QApplication a(argc, argv);
    Dialog w;

    w.iNit1ialize_gpio(); // dugmad i led






    w.show();

    return a.exec();
}


