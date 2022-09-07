/********************************************************************************
** Form generated from reading UI file 'ncstting.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NCSTTING_H
#define UI_NCSTTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NCstting
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QComboBox *comboBox_daobu;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *comboBox;

    void setupUi(QWidget *NCstting)
    {
        if (NCstting->objectName().isEmpty())
            NCstting->setObjectName(QString::fromUtf8("NCstting"));
        NCstting->resize(329, 213);
        pushButton = new QPushButton(NCstting);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(50, 180, 93, 28));
        pushButton_2 = new QPushButton(NCstting);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(180, 180, 93, 28));
        layoutWidget = new QWidget(NCstting);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 10, 239, 23));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        layoutWidget1 = new QWidget(NCstting);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(50, 60, 239, 23));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEdit_2 = new QLineEdit(layoutWidget1);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_3->addWidget(lineEdit_2);

        layoutWidget2 = new QWidget(NCstting);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(50, 100, 191, 23));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        comboBox_daobu = new QComboBox(layoutWidget2);
        comboBox_daobu->addItem(QString());
        comboBox_daobu->addItem(QString());
        comboBox_daobu->setObjectName(QString::fromUtf8("comboBox_daobu"));

        horizontalLayout->addWidget(comboBox_daobu);

        widget = new QWidget(NCstting);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(50, 140, 191, 23));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        comboBox = new QComboBox(widget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_4->addWidget(comboBox);


        retranslateUi(NCstting);

        QMetaObject::connectSlotsByName(NCstting);
    } // setupUi

    void retranslateUi(QWidget *NCstting)
    {
        NCstting->setWindowTitle(QCoreApplication::translate("NCstting", "\346\225\260\346\216\247\346\234\272\345\272\212\345\217\202\346\225\260\351\200\211\346\213\251", nullptr));
        pushButton->setText(QCoreApplication::translate("NCstting", "\344\277\235\345\255\230", nullptr));
        pushButton_2->setText(QCoreApplication::translate("NCstting", "\351\273\230\350\256\244\345\217\202\346\225\260", nullptr));
        label->setText(QCoreApplication::translate("NCstting", "\344\270\273\350\275\264\350\275\254\351\200\237    ", nullptr));
        label_2->setText(QCoreApplication::translate("NCstting", "\350\277\233\347\273\231\351\200\237\345\272\246    ", nullptr));
        label_4->setText(QCoreApplication::translate("NCstting", "\345\210\200\350\241\245\347\261\273\345\236\213", nullptr));
        comboBox_daobu->setItemText(0, QCoreApplication::translate("NCstting", "\345\267\246\345\210\200\350\241\245", nullptr));
        comboBox_daobu->setItemText(1, QCoreApplication::translate("NCstting", "\345\217\263\345\210\200\350\241\245", nullptr));

        label_3->setText(QCoreApplication::translate("NCstting", "\345\210\200\350\241\245\345\217\267", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("NCstting", "D01", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("NCstting", "D02", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("NCstting", "D11", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("NCstting", "D12", nullptr));

    } // retranslateUi

};

namespace Ui {
    class NCstting: public Ui_NCstting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NCSTTING_H
