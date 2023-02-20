#include "dialog.h"
#include "ui_dialog.h"

#include "nit1.h"
#include "nit2.h"



Dialog::Dialog(QWidget *parent) :  //konstruktor
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

   // QMetaObject::connectSlotsByName(this);
//  connect
    QTimer *timer = new QTimer(this);
    connect (timer,&QTimer::timeout,this, &Dialog::detect_press); // kad tajmer odbroji d_p se izvrsava
    timer->start(REF_RATE);

    detect_press();


    connect (this,&Dialog::button_pressed,this,&Dialog::do_on_press); // kad je dugme prit. izvrsava se d_o_p



    //mod = moNit1or; // pod. vrednost
    choose_note(ui->comboBox_change_note_1);
    choose_note(ui->comboBox_change_note_2);
    choose_note(ui->comboBox_change_note_3);
    choose_note(ui->comboBox_change_note_4);
    choose_note(ui->comboBox_change_note_5);
    choose_note(ui->comboBox_change_note_6);
    choose_note(ui->comboBox_change_note_7);

    // treba podesiti pod. vrednosti

     ui->comboBox_change_note_1->setCurrentIndex(0) ;
     ui->comboBox_change_note_2->setCurrentIndex(2) ;
     ui->comboBox_change_note_3->setCurrentIndex(4) ;
     ui->comboBox_change_note_4->setCurrentIndex(5) ;
     ui->comboBox_change_note_5->setCurrentIndex(7) ;
     ui->comboBox_change_note_6->setCurrentIndex(9) ;
     ui->comboBox_change_note_7->setCurrentIndex(11) ;

    // povezi signal minor sa slotom puni_p_meni_minor
    connect (this,&Dialog::minor,this,&Dialog::puni_p_meni_minor);

    puni_p_meni_major(); // inic vrednost p menija

    ui->comboBox_scale->setCurrentIndex(0); // inic vr

    on_pushButton_major_clicked(1); // inic vrednost major dugmeta

    connect (this,&Dialog::major,this,&Dialog::puni_p_meni_major);

    connect(ui->pushButton_flatten_1,&QPushButton::clicked,this,&Dialog::on_pushButton_flatten_1_pritisnuto);
    connect(ui->pushButton_flatten_2,&QPushButton::clicked,this,&Dialog::on_pushButton_flatten_2_pritisnuto);
    connect(ui->pushButton_flatten_3,&QPushButton::clicked,this,&Dialog::on_pushButton_flatten_3_pritisnuto);
    connect(ui->pushButton_flatten_4,&QPushButton::clicked,this,&Dialog::on_pushButton_flatten_4_pritisnuto);
    connect(ui->pushButton_flatten_5,&QPushButton::clicked,this,&Dialog::on_pushButton_flatten_5_pritisnuto);
    connect(ui->pushButton_flatten_6,&QPushButton::clicked,this,&Dialog::on_pushButton_flatten_6_pritisnuto);
    connect(ui->pushButton_flatten_7,&QPushButton::clicked,this,&Dialog::on_pushButton_flatten_7_pritisnuto);


    connect(ui->pushButton_sharpen_1,&QPushButton::clicked,this,&Dialog::on_pushButton_sharpen_1_pritisnuto);
    connect(ui->pushButton_sharpen_2,&QPushButton::clicked,this,&Dialog::on_pushButton_sharpen_2_pritisnuto);
    connect(ui->pushButton_sharpen_3,&QPushButton::clicked,this,&Dialog::on_pushButton_sharpen_3_pritisnuto);
    connect(ui->pushButton_sharpen_4,&QPushButton::clicked,this,&Dialog::on_pushButton_sharpen_4_pritisnuto);
    connect(ui->pushButton_sharpen_5,&QPushButton::clicked,this,&Dialog::on_pushButton_sharpen_5_pritisnuto);
    connect(ui->pushButton_sharpen_6,&QPushButton::clicked,this,&Dialog::on_pushButton_sharpen_6_pritisnuto);
    connect(ui->pushButton_sharpen_7,&QPushButton::clicked,this,&Dialog::on_pushButton_sharpen_7_pritisnuto);



  //  ocitaj_pot(); // mora u tredu!



    //ocitaj_pot();


    connect (this, &Dialog::vrednost_pot, this, &Dialog::move_vslider);

    QThread* thread_p = new QThread( ); // napravi novi tred da bi mogao da obojis labelu
    Nit2* task_p = new Nit2(this);  //color when pressed in another thread
    task_p->moveToThread(thread_p);

    Dialog::connect(thread_p, &QThread::started, task_p, &Nit2::sr_vr_pot);
    connect(task_p, &Nit2::nit2_finished, thread_p, &QThread::quit);
    connect(task_p, &Nit2::nit2_finished, task_p, &Nit2::deleteLater);
    connect(thread_p, &QThread::finished, thread_p, &QThread::deleteLater);

    thread_p->start();




    pot_step = (pot_max - pot_min) / bit ; // nisam koristio na kraju

    ui->verticalSlider_1->setRange(pot_min,pot_max);



    ui->horizontalSlider_octave->setRange(-1,7); //octave slider
    ui->horizontalSlider_octave->setValue(3); // pod. vr.
    ui->horizontalSlider_octave->setTickPosition(QSlider::TicksAbove);
    ui->horizontalSlider_octave->setTickInterval(1);
    ui->horizontalSlider_octave->setPageStep(1);

    ui->label_octave->setText("3");

    connect(ui->horizontalSlider_octave, &QSlider::valueChanged, this, &Dialog::setuj_tekst);
    connect(ui->horizontalSlider_octave, &QSlider::valueChanged, this, &Dialog::promeni_oktavu);


    ui->comboBox_samples->addItems(sample_names);
    ui->comboBox_samples->setCurrentIndex(0);

    connect(ui->comboBox_samples, SIGNAL(currentIndexChanged(int)),this,SLOT(bira_samplove(int)));




    connect(ui->pushButton_test, SIGNAL(toggled(bool)),this,SLOT(test_mod(bool)));


    connect (this,&Dialog::button_pressed,this,&Dialog::dugme_pritisnuto);

    connect (ui->pushButton_rec,&QPushButton::toggled,this,&Dialog::rec_mod);


    connect (ui->pushButton_play,&QPushButton::toggled,this,&Dialog::play_mod);


    this->inic_memorije(); // inic. dva pom. niza za rec mod

}



Dialog::~Dialog()
{
    delete ui;
}

//void Dialog::connect(





void Dialog::detect_press() // izvrsava se svakih reff_rate
{
    for(int i=0;i<8;i++)
    {
        if (digitalRead(button[i])){

            emit (button_pressed(i)); // 0 pauza 1-7 dugme
            }
    }

}

char Dialog::get_button(int i) const
{
    return button[i];
}

char Dialog::get_led(int i) const
{
    return led[i];
}

void Dialog::do_on_press(int but_num)
{
    disconnect (this,&Dialog::button_pressed,this,&Dialog::do_on_press);
   // disconnect (this,&Dialog::button_pressed,this,&Dialog::dugme_pritisnuto);


    QThread* thread = new QThread( ); // napravi novi tred da bi mogao da obojis labelu
    Nit1* task = new Nit1(this,but_num);  //color when pressed in another thread
    task->moveToThread(thread);

    Dialog::connect(thread, &QThread::started, task, &Nit1::oboji);
    connect(task, &Nit1::oboji_finished, thread, &QThread::quit);
    connect(task, &Nit1::oboji_finished, task, &Nit1::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    connect(thread, &QThread::finished,this,&Dialog::thread_done);



    thread->start();

    thread_fin = 0;

    qDebug()<<"Pritisnuto dugme broj "<< but_num;






}


void Dialog::iNit1ialize_gpio()
{
    for (int i = 0;i<8;i++)
        pinMode (button[i],INPUT);
    for (int i = 0;i<3;i++)
        pinMode (led[i], OUTPUT);


}

//!!!!!!!!!!!!!!!!!!

void Dialog::oboji_notu(int but_num) // on
{
   // QString temp = ;
    switch(but_num)
    {
                    case 0: break;
                    case 1: this->oboji_sliku_labele(ui->label_1,ui->comboBox_change_note_1->currentText()); break;
                    case 2: this->oboji_sliku_labele(ui->label_2,ui->comboBox_change_note_2->currentText()); break;
                    case 3: this->oboji_sliku_labele(ui->label_3,ui->comboBox_change_note_3->currentText()); break;
                    case 4: this->oboji_sliku_labele(ui->label_4,ui->comboBox_change_note_4->currentText()); break;
                    case 5: this->oboji_sliku_labele(ui->label_5,ui->comboBox_change_note_5->currentText()); break;
                    case 6: this->oboji_sliku_labele(ui->label_6,ui->comboBox_change_note_6->currentText()); break;
                    case 7: this->oboji_sliku_labele(ui->label_7,ui->comboBox_change_note_7->currentText()); break;
                    default: ;
    }


}

void Dialog::izbledi_note()
{
    on_comboBox_change_note_1_currentTextChanged(ui->comboBox_change_note_1->currentText());
    on_comboBox_change_note_2_currentTextChanged(ui->comboBox_change_note_2->currentText());
    on_comboBox_change_note_3_currentTextChanged(ui->comboBox_change_note_3->currentText());
    on_comboBox_change_note_4_currentTextChanged(ui->comboBox_change_note_4->currentText());
    on_comboBox_change_note_5_currentTextChanged(ui->comboBox_change_note_5->currentText());
    on_comboBox_change_note_6_currentTextChanged(ui->comboBox_change_note_6->currentText());
    on_comboBox_change_note_7_currentTextChanged(ui->comboBox_change_note_7->currentText());

}

void Dialog::promeni_sliku_labele(QLabel *l,QString s) // ul parametar qstring //off!!
{
    QString temp = ":/media/";
    temp.append(s);
    temp.append("_off");
    QPixmap slika (temp);
    l->setPixmap(slika);
    //delay(1000); nikad!! u gui

 //    slika.load (":media/C_on.png");
//    l->setPixmap(slika);
    //qDebug()<<"Izvrsio se setPixmap ";

}

void Dialog:: oboji_sliku_labele(QLabel *l,QString s)
{

    QString temp = ":/media/";
    temp.append(s);
    temp.append("_on");
    QPixmap slika (temp);
    l->setPixmap(slika);

}
/*
void Dialog::thread_delay(int ms) {

      QThread wait_thread (this); // dilej funkcija
      wait_thread.start();
      wait_thread.msleep(ms);
      wait_thread.quit();
   //   qDebug()<<"Izvrsio se dilej ";
}
*/



////////////// treba da napravim nesto da kad se promeni indeks padajuceg menija promene se i slike zajedno


void Dialog::choose_note(QComboBox *c) // ubacivanje imena nota u padajuci meni
{
   // int sharp_or_flat_index [5] = {1,3,6,8,10};
    int old_index = c->currentIndex();
   // for (int i =0; i<5 ; i++)
   //     if (old_index == sharp_or_flat_index[i]) // proverava da li je trenutna vrednost sarp ili flet


    if (c->count()!=0) c->clear();
    for (int i=0;i<12;i++)
    {
        if (sharp_or_flat==sharp)
        {
            QString string_sharp (note_names_sharp[i]);
            c->insertItem(i+1,string_sharp,0);
        }
        if (sharp_or_flat==flat)
        {
            QString string_flat (note_names_flat[i]);
            c->insertItem(i+1,string_flat,0);
        }
    }
    c->setCurrentIndex(old_index);
}



void Dialog::on_checkBox_on_off_stateChanged(int arg1)
{
    if (arg1==Qt::Unchecked)
        disconnect (this,&Dialog::button_pressed,this,&Dialog::do_on_press);
    if (arg1 == Qt::Checked)
        connect (this,&Dialog::button_pressed,this,&Dialog::do_on_press);

}

void Dialog::on_radioButton_sharp_clicked(bool checked)
{
    if (checked)
    {
        this->sharp_or_flat = sharp;
        choose_note(ui->comboBox_change_note_1);
        choose_note(ui->comboBox_change_note_2);
        choose_note(ui->comboBox_change_note_3);
        choose_note(ui->comboBox_change_note_4);
        choose_note(ui->comboBox_change_note_5);
        choose_note(ui->comboBox_change_note_6);
        choose_note(ui->comboBox_change_note_7);


        if(ui->pushButton_major->isChecked())
            puni_p_meni_major();
        else if(ui->pushButton_minor->isChecked())
            puni_p_meni_minor();


    }

}

void Dialog::on_radioButton_flat_clicked(bool checked)
{
    if (checked)
    {
        this->sharp_or_flat = flat;
        choose_note(ui->comboBox_change_note_1);
        choose_note(ui->comboBox_change_note_2);
        choose_note(ui->comboBox_change_note_3);
        choose_note(ui->comboBox_change_note_4);
        choose_note(ui->comboBox_change_note_5);
        choose_note(ui->comboBox_change_note_6);
        choose_note(ui->comboBox_change_note_7);


        if(ui->pushButton_major->isChecked())
            puni_p_meni_major();
        else if(ui->pushButton_minor->isChecked())
            puni_p_meni_minor();

    }


}


// kada je u padajucem meniju izabrana nota, menja se slika
void Dialog::on_comboBox_change_note_1_activated(const QString &arg1) // all obsolete
{
    promeni_sliku_labele(ui->label_1,arg1);
}

void Dialog::on_comboBox_change_note_2_activated(const QString &arg1)
{
    promeni_sliku_labele(ui->label_2,arg1);
}

void Dialog::on_comboBox_change_note_3_activated(const QString &arg1)
{
    promeni_sliku_labele(ui->label_3,arg1);
}

void Dialog::on_comboBox_change_note_4_activated(const QString &arg1)
{
    promeni_sliku_labele(ui->label_4,arg1);
}

void Dialog::on_comboBox_change_note_5_activated(const QString &arg1)
{
    promeni_sliku_labele(ui->label_5,arg1);

}

void Dialog::on_comboBox_change_note_6_activated(const QString &arg1)
{
    promeni_sliku_labele(ui->label_6,arg1);
}

void Dialog::on_comboBox_change_note_7_activated(const QString &arg1)
{
    promeni_sliku_labele(ui->label_7,arg1);
}




///////////////////






void Dialog::on_pushButton_minor_clicked(bool checked)
{
    if (checked)
    {
        ui->pushButton_major->setChecked(0);
        emit minor(); // signal
    }
    if (!checked)
    {
        ui->pushButton_major->setChecked(1);
        emit major();
    }
}




void Dialog::puni_p_meni_minor( ) //slot
{
    QComboBox *c = new QComboBox ();
    c = ui->comboBox_scale;
    int index_stari = c->currentIndex();
    if (c->count()!=0) c->clear();
    for (int i=0;i<8;i++) // puni sa minor imenima
    {
        if (sharp_or_flat==sharp)
        {

            QString string_sharp (minor_scale_names_sharp[i]);
            c->insertItem(i+1,string_sharp,0); // ubacivanje imena nota u padajuci meni
        }
        if (sharp_or_flat==flat)
        {
            QString string_flat (minor_scale_names_flat[i]);
            c->insertItem(i+1,string_flat,0);
        }
    }

    c->setCurrentIndex(index_stari);
}

void Dialog::puni_p_meni_major( ) //slot
{
    QComboBox *c = new QComboBox ();
    c = ui->comboBox_scale;
    int index_stari = c->currentIndex();
    if (c->count()!=0) c->clear();
    for (int i=0;i<8;i++) // puni sa minor imenima
    {
        if (sharp_or_flat==sharp)
        {

            QString string_sharp (major_scale_names_sharp[i]);
            c->insertItem(i+1,string_sharp,0); // ubacivanje imena nota u padajuci meni
        }
        if (sharp_or_flat==flat)
        {
            QString string_flat (major_scale_names_flat[i]);
            c->insertItem(i+1,string_flat,0);
        }
    }

     c->setCurrentIndex(index_stari);
}



void Dialog::on_pushButton_major_clicked(bool checked)
{
    if (checked)
    {
        ui->pushButton_minor->setChecked(0);
        emit major(); // signal
    }
    if (!checked)
    {
        ui->pushButton_minor->setChecked(1);
        emit minor();
    }
}



void Dialog::on_comboBox_change_note_1_currentTextChanged(const QString &arg1) // promena teksta usled pritiska flat sharp dugmeta treba da uzrokuje da se slike azuriraju
{
    promeni_sliku_labele(ui->label_1,arg1);
}

void Dialog::on_comboBox_change_note_2_currentTextChanged(const QString &arg1)
{
    promeni_sliku_labele(ui->label_2,arg1);
}

void Dialog::on_comboBox_change_note_3_currentTextChanged(const QString &arg1)
{
    promeni_sliku_labele(ui->label_3,arg1);
}

void Dialog::on_comboBox_change_note_4_currentTextChanged(const QString &arg1)
{
    promeni_sliku_labele(ui->label_4,arg1);
}

void Dialog::on_comboBox_change_note_5_currentTextChanged(const QString &arg1)
{
    promeni_sliku_labele(ui->label_5,arg1);
}

void Dialog::on_comboBox_change_note_6_currentTextChanged(const QString &arg1)
{
    promeni_sliku_labele(ui->label_6,arg1);
}

void Dialog::on_comboBox_change_note_7_currentTextChanged(const QString &arg1)
{
    promeni_sliku_labele(ui->label_7,arg1);
}













void Dialog::on_comboBox_scale_currentTextChanged(const QString &arg1)
{

    for (int i=0; i<30;i++) // 30 skala postoji
   {

       QString text;


       if (all_scales [i][0] == arg1)
       {


           text = all_scales [i][1];
           ui->comboBox_change_note_1->setCurrentText(text);

           text.clear();

           text = all_scales [i][2];
           ui->comboBox_change_note_2->setCurrentText(text);
           text.clear();

           text = all_scales [i][3];
           ui->comboBox_change_note_3->setCurrentText(text);
           text.clear();

           text = all_scales [i][4];
           ui->comboBox_change_note_4->setCurrentText(text);
           text.clear();

           text = all_scales [i][5];
           ui->comboBox_change_note_5->setCurrentText(text);
           text.clear();

           text = all_scales [i][6];
           ui->comboBox_change_note_6->setCurrentText(text);
           text.clear();

           text = all_scales [i][7];
           ui->comboBox_change_note_7->setCurrentText(text);
           text.clear();
       }
   }
}





//////////////// u drugi cpp fajl


/*
Nit1::Nit1(Dialog* d,int bn) {

    dia =d;
    but_num = bn;
}


Nit1::~Nit1() {}

    // oboji must emit oboji_finished when it is done.
void Nit1:: oboji(){

    dia->oboji_notu(but_num);
    delay(1000);
    dia->izbledi_note();
    emit(oboji_finished());

}


*/






void Dialog::on_pushButton_flatten_1_pritisnuto()
{

    int new_index = ui->comboBox_change_note_1->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_1->setCurrentIndex(new_index);
}

void Dialog::on_pushButton_flatten_2_pritisnuto()
{

    int new_index = ui->comboBox_change_note_2->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_2->setCurrentIndex(new_index);
}

void Dialog::on_pushButton_flatten_3_pritisnuto()
{

    int new_index = ui->comboBox_change_note_3->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_3->setCurrentIndex(new_index);
}

void Dialog::on_pushButton_flatten_4_pritisnuto()
{

    int new_index = ui->comboBox_change_note_4->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_4->setCurrentIndex(new_index);
}

void Dialog::on_pushButton_flatten_5_pritisnuto()
{

    int new_index = ui->comboBox_change_note_5->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_5->setCurrentIndex(new_index);
}

void Dialog::on_pushButton_flatten_6_pritisnuto()
{

    int new_index = ui->comboBox_change_note_6->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_6->setCurrentIndex(new_index);
}

void Dialog::on_pushButton_flatten_7_pritisnuto()
{

    int new_index = ui->comboBox_change_note_7->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_7->setCurrentIndex(new_index);
}

/*
    if (1)
    {


    int new_index = ui->comboBox_change_note_1->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->comboBox_change_note_1->setCurrentIndex(new_index);
    qDebug()<<"Sharp dugme pritisnuto";


    }
*/

void Dialog::on_pushButton_sharpen_1_pritisnuto()
{

    int new_index = ui->comboBox_change_note_1->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->comboBox_change_note_1->setCurrentIndex(new_index);

}

void Dialog::on_pushButton_sharpen_2_pritisnuto()
{

    int new_index = ui->comboBox_change_note_2->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->comboBox_change_note_2->setCurrentIndex(new_index);

}

void Dialog::on_pushButton_sharpen_3_pritisnuto()
{

    int new_index = ui->comboBox_change_note_3->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->comboBox_change_note_3->setCurrentIndex(new_index);

}

void Dialog::on_pushButton_sharpen_4_pritisnuto()
{

    int new_index = ui->comboBox_change_note_4->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->comboBox_change_note_4->setCurrentIndex(new_index);

}

void Dialog::on_pushButton_sharpen_5_pritisnuto()
{

    int new_index = ui->comboBox_change_note_5->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->comboBox_change_note_5->setCurrentIndex(new_index);

}

void Dialog::on_pushButton_sharpen_6_pritisnuto()
{

    int new_index = ui->comboBox_change_note_6->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->comboBox_change_note_6->setCurrentIndex(new_index);

}

void Dialog::on_pushButton_sharpen_7_pritisnuto()
{

    int new_index = ui->comboBox_change_note_7->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->comboBox_change_note_7->setCurrentIndex(new_index);

}








void Dialog::ocitaj_pot()
{



}



void Dialog::move_vslider (int pot_val)
{

    pot_value = pot_val;

    ui->verticalSlider_1->setEnabled(1);


    ui->verticalSlider_1->setValue(pot_val);

    ui->verticalSlider_1->setDisabled(1);


}







void Dialog::discharge()
{
    pinMode(a_pin,INPUT);
    pinMode(b_pin,OUTPUT);
    digitalWrite(b_pin,0);
    delay(5); // 5ms


}

int Dialog::charge_time()
{
    pinMode(b_pin,INPUT);
    pinMode(a_pin,OUTPUT);
    int count = 0; // counter
    digitalWrite(a_pin,1);
    while (!digitalRead(b_pin))
        count++;
    return count;

}

int Dialog::analogread()
{
    this->discharge();
    return(this->charge_time());
}




void Dialog::sviraj_notu_labele(QComboBox *c)
{
    QString ime_sampla;
    if (sample_izabran==0)  ime_sampla = "grand piano/grand piano  - ";
    else if (sample_izabran==1)  ime_sampla = "modern upright/modern upright - ";
    else if (sample_izabran==2)  ime_sampla = "maple hill funk/maple hill funk - ";
    else if (sample_izabran==3)  ime_sampla = "voodoo magic/voodoo magic - ";
    else if (sample_izabran==4)  ime_sampla = "industrial pad/industrial pad - ";
    else if (sample_izabran==5)  ime_sampla = "current value h/current value h - ";


   // QString ime_sampla ("grand piano/grand piano  - ");  // ovde imati razlicite samplove i efekte
    QString temp (c->currentText());

    if (temp[1]=='b')
        temp = note_names_sharp[c->currentIndex()];

    if (temp[1]=='#')
        temp[1]='s';

    ime_sampla.append(temp);

    QString scale (ui->comboBox_scale->currentText());

    bool bio_u_petlji = 0;
    for (int i=0;i<30;i++)
        if(all_scales[i][0]==scale)
        {
            for (int j =0;j<8;j++)
            {
                if (all_scales[i][j]==c->currentText())
                {
                    if (scale_helper[i][j-1])
                    {
                        octave++;
                        bio_u_petlji = 1;
                    }
                }
            }
        }

    ime_sampla.append(QString::number(octave));  // slajder za oktavu, zavisi od izabrane oktave

    if (bio_u_petlji){
        octave--;
        bio_u_petlji=0;
    }
    ime_sampla.append(".wav");
    qDebug()<<ime_sampla;
    //ime_sampla.prepend("qrc:///samples/");  // resource file - previse velik za sve samplove!
    ime_sampla.prepend("file:///home/pi/Desktop/");   // utice preset samples/effects
    //qDebug()<<"Ime fajla je" << ime_sampla;
    QUrl url_sampla= QUrl(ime_sampla);
    //qDebug()<<url_sampla;
   // url_sampla.toLocalFile();


    if (rec_mode_on) playlist->addMedia(url_sampla); //puni listu sa odg. url-ovima
    else if(!playlist->isEmpty())
    {
         //qDeleteAll(lista.begin(),lista.end()); nije pok na listu pa ne treba
         playlist->clear();
    }


    zvuk->setSource(url_sampla);
   // zvuk->setLoopCount(1);
    zvuk->setVolume(this->jacina_note()); // podaci sa potenciometra 0-1
    qDebug ()<<"Jacina note je "<<this->jacina_note();
    zvuk->setMuted(false);
    //qDebug()<<"Status "<<zvuk->status();
    zvuk->play(); // treba da imam dilej onoliko koliko traje nota!!
   // delay(750);

}


void Dialog::sviraj (int broj_dugmeta)
{

    switch (broj_dugmeta)
    {
        case 1: this->sviraj_notu_labele(ui->comboBox_change_note_1); break;
        case 2: this->sviraj_notu_labele(ui->comboBox_change_note_2); break;
        case 3: this->sviraj_notu_labele(ui->comboBox_change_note_3); break;
        case 4: this->sviraj_notu_labele(ui->comboBox_change_note_4); break;
        case 5: this->sviraj_notu_labele(ui->comboBox_change_note_5); break;
        case 6: this->sviraj_notu_labele(ui->comboBox_change_note_6); break;
        case 7: this->sviraj_notu_labele(ui->comboBox_change_note_7); break;
        case 0: this->sviraj_pauzu(); break; // za rec mode

    }
}




double Dialog:: jacina_note () //izmedju 0 i 1 radi sigurno
{
    int volume = pot_value;
    if (volume <= pot_min) return 0.0;
    else if (volume >= pot_max) return 1.0;
    else {
        return ((double)volume-(double)pot_min)/(double)pot_max;
    }
}

/*
int Dialog:: jacina_note () //izmedju 0 i 100 ne radi
{
    int volume = pot_value;
    if (volume <= pot_min) return 0;
    else if (volume >= pot_max) return 100;
    else {
        return (int)(((double)volume-(double)pot_min)/((double)pot_max))*100;
    }
}
*/
void Dialog::setuj_tekst (int broj)
{
    QString temp = QString::number(broj);
    ui->label_octave->setText(temp);
}

void Dialog::promeni_oktavu (int broj)
{
   // qDebug()<<"Slajder pomeren, vrednost "<<broj;
    octave = broj;
}


void Dialog::upali_diodu(int i)
{
    digitalWrite(led[i],1);
}

void Dialog::ugasi_diodu(int i)
{
    digitalWrite(led[i],0);
}




void Dialog::bira_samplove (int index){

    sample_izabran = index;
}


void Dialog::test_mod (bool pritisnuto){

    if (pritisnuto) {
        ui->pushButton_rec->setChecked(0);
        ui->pushButton_play->setChecked(0); //iskljucivost izmedju tri dugmeta

    test_mode_on = 1;
    rec_mode_on = 0;
    play_mode_on = 0;
        //QMessageBox - za odgovor!

 /*   QMessageBox msgBox (QMessageBox::Question,"Hearing test", "Which note is being played?",QMessageBox::NoButton,this,Qt::Dialog);
    msgBox.addButton(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok); // enter key pressed
    msgBox.addButton(QMessageBox::Cancel);
    msgBox.setEscapeButton(QMessageBox::Cancel); //esc key pressed
    msgBox.setDetailedText("Press the appropriate button to progress");

*/

   // msgBox.exec();

    QDateTime cd = QDateTime::currentDateTime(); //rand seed
    qsrand(cd.toTime_t());
    tacan_odgovor = qrand() % 7 + 1; // od 1 do 7
    this->sviraj(tacan_odgovor); //
    this->upali_diodu(2); //plava
    delay(1000);
    this->ugasi_diodu(2);




}
    else test_mode_on = 0;

}


void Dialog::dugme_pritisnuto(int dugme) //ovde ce test da proverava da li je dobro dugme prit.
{
    //disconnect (this,&Dialog::button_pressed,this,&Dialog::dugme_pritisnuto);

    if (test_mode_on == 1)
    {
        QString string ("file:///home/pi/Desktop/");
        QUrl url;
        QSoundEffect efekat (this);

        if (tacan_odgovor==dugme)
        {
            QMessageBox mb_tacno (QMessageBox::Information,"Congrats!","That is the correct answer",QMessageBox::Close,this,Qt::Dialog);
            mb_tacno.setDefaultButton(QMessageBox::Close);
            mb_tacno.setEscapeButton(QMessageBox::Close);

            string.append("correct.wav");  //sviraj nesto, dioda zelena
            url = QUrl (string);
            efekat.setSource(url);
            efekat.play();

            this->upali_diodu(1);

            mb_tacno.exec();

            this->ugasi_diodu(1);

            test_mode_on = 0;
            ui->pushButton_test->setChecked(0);
            ui->pushButton_play->setChecked(0);

        }
        else if (tacan_odgovor != dugme && dugme!= 0)
        {
            QMessageBox mb_pogr (QMessageBox::Warning,"Too bad!","Better luck next time!",QMessageBox::Close,this,Qt::Dialog);
            mb_pogr.setDefaultButton(QMessageBox::Close);
            mb_pogr.setEscapeButton(QMessageBox::Close);

            string.append("wrong.wav");
            url = QUrl (string);
            efekat.setSource(url);
            efekat.play();

            this->upali_diodu(0);

            mb_pogr.exec();

            this->ugasi_diodu(0);

            test_mode_on = 0;
            ui->pushButton_test->setChecked(0);
            ui->pushButton_play->setChecked(0);
        }

    }

 /*   if (rec_mode_on)
    {



        odsvirano_memory[i_mem] = dugme;
        qDebug()<<"U niz upisano" <<odsvirano_memory[i_mem];

        i_mem++; //treba prebrisati vrednosti kasnije
        if (i_mem==BUFFER)
        {
                rec_mode_on = 0;
                play_mode_on = 1;
                ui->pushButton_rec->setChecked(0);
                ui->pushButton_play->setChecked(1);
                i_mem=0;
        }

    }
*/
    if (play_mode_on)
    {
        /*
       ui->pushButton_rec->setChecked(0);
       ui->pushButton_test->setChecked(0); //iskljucivost vec imam

        if (!playlist->isEmpty()) // ako u memoriji ima makar jedan el
        {
            //QMediaPlayer
            QMediaPlayer player (this,QMediaPlayer::LowLatency);
            player.setAudioRole(QAudio::SonificationRole); //zvuci interfejsa
            player.setPlaybackRate(1); //dodaj unos za bpm!


            playlist->setPlaybackMode(QMediaPlaylist::Sequential); //jedan za drugim se pusta

            //QList <QMediaContent> lista; polje
            //QUrl file_url;
           // QMediaContent content (); u konstruktor ide qurl elemnta koji ubacujemo u playlist


            playlist->setCurrentIndex(1);

            player.setPlaylist(playlist);
            player.setVolume(qRound(jacina_note()*100));

            qDebug()<<player.errorString();

            player.play();



        }

        */


    }

  //  connect (this,&Dialog::button_pressed,this,&Dialog::dugme_pritisnuto);


}


 void Dialog::rec_mod (bool pritisnuto) //kad je rec dugme trigerovano
 {

        if (pritisnuto)
        {
            ui->pushButton_play->setChecked(0);
            ui->pushButton_test->setChecked(0); // iskljucivost



        rec_mode_on = 1;
        test_mode_on = 0;
        play_mode_on = 0;

        }
        else
        {
            rec_mode_on = 0;
            i_mem = 0;
        }


 }


 void Dialog::play_mod (bool pritisnuto)
 {
    if(pritisnuto)
    {
        ui->pushButton_test->setChecked(0);
        ui->pushButton_rec->setChecked(0);

        play_mode_on = 1;
        test_mode_on = 0;
        rec_mode_on = 0;

        if (odsvirano_memory[0]!=-1) //znaci da je nesto snimljeno

        {


            //sviraj memoriju
            int n=1;
            for (int i =0;i<BUFFER;i++)
            {
                if (odsvirano_memory[i]==-1) {n=i; break;}
            }
            if (odsvirano_memory[BUFFER-1]!=-1) n=BUFFER; //pun

            odsvirano_memory_n = n;

            do_on_press(odsvirano_memory[i_play++]);
            delay(100);
           // i_play++;


            {
               //disconnect (this,&Dialog::button_pressed,this,&Dialog::do_on_press); //nema ulaza dok se pusta podrazumeva se
                //treba sacekati da se izvrsi jedan
               //while (!thread_fin);
               //delay(2000);
            }

           // connect (this,&Dialog::button_pressed,this,&Dialog::do_on_press);


        }
        else
        {
            odsvirano_memory_n = 0;
            i_play = 0;
        }


        //play_mode_on = 0;
        //inic_memorije();

    }

    else //nije pritisnuto
    {
        play_mode_on = 0;


        i_play = 0;

    }
 }


 void Dialog::inic_memorije () // delete later
 {
     for (int k=0;k<BUFFER;k++)
     {
         odsvirano_memory[k]=-1; // po pod. vr. puni -1-cama
        // jacine_memory[k]=-1;
     }

 }


 double Dialog::scale_volume (double volumeslider) // ne koristim
 {
     double linear_volume = QAudio::convertVolume(volumeslider/double(1.0),QAudio::LogarithmicVolumeScale,QAudio::LinearVolumeScale);
     return linear_volume;
 }


  void Dialog::thread_done ( )
{
    thread_fin = 1;


    if (ui->pushButton_play->isChecked())
    {
       // qDebug()<<"Izvrsio se thread_done";
        if (i_play<odsvirano_memory_n)
        {
            do_on_press(odsvirano_memory[i_play++]);
            delay(100);
        }
        if (i_play==odsvirano_memory_n)
        {
            i_play = 0;
            //inic_memorije ();
            ui->pushButton_play->setChecked(0);
        }



    }

 }

  void Dialog::pisi_u_mem (bool recording, int but)
  {
      if (recording)
      {




              odsvirano_memory[i_mem] = but;
              qDebug()<<"U niz upisano" <<odsvirano_memory[i_mem];

              i_mem++; //treba prebrisati vrednosti kasnije
              if (i_mem==BUFFER)
              {
                      rec_mode_on = 0;
                      play_mode_on = 1;
                      ui->pushButton_rec->setChecked(0);
                      ui->pushButton_play->setChecked(1);
                      i_mem=0;
              }


      }

  }

  void Dialog::sviraj_pauzu()
  {

            QString ime_sampla ("file:///home/pi/Desktop/silent_note.wav");
            QUrl url_sampla= QUrl(ime_sampla);
            zvuk->setSource(url_sampla);
            zvuk->setMuted(false);
            //qDebug()<<"Status "<<zvuk->status();
            zvuk->play();
            delay(50);


  }
