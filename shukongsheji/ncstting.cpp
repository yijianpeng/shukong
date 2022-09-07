#include "ncstting.h"
#include "ui_ncstting.h"
#include "mainwindow.h"
#include <QMessageBox>


NCstting::NCstting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NCstting)
{
    ui->setupUi(this);
    ui->lineEdit->setText(QString::number(Spindle_speed));
    ui->lineEdit_2->setText(QString::number(Feed_rate));
    connect(ui->pushButton, &QPushButton::clicked, this, &NCstting::onMessageBox1);
    connect(ui->comboBox_daobu, SIGNAL(currentIndexChanged(int)), this, SLOT(daobusetting()));

    if(daobu==1)
    {
       ui->comboBox_daobu->setCurrentIndex(0);
    }
    else if(daobu==2){
       ui->comboBox_daobu->setCurrentIndex(1);
    }
}

NCstting::~NCstting()
{
    delete ui;
}



//保存按钮按下
void NCstting::on_pushButton_clicked()
{
   QString str1=ui->lineEdit->text();
   Spindle_speed=str1.toInt();
   QString str2=ui->lineEdit_2->text();
   Feed_rate=str2.toInt();

}

//还原默认参数
void NCstting::on_pushButton_2_clicked()
{
    Spindle_speed=12000;
    Feed_rate=800;
    ui->lineEdit->setText(QString::number(Spindle_speed));
    ui->lineEdit_2->setText(QString::number(Feed_rate));
}

//参数输入错误提示窗口
void NCstting::onMessageBox1()
{
        //含有为0的提示输入完整参数，都不为0则判断角度是否正确
       if (Spindle_speed==0||Feed_rate==0)
       {
           QMessageBox::critical(this, "错误", "速度不能为0！！",
                       QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       }
       else{
          int r= QMessageBox::question(this, "保存", "是否保存机床参数",
                       QMessageBox::Save | QMessageBox::No, QMessageBox::Save);
          if(r==QMessageBox::Save){
              close();
          }
       }
}

void NCstting:: daobusetting()
{
    if(ui->comboBox_daobu->currentIndex()== 0)
    {
        daobu=1;//左刀补
        qDebug("1");
    }
    else {
        daobu=2;//右刀补
         qDebug("2");
    }
}
