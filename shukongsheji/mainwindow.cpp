#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "qdebug.h"
#include <QPaintEvent>
#include <QPen>
#include <QPainter>
#include <QMessageBox>
#include "ncstting.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QInputDialog>
#include <QtSvg/QSvgGenerator>
#include <QRadioButton>
#include <QClipboard>
#include <QApplication>

#define pi 2*3.1415926/360

int tuigang=0;//推杆类型选择
//各个参数全局变量的定义
int sport_angle=0,far_angle=0,hsport_angle=0,
                    near_angle=0,yradius=0,offset=0,gradius=0,journey=0,s0=0;

float kx1[360],ky1[360];
//推程和回程变量选择
int tui=0,hui=0;
//绘图
int huitu=0,huabu=0;
QPointF points[360];  // 绘图数组
QPointF points2[360];

//数控机床参数设置
int Spindle_speed=12000,Feed_rate=800,daobu=1,houdu=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/tulun.png"));//软件图标

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onMessageBox);
    connect(ui->actionNC,SIGNAL(triggered(bool)),this,SLOT(Setting_CNCmachine()));
    connect(ui->action_export,SIGNAL(triggered(bool)),this,SLOT(NCexport()));
    connect(ui->action_SVG,SIGNAL(triggered(bool)),this,SLOT(SVGexport()));
    connect(ui->radioButton,&QRadioButton::clicked,this,[=]{
        tuigang=0;
        qDebug("%d",tuigang);
    });
    connect(ui->radioButton_2,&QRadioButton::clicked,this,[=]{
        tuigang=1;
        qDebug("%d",tuigang);
    });
    ui->radioButton->setChecked(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//按下保存按钮获取各个参数
void MainWindow::on_pushButton_clicked()
{
    /*****数据读取*****/
    //推程运动角
    QString str1 =ui->tsport_angle->text();
    sport_angle =str1.toInt();
    //远休止角
    QString str2 =ui->far_angle->text();
    far_angle =str2.toInt();
    //回程运动角
    QString str3 =ui->hsort_angle->text();
    hsport_angle=str3.toInt();
    //近休止角
    QString str4=ui->near_angle->text();
    near_angle=str4.toInt();
    //基圆半径
    QString str5=ui->yradius->text();
    yradius=str5.toInt();
    //偏距
    QString str6=ui->offset->text();
    offset =str6.toInt();
    //滚子半径
    QString str7=ui->gradius->text();
    gradius=str7.toInt();
    //行程
    QString str8=ui->journey->text();
    journey =str8.toInt();
    qDebug() << "成功写入参数";
    QMessageBox::information(this, tr("成功"), tr("成功保存凸轮参数!"));
}


//推程算法选择(可以不用写的)直接在计算那获取索引值
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << "%d";
    if( ui->comboBox->currentIndex() == 0)
    {
        tui=0;
    }
    else if( ui->comboBox->currentIndex()==1)
    {
        tui=1;
    }
    else if( ui->comboBox->currentIndex()==2){
        tui=2;
    }
    else if( ui->comboBox->currentIndex()==3){
        tui=3;
    }
    else if( ui->comboBox->currentIndex()==4)
    {
        tui=4;
    }
}


//回程运动规律（可以不用写的）直接在计算那获取索引值
void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
     qDebug("%d",tui);
    if(ui->comboBox_2->currentIndex()== 0)
    {
        hui=0;
    }
    else if(ui->comboBox_2->currentIndex()==1)
    {
        hui=1;
    }
    else if(ui->comboBox_2->currentIndex()==2){
        hui=2;
    }
    else if(ui->comboBox_2->currentIndex()==3){
        hui=3;
    }
    else if(ui->comboBox_2->currentIndex()==4)
    {
        hui=4;
    }
}

//计算各个参数并生成轮廓曲线
void MainWindow::on_pushButton_3_clicked()
{
    /*****数据读取*****/
    //推程运动角
    QString str1 =ui->tsport_angle->text();
    sport_angle =str1.toInt();
    //远休止角
    QString str2 =ui->far_angle->text();
    far_angle =str2.toInt();
    //回程运动角
    QString str3 =ui->hsort_angle->text();
    hsport_angle=str3.toInt();
    //近休止角
    QString str4=ui->near_angle->text();
    near_angle=str4.toInt();
    //基圆半径
    QString str5=ui->yradius->text();
    yradius=str5.toInt();
    //偏距
    QString str6=ui->offset->text();
    offset =str6.toInt();
    //滚子半径
    QString str7=ui->gradius->text();
    gradius=str7.toInt();
    //行程
    QString str8=ui->journey->text();
    journey =str8.toInt();
    qDebug() << "成功写入参数";
    tui=ui->comboBox->currentIndex();
    float x[360],y[360],x1[360],y1[360];

    float s1[360],s2[360],s3[360],s4[360];
    float dx1[360],dx2[360],dx3[360],dx4[360];
    float dy1[360],dy2[360],dy3[360],dy4[360];
    float ds1[360],ds2[360],ds3[360],ds4[360]; //对心平底凸轮


    s0=sqrt(yradius*yradius-offset*offset); //计算s0

    //推程的算法计算
    switch (tui) {
    case 0:
        qDebug()<<"推程运动规律：等速运动";
        for (int k=0;k<sport_angle;k++) {
              s1[k]=((k*pi) /(sport_angle*pi))*journey;
              ds1[k]=journey/(sport_angle*pi);
              dx1[k]=(journey/(sport_angle*pi))*sin(k*pi)+(s0+s1[k])*cos(k*pi);
              dy1[k]=(journey/(sport_angle*pi))*cos(k*pi)-(s0+s1[k])*sin(k*pi);
        }
        break;
    case 1:
        qDebug()<<"推程运动规律：等加速等减速";
        for (int k=0;k<sport_angle;k++) {
            if(k < sport_angle/2){
                s1[k]=(pow((k*pi),2)/pow((sport_angle*pi),2))*2*journey;
                ds1[k]=4*journey*(k*pi)/pow(sport_angle*pi,2);
                dx1[k]=(4*journey*(k*pi)/pow(sport_angle*pi,2))*sin(k*pi)+(s0+s1[k])*cos(k*pi);
                dy1[k]=(4*journey*(k*pi)/pow(sport_angle*pi,2))*cos(k*pi)-(s0+s1[k])*sin(k*pi);
            }
            else {
                s1[k]=journey-(pow((sport_angle*pi-k*pi),2)/pow((sport_angle*pi),2))*2*journey;
                ds1[k]=4*journey*(sport_angle*pi-k*pi)/pow(sport_angle*pi,2);
                dx1[k]=(4*journey*(sport_angle*pi-k*pi)/pow(sport_angle*pi,2))*sin(k*pi)+(s0+s1[k])*cos(k*pi);
                dy1[k]=(4*journey*(sport_angle*pi-k*pi)/pow(sport_angle*pi,2))*cos(k*pi)-(s0+s1[k])*sin(k*pi);
            }
        }
        break;
    case 2: qDebug()<<"推程运动规律：五次多项式";
        for (int k=0;k<sport_angle;k++) {
              s1[k]=(10*journey*pow((k*pi),3))/(pow((sport_angle*pi),3))-15*journey*pow((k*pi),4)/pow((sport_angle*pi),4)+6*journey*pow((k*pi),5)/pow((sport_angle*pi),5);
              ds1[k]=30*journey*pow(k*pi,2)/pow((sport_angle*pi),3)-60*journey*pow(k*pi,3)/pow((sport_angle*pi),4)+30*journey*pow(k*pi,4)/pow((sport_angle*pi),5);
              dx1[k]=(30*journey*pow(k*pi,2)/pow((sport_angle*pi),3)-60*journey*pow(k*pi,3)/pow((sport_angle*pi),4)+30*journey*pow(k*pi,4)/pow((sport_angle*pi),5))*sin(k*pi)+(s0+s1[k])*cos(k*pi);
              dy1[k]=(30*journey*pow(k*pi,2)/pow((sport_angle*pi),3)-60*journey*pow(k*pi,3)/pow((sport_angle*pi),4)+30*journey*pow(k*pi,4)/pow((sport_angle*pi),5))*cos(k*pi)-(s0+s1[k])*sin(k*pi);
        }
        break;
    case 3:
        qDebug()<<"推程运动规律：余弦加速度";
        for (int k=0;k<sport_angle;k++) {
              s1[k]=(1-(cos(3.14*(k*pi)/(sport_angle*pi))))*journey/2;
              ds1[k]=3.14159*journey*(sin(3.14*(k*pi)/(sport_angle*pi)))/(2*sport_angle*pi);
              dx1[k]=(3.14159*journey*(sin(3.14*(k*pi)/(sport_angle*pi)))/(2*sport_angle*pi))*sin(k*pi)+(s0+s1[k])*cos(k*pi);
              dy1[k]=(3.14159*journey*(sin(3.14*(k*pi)/(sport_angle*pi)))/(2*sport_angle*pi))*cos(k*pi)-(s0+s1[k])*sin(k*pi);
        }
        break;
    case 4:
        qDebug()<<"推程运动规律：正弦加速度";
        for (int k=0;k<sport_angle;k++) {
              s1[k]=((k*pi)/(sport_angle*pi)-sin(2*3.14159*(k*pi)/(sport_angle*pi))/(2*3.14159))*journey;
              ds1[k]=journey*(1-cos(2*k*pi))/(sport_angle*pi);
              dx1[k]=(journey*(1-cos(2*k*pi))/(sport_angle*pi))*sin(k*pi)+(s0+s1[k])*cos(k*pi);
              dy1[k]=(journey*(1-cos(2*k*pi))/(sport_angle*pi))*cos(k*pi)-(s0+s1[k])*sin(k*pi);
        }
        break;
    default:
        break;
    }

    //远休止算法计算
    for (int k=0;k<far_angle;k++) {
        s2[k]=journey;
        ds2[k]=0;
        dx2[k]=(yradius+s2[k])*cos(sport_angle*pi+k*pi);
        dy2[k]=-(yradius+s2[k])*sin(sport_angle*pi+k*pi);
    }


   //回程的算法计算
    switch (hui) {
    case 0:
        qDebug()<<"回程运动规律：等速运动";
        for (int k=0;k<hsport_angle;k++) {
            s3[k]=journey*(1-(k*pi)/(hsport_angle*pi));
            ds3[k]=-journey/(hsport_angle*pi);
            dx3[k]=(-journey/(hsport_angle*pi))*sin(sport_angle*pi+far_angle*pi+k*pi)+(s0+s3[k])*cos(sport_angle*pi+far_angle*pi+k*pi);
            dy3[k]=(-journey/(hsport_angle*pi))*cos(sport_angle*pi+far_angle*pi+k*pi)-(s0+s3[k])*sin(sport_angle*pi+far_angle*pi+k*pi);
        }
        break;
    case 1:
        qDebug()<<"回程运动规律：等加速等减速";
        for (int k=0;k<hsport_angle;k++) {
            if(k< hsport_angle/2){
                s3[k]=journey-(pow((k*pi),2)/pow((hsport_angle*pi),2))*2*journey;
                ds3[k]=-4*journey*(k*pi)/pow(hsport_angle*pi,2);
                dx3[k]=(-4*journey*(k*pi)/pow(hsport_angle*pi,2))*sin(sport_angle*pi+far_angle*pi+k*pi)+(s0+s3[k])*cos(sport_angle*pi+far_angle*pi+k*pi);
                dy3[k]=(-4*journey*(k*pi)/pow(hsport_angle*pi,2))*cos(sport_angle*pi+far_angle*pi+k*pi)-(s0+s3[k])*sin(sport_angle*pi+far_angle*pi+k*pi);
            }
            else {
                s3[k]=(pow((hsport_angle*pi-k*pi),2)/pow((hsport_angle*pi),2))*2*journey;
                ds3[k]=-4*journey*(hsport_angle*pi-k*pi)/pow(hsport_angle*pi,2);
                dx3[k]=(-4*journey*(hsport_angle*pi-k*pi)/pow(hsport_angle*pi,2))*sin(sport_angle*pi+far_angle*pi+k*pi)+(s0+s3[k])*cos(sport_angle*pi+far_angle*pi+k*pi);
                dy3[k]=(-4*journey*(hsport_angle*pi-k*pi)/pow(hsport_angle*pi,2))*cos(sport_angle*pi+far_angle*pi+k*pi)-(s0+s3[k])*sin(sport_angle*pi+far_angle*pi+k*pi);
            }
        }
        break;
    case 2:
        qDebug()<<"回程运动规律：五次多项式";
        for (int k=0;k<hsport_angle;k++) {
            s3[hsport_angle-1-k]=(10*journey*pow((k*pi),3))/(pow((hsport_angle*pi),3))-15*journey*pow((k*pi),4)/pow((hsport_angle*pi),4)+6*journey*pow((k*pi),5)/pow((hsport_angle*pi),5);
        }
        for (int k=0;k<hsport_angle;k++) {
            ds3[k]=30*journey*pow(k*pi,2)/pow((hsport_angle*pi),3)-60*journey*pow(k*pi,3)/pow((hsport_angle*pi),4)+30*journey*pow(k*pi,4)/pow((hsport_angle*pi),5);
            dx3[k]=(30*journey*pow(k*pi,2)/pow((hsport_angle*pi),3)-60*journey*pow(k*pi,3)/pow((hsport_angle*pi),4)+30*journey*pow(k*pi,4)/pow((hsport_angle*pi),5))*sin(sport_angle*pi+far_angle*pi+k*pi)+(yradius+s3[k])*cos(sport_angle*pi+far_angle*pi+k*pi);
            dy3[k]=(30*journey*pow(k*pi,2)/pow((hsport_angle*pi),3)-60*journey*pow(k*pi,3)/pow((hsport_angle*pi),4)+30*journey*pow(k*pi,4)/pow((hsport_angle*pi),5))*cos(sport_angle*pi+far_angle*pi+k*pi)-(yradius+s3[k])*sin(sport_angle*pi+far_angle*pi+k*pi);
        }
        break;
    case 3:
        qDebug()<<"回程运动规律：余弦加速度";
        for (int k=0;k<hsport_angle;k++) {
            s3[k]=(1+(cos(3.14159*(k*pi)/(hsport_angle*pi))))*journey/2;
            ds3[k]=-3.14159*journey*(sin(3.14*(k*pi)/(hsport_angle*pi)))/(2*hsport_angle*pi);
            dx3[k]=(-3.14159*journey*(sin(3.14*(k*pi)/(hsport_angle*pi)))/(2*hsport_angle*pi))*sin(sport_angle*pi+far_angle*pi+k*pi)+(s0+s3[k])*cos(sport_angle*pi+far_angle*pi+k*pi);
            dy3[k]=(-3.14159*journey*(sin(3.14*(k*pi)/(hsport_angle*pi)))/(2*hsport_angle*pi))*cos(sport_angle*pi+far_angle*pi+k*pi)-(s0+s3[k])*sin(sport_angle*pi+far_angle*pi+k*pi);
        }
        break;
    case 4:
        qDebug()<<"回程运动规律：正弦加速度";
        for (int k=0;k<hsport_angle;k++) {
            s3[k]=(1-(k*pi)/(hsport_angle*pi)+sin(2*3.14159*(k*pi)/(hsport_angle*pi))/(2*3.14159))*journey;
            ds3[k]=-journey*(1-cos(2*k*pi))/(hsport_angle*pi);
            dx3[k]=(-journey*(1-cos(2*k*pi))/(hsport_angle*pi))*sin(sport_angle*pi+far_angle*pi+k*pi)+(s0+s3[k])*cos(sport_angle*pi+far_angle*pi+k*pi);
            dy3[k]=(-journey*(1-cos(2*k*pi))/(hsport_angle*pi))*cos(sport_angle*pi+far_angle*pi+k*pi)-(s0+s3[k])*sin(sport_angle*pi+far_angle*pi+k*pi);
        }
        break;
    default:
        break;
    }

    //近休止算法计算
    for (int k=0;k<near_angle;k++) {
        s4[k]=0;
        ds4[k]=0;
        dx4[k]=(yradius+s4[k])*cos(sport_angle*pi+far_angle*pi+hsport_angle*pi+k*pi);
        dy4[k]=-(yradius+s4[k])*sin(sport_angle*pi+far_angle*pi+hsport_angle*pi+k*pi);
    }


    //偏置直动滚子推杆盘形凸轮
    if(tuigang==0){
        //计算理论廓线
        for (int i=0;i<=360;i++) {
           if(i>=0 &&i<sport_angle)
            {
                x[i]=(s0+s1[i])*sin(i*pi)+offset*cos(i*pi);
                y[i]=(s0+s1[i])*cos(i*pi)-offset*sin(i*pi);

            }
            else if ((i>=sport_angle) && i<(sport_angle+far_angle)) {
                x[i]=(yradius+s2[i-sport_angle])*sin(i*pi);
                y[i]=(yradius+s2[i-sport_angle])*cos(i*pi);
            }
            else if (i>=(sport_angle+far_angle) && i<(sport_angle+far_angle+hsport_angle)) {
                x[i]=(yradius+s3[i-sport_angle-far_angle])*sin(i*pi);
                y[i]=(yradius+s3[i-sport_angle-far_angle])*cos(i*pi);
            }
            else{
                x[i]=(yradius+s4[i-sport_angle-far_angle-hsport_angle])*sin(i*pi);
                y[i]=(yradius+s4[i-sport_angle-far_angle-hsport_angle])*cos(i*pi);
            }
           points[i]=QPointF(x[i]*3, -y[i]*3);
          }

        //计算工作廓线
        for (int i=0;i<=360;i++) {
            if(i>=0 && i<sport_angle)
            {
                x[0]=0;
                x1[i]=x[i]-(-dy1[i]/sqrt(pow(dx1[i],2)+pow(dy1[i],2)))*gradius;
                y1[i]=y[i]-(dx1[i]/sqrt(pow(dx1[i],2)+pow(dy1[i],2)))*gradius;
    //           qDebug("x%d：%f", i,x[0]);
    //           qDebug("y%d：%f", i,y[0]);
            }
            else if (i>=sport_angle && i<(sport_angle+far_angle)) {
                x1[i]=x[i]-gradius*(-dy2[i-sport_angle]/sqrt(pow(dx2[i-sport_angle],2)+pow(dy2[i-sport_angle],2)));
                y1[i]=y[i]-gradius*(dx2[i-sport_angle]/sqrt(pow(dx2[i-sport_angle],2)+pow(dy2[i-sport_angle],2)));
            }
            else if (i>=(sport_angle+far_angle) && i<(sport_angle+far_angle+hsport_angle)) {
                x1[i]=x[i]-gradius*(-dy3[i-sport_angle-far_angle]/sqrt(pow(dx3[i-sport_angle-far_angle],2)+pow(dy3[i-sport_angle-far_angle],2)));
                y1[i]=y[i]-gradius*(dx3[i-sport_angle-far_angle]/sqrt(pow(dx3[i-sport_angle-far_angle],2)+pow(dy3[i-sport_angle-far_angle],2)));
            }
            else{
                int k=i-sport_angle-far_angle-hsport_angle;
                x1[i]=x[i]-gradius*(-dy4[k]/sqrt(pow(dx4[k],2)+pow(dy4[k],2)));
                y1[i]=y[i]-gradius*(dx4[k]/sqrt(pow(dx4[k],2)+pow(dy4[k],2)));
            }
            kx1[i]=x1[i];
            ky1[i]=y1[i];
            points2[i]=QPointF(x1[i]*3, -y1[i]*3);
        }

    }
    //对心平底推杆盘形凸轮
    else if(tuigang==1){
        for (int i=0;i<=360;i++) {
            if(i>=0 && i<sport_angle){
                x1[i]=(yradius+s1[i])*sin(i*pi)+ds1[i]*cos(i*pi);
                y1[i]=(yradius+s1[i])*cos(i*pi)-ds1[i]*sin(i*pi);
            }
            else if (i>=sport_angle && i<(sport_angle+far_angle)){
                x1[i]=(yradius+s2[i-sport_angle])*sin(i*pi);
                y1[i]=(yradius+s2[i-sport_angle])*cos(i*pi);
            }
            else if (i>=(sport_angle+far_angle) && i<(sport_angle+far_angle+hsport_angle)) {
                x1[i]=(yradius+s3[i-sport_angle-far_angle])*sin(i*pi)+ds3[i-sport_angle-far_angle]*cos(i*pi);
                y1[i]=(yradius+s3[i-sport_angle-far_angle])*cos(i*pi)-ds3[i-sport_angle-far_angle]*sin(i*pi);
            }
            else {
                x1[i]=(yradius+s4[i-sport_angle-far_angle-hsport_angle])*sin(i*pi)
                                 +ds4[i-sport_angle-far_angle-hsport_angle]*cos(i*pi);
                y1[i]=(yradius+s4[i-sport_angle-far_angle-hsport_angle])*cos(i*pi)
                                -ds4[i-sport_angle-far_angle-hsport_angle]*sin(i*pi);
            }
            kx1[i]=x1[i];
            ky1[i]=y1[i];
            points[i]=QPointF(0, 0);
            points2[i]=QPointF(x1[i]*3, -y1[i]*3);
        }

    }
    //摆动滚子推杆盘形
    else{

    }

        //更新绘图标志位
        huitu=1;
        huabu=0;
        update();

}

// 画图设置
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPainter painter2(this);
    QPixmap pixmap;
    QPen pen;

    painter.setRenderHint(QPainter::Antialiasing); // 启用抗锯齿效果
    painter.translate(568, 243); // 把坐标原点移动到 widget 的中心
//    qDebug("%d",width()/2+30);
//    qDebug("%d",height()/2-70);
    painter2.setRenderHint(QPainter::Antialiasing); // 启用抗锯齿效果
    painter2.translate(568, 243); // 把坐标原点移动到 widget 的中心

    //绘制画布，为避免刷新只启用一次
     painter.fillRect(QRect(-172, -172,390,400), QBrush(Qt::white));
     painter.drawRect(QRect(-172, -172,390,400));

     pixmap.load("D:/qt project/shukong/shukongsheji/1.bmp");
     painter.drawPixmap(120, 250,100, 100, pixmap);

     if(huabu==0)
     {
        huitu=1;
     }
     if(huabu==1)
     {
        huitu=0;
     }
    if(huitu==1){
        painter.save();
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.drawPolygon(points, 360);
        painter.restore();
        painter.drawPolygon(points2, 360);
        huitu=0;
    }
    else {

    }
}


//数控代码生成按钮函数
void MainWindow::on_pushButton_4_clicked()
{ 

    ui->textEdit->clear(); //清空原有数据
    qDebug("%d",daobu);


    QInputDialog *inputDialog = new QInputDialog(this);
    bool getInfo;
    QString down = inputDialog->getText(this,"输入","请输入加工凸轮厚度",QLineEdit::Normal,"",&getInfo,Qt::WindowFlags(0),Qt::ImhNone);
    if(getInfo){
        houdu=down.toInt();
    }
        qDebug("%d",houdu);
    //机床启动代码
    ui->textEdit->insertPlainText("%0010\n");
    ui->textEdit->insertPlainText("G90 MO3 S"); //绝对坐标编程
    ui->textEdit->insertPlainText(QString::number(Spindle_speed));
    ui->textEdit->insertPlainText("\n");
    ui->textEdit->insertPlainText("G54 ");
    if(daobu==1){
     ui->textEdit->insertPlainText("G01 G41 X0 Y0 D01 Z10 F");
     ui->textEdit->insertPlainText(QString::number(Feed_rate));
     ui->textEdit->insertPlainText("\n");
     //走刀坐标输出
     //工件移动置加工起点，下刀
     ui->textEdit->insertPlainText("G01 X");
     ui->textEdit->insertPlainText(QString::number(kx1[0]));
     ui->textEdit->insertPlainText(" Y");
     ui->textEdit->insertPlainText(QString::number(ky1[0]));
     ui->textEdit->insertPlainText("\n");
     ui->textEdit->insertPlainText("Z-");
     ui->textEdit->insertPlainText(QString::number(houdu));
     ui->textEdit->insertPlainText("\n");

     //推程段的非圆曲线逼近
     for (int i=1;i<sport_angle;i++) {
          ui->textEdit->insertPlainText("G01 X");
          ui->textEdit->insertPlainText(QString::number(kx1[i]));
          ui->textEdit->insertPlainText(" Y");
          ui->textEdit->insertPlainText(QString::number(ky1[i]));
          ui->textEdit->insertPlainText("\n");
     }
     //远休止段的圆形加工，采用R指令
          ui->textEdit->insertPlainText("G02 X");
          ui->textEdit->insertPlainText(QString::number(kx1[180]));
          ui->textEdit->insertPlainText(" Y");
          ui->textEdit->insertPlainText(QString::number(ky1[180]));
          ui->textEdit->insertPlainText(" R");
          ui->textEdit->insertPlainText(QString::number(yradius+journey));
          ui->textEdit->insertPlainText("\n");
     //回程段的非圆曲线逼近
     for (int i=(sport_angle+far_angle);i<(sport_angle+far_angle+hsport_angle);i++) {
          ui->textEdit->insertPlainText("G01 X");
          ui->textEdit->insertPlainText(QString::number(kx1[i]));
          ui->textEdit->insertPlainText(" Y");
          ui->textEdit->insertPlainText(QString::number(ky1[i]));
          ui->textEdit->insertPlainText("\n");
      }
     //近休止段的圆形加工，采用R指令
          ui->textEdit->insertPlainText("G02 X");
          ui->textEdit->insertPlainText(QString::number(kx1[0]));
          ui->textEdit->insertPlainText(" Y");
          ui->textEdit->insertPlainText(QString::number(ky1[0]));
          ui->textEdit->insertPlainText(" R");
          ui->textEdit->insertPlainText(QString::number(yradius));
          ui->textEdit->insertPlainText("\n");
    } //右刀补
    else if(daobu==2){
        qDebug("右");
        ui->textEdit->insertPlainText("G01 G42 X0 Y0 D01 Z10 F");
        ui->textEdit->insertPlainText(QString::number(Feed_rate));
        ui->textEdit->insertPlainText("\n");
        //走刀坐标输出
        //工件移动置加工起点，下刀，默认加工深度为10
        ui->textEdit->insertPlainText("G01 X");
        ui->textEdit->insertPlainText(QString::number(kx1[0]));
        ui->textEdit->insertPlainText(" Y");
        ui->textEdit->insertPlainText(QString::number(ky1[0]));
        ui->textEdit->insertPlainText("\n");
        ui->textEdit->insertPlainText("Z-");
        ui->textEdit->insertPlainText(QString::number(houdu));
        ui->textEdit->insertPlainText("\n");
        //近休止段的圆形加工，采用R指令
        ui->textEdit->insertPlainText("G03 X");
        ui->textEdit->insertPlainText(QString::number(kx1[sport_angle+far_angle+hsport_angle]));
        ui->textEdit->insertPlainText(" Y");
        ui->textEdit->insertPlainText(QString::number(ky1[sport_angle+far_angle+hsport_angle]));
        ui->textEdit->insertPlainText(" R");
        ui->textEdit->insertPlainText(QString::number(yradius));
        ui->textEdit->insertPlainText("\n");
        //回程段的非圆曲线逼近
        for (int i=(sport_angle+far_angle+hsport_angle);i>(sport_angle+far_angle);i--) {
             ui->textEdit->insertPlainText("G01 X");
             ui->textEdit->insertPlainText(QString::number(kx1[i]));
             ui->textEdit->insertPlainText(" Y");
             ui->textEdit->insertPlainText(QString::number(ky1[i]));
             ui->textEdit->insertPlainText("\n");
         }
        //远休止段的圆形加工，采用R指令
        ui->textEdit->insertPlainText("G03 X");
        ui->textEdit->insertPlainText(QString::number(kx1[sport_angle]));
        ui->textEdit->insertPlainText(" Y");
        ui->textEdit->insertPlainText(QString::number(ky1[sport_angle]));
        ui->textEdit->insertPlainText(" R");
        ui->textEdit->insertPlainText(QString::number(yradius+journey));
        ui->textEdit->insertPlainText("\n");
        //推程段的非圆曲线逼近
        for (int i=sport_angle;i>0;i--) {
             ui->textEdit->insertPlainText("G01 X");
             ui->textEdit->insertPlainText(QString::number(kx1[i]));
             ui->textEdit->insertPlainText(" Y");
             ui->textEdit->insertPlainText(QString::number(ky1[i]));
             ui->textEdit->insertPlainText("\n");
        }
    }
    //解除刀补
    ui->textEdit->insertPlainText("G40 X0 Y0 Z10");
    ui->textEdit->insertPlainText("\n");
    //机床停止运行代码
    ui->textEdit->insertPlainText("M05 M30");
}


//清空数据函数
void MainWindow::on_pushButton_2_clicked()
{
     ui->textEdit->clear(); //清空数据
     huabu=1;
     update();//更新画图，重新画个白布进行填充
}



//参数输入错误提示窗口
void MainWindow::onMessageBox()
{
        //含有为0的提示输入完整参数，都不为0则判断角度是否正确
       if (sport_angle==0 ||far_angle==0 || hsport_angle==0 || near_angle==0 ||yradius==0 )
       {
           QMessageBox::critical(this, "错误的参数", "请输入完整的参数",
                       QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       }
       else{
           if(sport_angle+far_angle+hsport_angle+near_angle!=360){
               QMessageBox::critical(this, "错误的参数", "角度输入错误",
                           QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
           }
       }
}



//数控机床设置代码
void MainWindow::Setting_CNCmachine()
{
    NCstting *v = new  NCstting();
    v->show();
}

//导出数控代码
void MainWindow::NCexport()
{

        QFileDialog dlg(this);

        //获取内容的保存路径
        QString fileName = dlg.getSaveFileName(this, tr("Save As"), "./", tr("Text File(*.txt)"));

        if( fileName == "" )
        {
            return;
        }

        //内容保存到路径文件
        QFile file(fileName);

        //以文本方式打开
        if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream out(&file); //IO设备对象的地址对其进行初始化

            out << ui->textEdit->toPlainText() << endl; //输出

            QMessageBox::information(this, tr("结束"), tr("成功保存到文件!"));

            file.close();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("保存文件失败!"));
        }
}

//导出SVG CAD可以读取的文件
void MainWindow::SVGexport()
{

        QFileDialog dlg(this);

        //获取内容的保存路径
        QString filePath = dlg.getSaveFileName(this, tr("Save SVG"), "./", tr("SVG files (*.svg)"));

        if( filePath == "" )
        {
            return;
        }
        //内容保存到路径文件
        QFile file(filePath);

        QSvgGenerator generator;
        generator.setFileName(filePath);

        QPainter painter3;
        painter3.begin(&generator);
        painter3.drawPolygon(points, 360);
        painter3.drawPolygon(points2, 360);
        painter3.end();
        QMessageBox::information(this, tr("结束"), tr("成功保存SVG文件!"));
}

//一键复制到剪切板
void MainWindow::on_pushButton_5_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText( ui->textEdit->toPlainText());
    QMessageBox::information(this, tr("复制成功"), tr("代码成功复制到剪切板"));
}

void MainWindow::on_pushButton_SVG_clicked()
{
    QFileDialog dlg(this);

    //获取内容的保存路径
    QString filePath = dlg.getSaveFileName(this, tr("Save SVG"), "./", tr("SVG files (*.svg)"));

    if( filePath == "" )
    {
        return;
    }
    //内容保存到路径文件
    QFile file(filePath);

    QSvgGenerator generator;
    generator.setFileName(filePath);

    QPainter painter3;
    painter3.begin(&generator);
    painter3.drawPolygon(points, 360);
    painter3.drawPolygon(points2, 360);
    painter3.end();
    QMessageBox::information(this, tr("结束"), tr("成功保存SVG文件!"));
}

