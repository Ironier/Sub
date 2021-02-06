#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <gauss_class.h>

#include <QStandardItem>
#include <QStandardItemModel>

#include <QList>
#include <QString>
#include <string>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Calculator<double> cal;

    Ui::MainWindow *ui;
    QStandardItemModel *ui_model;

private slots:
    void g_signal_calculate();
};
#endif // MAINWINDOW_H
