#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSerialPort/QSerialPort>

#include <QMainWindow>
#include <QPushButton>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool toBool(QString tmp);

    void getlift(int lift){lift=lift;}
    void dataHandle(QString);
    void displaylift(QString s);
    bool run_or_stop();
    bool isGood();
        // 类型转化  QString 转化为 BOOL
    void showWin();

    void panduan(QPushButton *tmp ,bool flag)
      {
        if(flag)
           tmp->setStyleSheet("background:green");
        else
             tmp->setStyleSheet("background:");
       }

    ~MainWindow();

public slots:

    void read_data();

private slots:


    void on_stop_clicked();

    void on_run_clicked();

    void on_openqsort_clicked();

    void on_closeqsort_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

      int lift;
      bool lightflag;
      bool l1U;
      bool l2U;
      bool l2D;
      bool l3U;
      bool l3D;
      bool l4D;
      bool l1;
      bool l2;
      bool l3;
      bool l4;
      bool opendor;
      bool closedor;
      bool stopFlag;
      bool runFlag;
      bool readyopen;
      bool readyclose;
      bool upFlag;
      bool dowmFlag;
      char beginFlag;
      char endFlag;
};

#endif // MAINWINDOW_H
