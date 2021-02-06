#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ui_model(new QStandardItemModel())
{
    ui->setupUi(this);

    ui_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Variable")));
    ui_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Value")));

    ui->treeView->setModel(ui_model);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ui_model;
}

void MainWindow::g_signal_calculate()
{
    ui_model->removeRows(0, ui_model->rowCount());
    vector<double> ans;
    cal.init(ui->textEdit->toPlainText().toStdString());
    int response = cal.get_ans(ans);
    for (int i=0;i<ans.size();++i)
    {
        QList<QStandardItem *> q;
        q.push_back(new QStandardItem(QString::fromStdString(cal.get_name(i))));
        q.push_back(new QStandardItem(QString::number(ans[i])));
    }
}

