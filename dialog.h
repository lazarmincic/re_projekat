#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "wiringPi.h"
#include <QPixmap>
#include <QThread>
//#include <QMetaObject>
#include<QMessageBox>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QComboBox>
#include <QGraphicsScene>
#include <QProgressBar>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>


#include <iostream>
using namespace std;

#define REF_RATE 100 // in ms



namespace Ui {
class Dialog;
//class QPixmap;
//class QThread;
//class QMetaObject;
}

//QT_BEGIN_NAMESPACE

//QT_END_NAMESPACE

enum mode {moNit1or, record, test};
enum note_mode {sharp, flat};



class Dialog : public QDialog
{
    Q_OBJECT



public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    char get_button (int i) const;
    char get_led(int i) const;

    void iNit1ialize_gpio();
    void oboji_notu(int but_num);
    void promeni_sliku_labele(QLabel* l,QString s);

    void choose_note(QComboBox* c );
    //void thread_delay(int ms);

    void izbledi_note();

    int ocitaj_pot(); // pot

private slots:

    void detect_press(); // petlja za detektovanje pritiska
    void do_on_press(int but_num);  //izv pri pritisku

    void on_checkBox_on_off_stateChanged(int arg1); //ukljuci-iskljuci ulaz

    void on_radioButton_sharp_clicked(bool checked);

    void on_radioButton_flat_clicked(bool checked);

    void on_comboBox_change_note_1_activated(const QString &arg1); // remove this later
    void on_comboBox_change_note_2_activated(const QString &arg1);
    void on_comboBox_change_note_3_activated(const QString &arg1);
    void on_comboBox_change_note_4_activated(const QString &arg1);
    void on_comboBox_change_note_5_activated(const QString &arg1);
    void on_comboBox_change_note_6_activated(const QString &arg1);
    void on_comboBox_change_note_7_activated(const QString &arg1); // until this all obsolete

    void on_pushButton_minor_clicked(bool checked);


    void puni_p_meni_minor();

    void puni_p_meni_major();

    void on_pushButton_major_clicked(bool checked);





    void on_comboBox_change_note_1_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_2_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_3_currentTextChanged(const QString &arg1);



    void on_comboBox_change_note_6_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_4_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_5_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_7_currentTextChanged(const QString &arg1);

    void on_comboBox_scale_currentTextChanged(const QString &arg1);

    void oboji_sliku_labele(QLabel *l,QString s);

    void on_pushButton_flatten_1_pritisnuto();
    void on_pushButton_flatten_2_pritisnuto();
    void on_pushButton_flatten_3_pritisnuto();
    void on_pushButton_flatten_4_pritisnuto();
    void on_pushButton_flatten_5_pritisnuto();
    void on_pushButton_flatten_6_pritisnuto();
    void on_pushButton_flatten_7_pritisnuto();


    void on_pushButton_sharpen_1_pritisnuto();
    void on_pushButton_sharpen_2_pritisnuto();
    void on_pushButton_sharpen_3_pritisnuto();
    void on_pushButton_sharpen_4_pritisnuto();
    void on_pushButton_sharpen_5_pritisnuto();
    void on_pushButton_sharpen_6_pritisnuto();
    void on_pushButton_sharpen_7_pritisnuto();



    void move_vslider (int pot_val); //pot





signals:
       void button_pressed(int broj_dugme); // kad je detektovan odasilje se ovaj signal
       void minor (); // kad je pritisnuto dugme minor
       void major();

       void pot_value_changed (int pot_value); //pot

private:
    Ui::Dialog *ui;

protected:
    const char button [8] = {22,7,0,4,2,5,29,28};       // wiring pi numbers, starts from 1, 0 is pause note
    const char led [3] = {25,24,23}; // crvena, zelena, plava
    const char note_names_sharp [12][3] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"}; // imena nota sarp mod
    const char note_names_flat [12][3] = {"C","Db","D","Eb","E","F","Gb","G","Ab","A","Bb","B"}; // imena nota flat mod
    const char major_scale_names_sharp [8][9] = {"C Major","D Major","E Major","G Major","A Major","B Major","F# Major","C# Major"}; // imena skala sharp mod
    const char major_scale_names_flat [8][9] = {"C Major","F Major","Bb Major","Eb Major","Ab Major","Cb Major","Gb Major","Db Major"}; // imena skala flat mod
    const char minor_scale_names_sharp [8][9] = {"A Minor","E Minor","B Minor","F# Minor","C# Minor","G# Minor","D# Minor","A# Minor"}; //////////////
    const char minor_scale_names_flat [8][9] = {"A Minor", "D Minor","G Minor","C Minor", "F Minor","Ab Minor","Eb Minor","Bb Minor"};


    const char all_scales [30][8][9] = {{"A Minor","A","B","C","D","E","F","G"},{"D Minor","D","E","F","G","A","Bb","C"},{"G Minor","G","A","Bb","C","D","Eb","F"},         // 30 skala ukupno, 15 major 15 minor
                                              {"C Minor","C","D","Eb","F","G","Ab","Bb"},{"F Minor","F","G","Ab","Bb","C","Db","Eb"},{"Ab Minor","Ab","Bb","B","Db","Eb","E","Gb"},
                                               {"Eb Minor","Eb","F","Gb","Ab","Bb","B","Db"},{"Bb Minor","Bb","C","Db","Eb","F","Gb","Ab"},{"E Minor","E","F#","G","A","B","C","D"},
                                               {"B Minor","B","C#","D","E","F#","G","A"},{"F# Minor","F#","G#","A","B","C#","D","E"},{"C# Minor","C#","D#","E","F#","G#","A","B"},
                                               {"G# Minor","G#","A#","B","C#","D#","E","F#"},{"D# Minor","D#","F","F#","G#","A#","B","C#"},{"A# Minor","A#","C","C#","D#","F","F#","G#"},

                                          {"C Major","C","D","E","F","G","A","B"},{"F Major","F","G","A","Bb","C","D","E"},{"Bb Major","Bb","C","D","Eb","F","G","A"},
                                            {"Eb Major","Eb","F","G","Ab","Bb","C","D"},{"Ab Major","Ab","Bb","C","Db","Eb","F","G"},{"Cb Major","B","Db","Eb","E","Gb","Ab","Bb"},
                                        {"Gb Major","Gb","Ab","Bb","B","Db","Eb","F"},{"Db Major","Db","Eb","F","Gb","Ab","Bb","C"},    {"G Major","G","A","B","C","D","E","F#"},
                                        {"D Major","D","E","F#","G","A","B","C#"},{"A Major","A","B","C#","D","E","F#","G#"},{"E Major","E","F#","G#","A","B","C#","D#"},
                                        {"B Major","B","C#","D#","E","F#","G#","A#"},{"F# Major","F#","G#","A#","B","C#","D#","F"},{"C# Major","C#","D#","F","F#","G#","A#","C"}
                                              };

    mode mod = moNit1or;
    note_mode sharp_or_flat = sharp;



//   QPixmap *ui_qpixmap;
//   QThread *ui_qthread;
//   QMetaObject *ui_qmetaobject;

};

//////////// u drugi h fajl

class Nit1 : public QObject  //boji
{
    Q_OBJECT
public:
    Nit1(Dialog *d,int bn);
    ~Nit1();
public slots:
    // oboji must emit oboji_finished when it is done.
    void oboji();
    void citaj_pot();

signals:
    void oboji_finished();
    void citaj_pot_finished();

private:
    Dialog* dia;
    int but_num;
};


class Nit2 : public QObject  //ocitava potenciometar
{
    Q_OBJECT
public:
    Nit2(QDialog *q);
    ~Nit2();
public slots:
    //
    void doo();

    void discharge();
    int charge_time();
    int analogread(); // umesto int qtimer


signals:
    void doo_finished();

private:
    //var
    QDialog* qd;
    const int a_pin = 1; //wiringpi
    const int b_pin = 27;
};









#endif // DIALOG_H
