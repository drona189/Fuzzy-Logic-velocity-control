#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<qextserialport.h>
#include <QMenuBar>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void serialupdate();
    void algo(char );

private:
    Ui::MainWindow *ui;
    QextSerialPort *port;
    QMenuBar *m;


private slots:
  //  void on_comboBox_3_currentIndexChanged(int index);
  //  void on_comboBox_3_currentIndexChanged(int index);
    void on_comboBox_3_currentIndexChanged(int index);
    void on_pushButton_4_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_comboBox_2_currentIndexChanged(int index);
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();


};

#endif // MAINWINDOW_H
