#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QMessageBox>
#include "ncstting.h"
#include<QtGui>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//槽函数
private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_3_clicked();

    void paintEvent(QPaintEvent *event);

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void onMessageBox();

    void Setting_CNCmachine();//数控机床的参数设置槽函数

    void NCexport();

    void SVGexport();

    void on_pushButton_5_clicked();

    void on_pushButton_SVG_clicked();

private:
    Ui::MainWindow *ui;
    QImage image; //定义一个类
};

extern int Spindle_speed;
extern int Feed_rate;
extern int huitu;
extern int daobu;
#endif // MAINWINDOW_H
