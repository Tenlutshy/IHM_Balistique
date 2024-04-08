#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tcp(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // No need to initialize the TCPServeur here; it's initialized in the member declaration
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_shotBtn_clicked()
{
    tcp.Send_Message(1);
}
