@[TOC](广工数控课设：平面凸轮的数控加工程序的编制)

---
***
# 1.前言
  网上数控课设多采用的是VB6.0编程的，感觉VB6.0已经好老的，软件的设计界面也不好看，设计逻辑感觉也是非常的脑残。所以分享一份基于QT的平面凸轮的数控课设（虽然QT是C++，但我基本上还是用了很多C编程）。完整的工程代码见文末。
# 2.软件界面
![程序主界面](https://img-blog.csdnimg.cn/7bc5174329fa4be282c2e127798c6c9a.png#pic_center)
用户可以选择凸轮是什么类型的推杆，然后输入凸轮参数，选择保存参数，在选择凸轮的运动规律，点击生成轮廓就可以绘制出凸轮的曲线了，点击生成数控代码，就可以在代码生成区中生成凸轮的数控代码。

# 2.主程序设计与分析
## 2.1参数输入控件
效果就是在主界面上输入参数，然后点击按钮，将参数保存进来。
按钮用的是pushButton控件，然后要怎么建立ui文件和cpp文件之间的联系呢?这就要涉及到QT的信号与槽函数之间的关系了，具体实现方式如下：[Qt七种信号与槽关联方式小结](https://blog.csdn.net/weixin_57040091/article/details/115730560?ops_request_misc=&request_id=&biz_id=102&utm_term=%E4%BF%A1%E5%8F%B7%E4%B8%8E%E6%A7%BD%E5%BB%BA%E7%AB%8B%E7%9A%84%E6%96%B9%E5%BC%8F&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-3-115730560.142%5Ev32%5Enew_blog_pos_by_title,185%5Ev2%5Econtrol&spm=1018.2226.3001.4187)
按钮控件我用的是第四种方式。

参数输入的控件用的textline，支持一行文本的输入，传入的参数为字符串型的变量，所以我用QString类来传递和转换输入进来的参数。具体效果如下：
当然咯，你在使用某个类的时候，一定要在前面加上它的头文件#include <qstring.h>
```cpp
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
```
## 2.2生成凸轮轮廓
凸轮的运动规律选择有5种，具体可以看机械原理课本，分别是等速运动、等加速等减速、五次多项式、余弦加速度和正弦加速度。这里我选择的是5种都做出来。不同的运动规律的选择，我用的是comboBox控件，可以读取当前的判断选择的是那个运动的规律。
然后具体的实现过程也是利用转到槽建立槽函数，comboBox里面的内容改变了就会进入一次槽函数，然后在槽函数读取，这里我设定了两个个全局变量tui和hui，用来标定用户的凸轮推程和回程分别选择的是什么运动规律。

```cpp

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
```

然后就要生成理论廓线和实际工作廓线。具体的方法用的方法是凸轮的解析法，按照选择的运动规律，计算推杆的行程s和s对角度的微分（自己求导，然后用公式在程序里面表达出来）。具体的步骤可以参照机械原理课本的案例，照着它来一步一步的编程序，先写一个运动规律，其余的运动规律也会很好写了。

实现过程如下：

 1. 使用pushButton控件，然后建立槽函数

```cpp
//计算各个参数并生成轮廓曲线的槽函数
void MainWindow::on_pushButton_3_clicked()
{
}
```
 2. 数据读取
 为了防止用户没有点击上面的保存参数按钮，而是直接点击生成轮廓按钮，导致出现了错误，所以我在这在读取一遍数据，代码跟上面的一样就行了。
 
  3. 行程s的计算
  利用for循环不断的计算对应角度的s，还有ds，这里我推程角，远休止角，回程角和近休止角都是从0开始算到最大值的。
  放在槽函数的程序如下，利用switch语句来判断选择的是哪种运动规律：
  

```cpp
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
```
  4. 理论廓线和实际廓线的计算
  这个按照书本的解析法来写，从0到360°，不过写的时候要注意过渡点的计算，我的程序是不计算的最大角度，而是最大角度作为下一段的初始角度，这样就能够实现均匀的过度了。
  

```cpp
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
```
  5. 凸轮曲线的绘制
  曲线的绘制利用的是QT中的 QPainter类，具体的使用可以参考这个：[QPainter详解](https://blog.csdn.net/qq21497936/article/details/105506028?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522165752664816780357210005%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=165752664816780357210005&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~hot_rank-2-105506028-null-null.142%5Ev32%5Enew_blog_pos_by_title,185%5Ev2%5Econtrol&utm_term=qpainter&spm=1018.2226.3001.4187)
我设置的是两个画家，后来发现可以不用的，一个就行了，Qpen设置画笔，因为理论廓线是用虚线画的。整个图像的白色画布直接画了个矩形，然后根据事假的更新来控制绘画。
```cpp

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
```
## 2.2数控代码的生成
利用的是按钮控件和textEdit控件，点击按钮然后在textEdit上输出数控代码，具体的代码如下：
这里的代码会有的长，一开始想字符串的拼接，但是嫌麻烦懒得搞了。然后后面知道了可以利用Qstring类来进行字符串的拼接和处理。有想法的可以试一下。
```cpp
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
```
## 2.4一些附加功能

 1. 参数错误提示功能

```cpp
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
```

 2. 清空功能

```cpp
//清空数据
void MainWindow::on_pushButton_2_clicked()
{
     ui->textEdit->clear(); //清空数据
     huabu=1;
     update();//更新画图，重新画个白布进行填充
}
```
 3. 代码导出功能
 
 用到的是QFileDialog这个类
```cpp
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
```
 4.代码一键复制功能
用到的是 QClipboard这个类
```cpp
//一键复制到剪切板
void MainWindow::on_pushButton_5_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText( ui->textEdit->toPlainText());
    QMessageBox::information(this, tr("复制成功"), tr("代码成功复制到剪切板"));
}
```

# 3结束
感觉QT还是非常的好入门的，只要是想认真搞这个课设的，稍微花一点时间还是会很轻松的拿下的，需要用到什么功能可以参考我的博客。附上自己的整个工程的代码还有报告，里面还有个可以直接运行的程序（不用QT就可以直接运行）。
