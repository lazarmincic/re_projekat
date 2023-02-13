#include "dialog.h"
#include "ui_dialog.h"



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QMetaObject::connectSlotsByName(this);
//  connect
    QTimer *timer = new QTimer(this);
    connect (timer,&QTimer::timeout,this, &Dialog::detect_press); // kad tajmer odbroji d_p se izvrsava
    timer->start(REF_RATE);

    detect_press();


    connect (this,&Dialog::button_pressed,this,&Dialog::do_on_press); // kad je dugme prit. izvrsava se d_o_p



    //mod = monitor; // pod. vrednost
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

    qDebug()<<"Pritisnuto dugme broj "<< but_num;
    this->oboji_notu(but_num);

}


void Dialog::initialize_gpio()
{
    for (int i = 0;i<8;i++)
        pinMode (button[i],INPUT);
    for (int i = 0;i<3;i++)
        pinMode (led[i], OUTPUT);


}



void Dialog::oboji_notu(int but_num)
{
   // QString temp = ;
    switch(but_num)
    {
                    case 0: break;
                  //  case 1: this->promeni_sliku_labele(ui->label_1,); break;
                    case 2:
                    default: ;
    }


}

void Dialog::promeni_sliku_labele(QLabel *l,QString s) // ul parametar qstring
{
    QString temp = ":/media/";
    temp.append(s);
    temp.append("_off");
    QPixmap slika (temp);
    l->setPixmap(slika);
    //delay(2000); nikad!!
  //  QThread wait_thread (this); // dilej funkcija
 //   wait_thread.start();
 //   wait_thread.msleep(1000);
 //   wait_thread.quit();
 //   qDebug()<<"Izvrsio se dilej ";
 //    slika.load (":media/C_on.png");
//    l->setPixmap(slika);
 //   qDebug()<<"Izvrsio se setPixmap ";

}




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
void Dialog::on_comboBox_change_note_1_activated(const QString &arg1)
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

void initialize_notes(QComboBox *c, QLabel* l){  // na osnovu imena note u padajucem meniju treba promeniti sliku u

    int i;
    c->setCurrentIndex(i); // pomera na indeks i

}




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









void Dialog::on_comboBox_scale_activated(const QString &arg1)
{

}


void Dialog::on_comboBox_scale_currentTextChanged(const QString &arg1)  // pritisnuta opcija u padajucem meniju scale treba da ucita u slike i padajuce menije nove note
{
    if (A_Minor [0] == arg1)
    {
        QString text;

        text = A_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = A_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = A_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = A_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = A_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = A_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = A_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (D_Minor [0] == arg1)
    {
        QString text;

        text = D_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = D_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = D_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = D_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = D_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = D_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = D_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }


    if (G_Minor [0] == arg1)
    {
        QString text;

        text = G_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = G_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = G_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = G_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = G_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = G_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = G_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (C_Minor [0] == arg1)
    {
        QString text;

        text = C_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = C_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = C_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = C_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = C_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = C_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = C_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (F_Minor [0] == arg1)
    {
        QString text;

        text = F_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = F_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = F_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = F_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = F_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = F_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = F_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (Ab_Minor [0] == arg1)
    {
        QString text;

        text = Ab_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = Ab_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = Ab_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = Ab_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = Ab_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = Ab_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = Ab_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (Eb_Minor [0] == arg1)
    {
        QString text;

        text = Eb_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = Eb_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = Eb_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = Eb_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = Eb_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = Eb_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = Eb_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (Bb_Minor [0] == arg1)
    {
        QString text;

        text = Bb_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = Bb_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = Bb_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = Bb_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = Bb_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = Bb_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = Bb_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (E_Minor [0] == arg1)
    {
        QString text;

        text = E_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = E_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = E_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = E_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = E_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = E_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = E_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }


    if (B_Minor [0] == arg1)
    {
        QString text;

        text = B_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = B_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = B_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = B_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = B_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = B_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = B_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }


    if (Fs_Minor [0] == arg1)
    {
        QString text;

        text = Fs_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = Fs_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = Fs_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = Fs_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = Fs_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = Fs_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = Fs_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }


    if (Cs_Minor [0] == arg1)
    {
        QString text;

        text = Cs_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = Cs_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = Cs_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = Cs_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = Cs_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = Cs_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = Cs_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }


    if (Gs_Minor [0] == arg1)
    {
        QString text;

        text = Gs_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = Gs_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = Gs_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = Gs_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = Gs_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = Gs_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = Gs_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }


    if (Ds_Minor [0] == arg1)
    {
        QString text;

        text = Ds_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = Ds_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = Ds_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = Ds_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = Ds_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = Ds_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = Ds_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

    if (As_Minor [0] == arg1)
    {
        QString text;

        text = As_Minor [1];
        ui->comboBox_change_note_1->setCurrentText(text);
        text.clear();

        text = As_Minor [2];
        ui->comboBox_change_note_2->setCurrentText(text);
        text.clear();

        text = As_Minor [3];
        ui->comboBox_change_note_3->setCurrentText(text);
        text.clear();

        text = As_Minor [4];
        ui->comboBox_change_note_4->setCurrentText(text);
        text.clear();

        text = As_Minor [5];
        ui->comboBox_change_note_5->setCurrentText(text);
        text.clear();

        text = As_Minor [6];
        ui->comboBox_change_note_6->setCurrentText(text);
        text.clear();

        text = As_Minor [7];
        ui->comboBox_change_note_7->setCurrentText(text);
        text.clear();
    }

}
