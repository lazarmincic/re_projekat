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
        if (digitalRead(button[i]))
            emit (button_pressed(i)); // 0 pauza 1-7 dugme
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


    QThread* thread = new QThread( ); // napravi novi tred da bi mogao da obojis labelu
    Nit1* task = new Nit1(this,but_num);  //color when pressed in another thread
    task->moveToThread(thread);

    Dialog::connect(thread, &QThread::started, task, &Nit1::oboji);
    connect(task, &Nit1::oboji_finished, thread, &QThread::quit);
    connect(task, &Nit1::oboji_finished, task, &Nit1::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();


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
    qDebug()<<"Izvrsio se setPixmap ";

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
}

void Dialog::puni_p_meni_major( ) //slot
{
    QComboBox *c = new QComboBox ();
    c = ui->comboBox_scale;
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

    this->pot_value = pot_val;

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
    QString ime_sampla ("projekat - ");
    ime_sampla.append(c->currentText());
    ime_sampla.append("3");  // slajder za oktavu
    ime_sampla.append(".wav");
    ime_sampla.prepend("qrc:///samples/");
    //qDebug()<<"Ime fajla je" << ime_sampla;
    QUrl url_sampla= QUrl(ime_sampla);
    //qDebug()<<url_sampla;
   // url_sampla = url_sampla.toLocalFile();
    zvuk->setSource(url_sampla);
   // zvuk->setLoopCount(1);
    zvuk->setVolume(1); // podaci sa potenciometra 0-100
    qDebug ()<<"Jacina note je "<<this->jacina_note();
    zvuk->setMuted(false);
    //qDebug()<<"Status "<<zvuk->status();
    zvuk->play(); // treba da imam dilej onoliko koliko traje nota!!
  //  delay(750);

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


    }
}

void Dialog::sviraj_frekv (double freq, int sec)
{
/*


    QByteArray* bytebuf = new QByteArray();
    bytebuf->resize(sec * SAMPLE_RATE);
    for (int i=0; i<(sec * SAMPLE_RATE); i++)
    {
        qreal t = (qreal)(freq * i);
        t = t * FREQ_CONST;
        t = qSin(t);
        t *= SPD_MAX_VAL;
        (*bytebuf)[i] = (quint8)t;
    }

    QBuffer* input = new QBuffer(bytebuf);
    input->open(QIODevice::ReadOnly);

    QAudioFormat format;
    format.setSampleRate(SAMPLE_RATE);
    format.setChannelCount(1);
    format.setSampleSize(SPD_SAMPLE_SIZE);
    format.setCodec(SPD_CODEC);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioOutput* audio = new QAudioOutput(format, this);
    audio->start(input);


*/


}


double Dialog:: jacina_note ()
{
    int volume = pot_value;
    if (volume <= pot_min) return 0.0;
    else if (volume >= pot_max) return 1.0;
    else {
        return ((double)volume-(double)pot_min)/(double)pot_max;
    }
}
