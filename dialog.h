#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "wiringPi.h"
#include <QPixmap>
#include <QThread>
#include <QMetaObject>
#include<QMessageBox>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QComboBox>

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

enum mode {monitor, record, test};
enum note_mode {sharp, flat};



class Dialog : public QDialog
{
    Q_OBJECT



public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    char get_button (int i) const;
    char get_led(int i) const;

    void initialize_gpio();
    void oboji_notu(int but_num);
    void promeni_sliku_labele(QLabel* l,QString s);

    void choose_note(QComboBox* c );
    void initialize_note(QComboBox *c, QLabel* l);


private slots:

    void detect_press(); // petlja za detektovanje pritiska
    void do_on_press(int but_num);  //izv pri pritisku

    void on_checkBox_on_off_stateChanged(int arg1); //ukljuci-iskljuci ulaz

    void on_radioButton_sharp_clicked(bool checked);

    void on_radioButton_flat_clicked(bool checked);

    void on_comboBox_change_note_1_activated(const QString &arg1);
    void on_comboBox_change_note_2_activated(const QString &arg1);
    void on_comboBox_change_note_3_activated(const QString &arg1);
    void on_comboBox_change_note_4_activated(const QString &arg1);
    void on_comboBox_change_note_5_activated(const QString &arg1);
    void on_comboBox_change_note_6_activated(const QString &arg1);
    void on_comboBox_change_note_7_activated(const QString &arg1);

    void on_pushButton_minor_clicked(bool checked);


    void puni_p_meni_minor();

    void puni_p_meni_major();

    void on_pushButton_major_clicked(bool checked);

    void on_comboBox_scale_activated(const QString &arg1);

    void on_comboBox_change_note_1_currentIndexChanged(const QString &arg1);

    void on_comboBox_change_note_1_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_2_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_3_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_4_editTextChanged(const QString &arg1);

    void on_comboBox_change_note_5_editTextChanged(const QString &arg1);

    void on_comboBox_change_note_6_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_4_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_5_currentTextChanged(const QString &arg1);

    void on_comboBox_change_note_7_currentTextChanged(const QString &arg1);

    void on_comboBox_scale_currentTextChanged(const QString &arg1);

signals:
       void button_pressed(int broj_dugme); // kad je detektovan odasilje se ovaj signal
       void minor (); // kad je pritisnuto dugme minor
       void major();

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

    const char A_Minor [8][9] = {"A Minor","A","B","C","D","E","F","G"}; // flat, sharp

    const char D_Minor [8][9] = {"D Minor","D","E","F","G","A","Bb","C"}; //flat
    const char G_Minor [8][9] = {"G Minor","G","A","Bb","C","D","Eb","F"};
    const char C_Minor [8][9] = {"C Minor","C","D","Eb","F","G","Ab","Bb"};
    const char F_Minor [8][9] = {"F Minor","F","G","Ab","Bb","C","Db","Eb"};
    const char Ab_Minor [8][9] = {"Ab Minor","Ab","Bb","B","Db","Eb","E","Gb"};
    const char Eb_Minor [8][9] = {"Eb Minor","Eb","F","Gb","Ab","Bb","B","Db"};
    const char Bb_Minor [8][9] = {"Bb Minor","Bb","C","Db","Eb","F","Gb","Ab"};

    const char E_Minor [8][9] = {"E Minor","E","F#","G","A","B","C","D"}; //sharp
    const char B_Minor [8][9] = {"B Minor","B","C#","D","E","F#","G","A"};
    const char Fs_Minor [8][9] = {"F# Minor","F#","G#","A","B","C#","D","E"};
    const char Cs_Minor [8][9] = {"C# Minor","C#","D#","E","F#","G#","A","B"};
    const char Gs_Minor [8][9] = {"G# Minor","G#","A#","B","C#","D#","E","F#"};
    const char Ds_Minor [8][9] = {"D# Minor","D#","F","F#","G#","A#","B","C#"};
    const char As_Minor [8][9] = {"A# Minor","A#","C","C#","D#","F","F#","G#"};



    mode mod = monitor;
    note_mode sharp_or_flat = sharp;



//   QPixmap *ui_qpixmap;
//   QThread *ui_qthread;
//   QMetaObject *ui_qmetaobject;

};





#endif // DIALOG_H
