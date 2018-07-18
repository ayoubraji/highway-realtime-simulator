/********************************************************************************
** Form generated from reading UI file 'customchoicedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMCHOICEDIALOG_H
#define UI_CUSTOMCHOICEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_customChoiceDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QSpinBox *vehicles_number;
    QLabel *label_2;
    QSpinBox *trucks_perc;
    QLabel *label_3;
    QSpinBox *cars_perc;
    QLabel *label_4;
    QSpinBox *motorcycles_perc;
    QLabel *label_5;
    QTextEdit *vehicle_to_track;
    QLabel *label_6;
    QRadioButton *rare_frequency;
    QRadioButton *frequent_frequency;
    QLabel *label_7;
    QLabel *label_8;

    void setupUi(QDialog *customChoiceDialog)
    {
        if (customChoiceDialog->objectName().isEmpty())
            customChoiceDialog->setObjectName(QStringLiteral("customChoiceDialog"));
        customChoiceDialog->resize(698, 286);
        buttonBox = new QDialogButtonBox(customChoiceDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(530, 230, 161, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(customChoiceDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 50, 171, 21));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        vehicles_number = new QSpinBox(customChoiceDialog);
        vehicles_number->setObjectName(QStringLiteral("vehicles_number"));
        vehicles_number->setGeometry(QRect(220, 40, 51, 31));
        QFont font1;
        font1.setPointSize(9);
        vehicles_number->setFont(font1);
        vehicles_number->setMaximum(500);
        vehicles_number->setValue(50);
        label_2 = new QLabel(customChoiceDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(110, 100, 91, 21));
        label_2->setFont(font);
        trucks_perc = new QSpinBox(customChoiceDialog);
        trucks_perc->setObjectName(QStringLiteral("trucks_perc"));
        trucks_perc->setGeometry(QRect(220, 90, 51, 31));
        trucks_perc->setFont(font1);
        trucks_perc->setMaximum(100);
        trucks_perc->setSingleStep(10);
        trucks_perc->setValue(30);
        label_3 = new QLabel(customChoiceDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(110, 150, 91, 21));
        label_3->setFont(font);
        cars_perc = new QSpinBox(customChoiceDialog);
        cars_perc->setObjectName(QStringLiteral("cars_perc"));
        cars_perc->setGeometry(QRect(220, 140, 51, 31));
        cars_perc->setFont(font1);
        cars_perc->setMaximum(100);
        cars_perc->setSingleStep(10);
        cars_perc->setValue(30);
        label_4 = new QLabel(customChoiceDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(110, 200, 91, 21));
        label_4->setFont(font);
        motorcycles_perc = new QSpinBox(customChoiceDialog);
        motorcycles_perc->setObjectName(QStringLiteral("motorcycles_perc"));
        motorcycles_perc->setGeometry(QRect(220, 190, 51, 31));
        motorcycles_perc->setFont(font1);
        motorcycles_perc->setMaximum(100);
        motorcycles_perc->setSingleStep(10);
        motorcycles_perc->setValue(40);
        label_5 = new QLabel(customChoiceDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(400, 40, 171, 21));
        label_5->setFont(font);
        vehicle_to_track = new QTextEdit(customChoiceDialog);
        vehicle_to_track->setObjectName(QStringLiteral("vehicle_to_track"));
        vehicle_to_track->setGeometry(QRect(540, 40, 41, 31));
        label_6 = new QLabel(customChoiceDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(400, 100, 271, 21));
        label_6->setFont(font);
        rare_frequency = new QRadioButton(customChoiceDialog);
        rare_frequency->setObjectName(QStringLiteral("rare_frequency"));
        rare_frequency->setGeometry(QRect(400, 140, 100, 20));
        rare_frequency->setFont(font);
        frequent_frequency = new QRadioButton(customChoiceDialog);
        frequent_frequency->setObjectName(QStringLiteral("frequent_frequency"));
        frequent_frequency->setGeometry(QRect(560, 140, 100, 20));
        frequent_frequency->setFont(font);
        label_7 = new QLabel(customChoiceDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 10, 661, 21));
        QFont font2;
        font2.setPointSize(13);
        label_7->setFont(font2);
        label_8 = new QLabel(customChoiceDialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(590, 40, 61, 21));
        label_8->setFont(font);

        retranslateUi(customChoiceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), customChoiceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), customChoiceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(customChoiceDialog);
    } // setupUi

    void retranslateUi(QDialog *customChoiceDialog)
    {
        customChoiceDialog->setWindowTitle(QApplication::translate("customChoiceDialog", "Dialog", 0));
        label->setText(QApplication::translate("customChoiceDialog", "Number of Vehicles :", 0));
        label_2->setText(QApplication::translate("customChoiceDialog", "Truck % :", 0));
        label_3->setText(QApplication::translate("customChoiceDialog", "Cars % :", 0));
        label_4->setText(QApplication::translate("customChoiceDialog", "MotorCycle % :", 0));
        label_5->setText(QApplication::translate("customChoiceDialog", "Vehicle to track:", 0));
        label_6->setText(QApplication::translate("customChoiceDialog", "Frequency of lateral movements:", 0));
        rare_frequency->setText(QApplication::translate("customChoiceDialog", "Rare", 0));
        frequent_frequency->setText(QApplication::translate("customChoiceDialog", "Frequent", 0));
        label_7->setText(QApplication::translate("customChoiceDialog", "IMPORTANT: There is no validation... So should be chosen reasonable values", 0));
        label_8->setText(QApplication::translate("customChoiceDialog", "[0, N-1]", 0));
    } // retranslateUi

};

namespace Ui {
    class customChoiceDialog: public Ui_customChoiceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMCHOICEDIALOG_H
