#ifndef NCSTTING_H
#define NCSTTING_H

#include <QWidget>

namespace Ui {
class NCstting;
}

class NCstting : public QWidget
{
    Q_OBJECT

public:
    explicit NCstting(QWidget *parent = nullptr);
    ~NCstting();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void onMessageBox1();

    void daobusetting();

private:
    Ui::NCstting *ui;
};


#endif // NCSTTING_H
