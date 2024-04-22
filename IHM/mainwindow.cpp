#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tcp(this), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->touchableZone->installEventFilter(this);
    this->touchHeight = ui->touchableZone->size().height();
    this->touchWidth = ui->touchableZone->size().width();
    QLabel *pointLabel = new QLabel(ui->touchableZone);
    pointLabel->setFixedSize(10, 10); // Taille du point
    pointLabel->setStyleSheet("background-color: black; border-radius: 5px;"); // Couleur et forme du point
    pointLabel->move(QPoint(touchHeight/2, touchWidth/2) - QPoint(5, 5)); // Déplacer le label pour centrer le point
    pointLabel->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveImpact(QString t)
{
    QStringList coord = t.split(' ');
    qDebug() << "Coordonnées de l'impact : " << coord.value(0).trimmed().replace(',', '.').toFloat()+(this->touchWidth/2)<< "," << (coord.value(2).trimmed().replace(',', '.').toFloat()-(this->touchHeight/2))*-1;


    QPoint impact = QPoint(coord.value(0).trimmed().replace(',', '.').toFloat()+(this->touchWidth/2),(coord.value(2).trimmed().replace(',', '.').toFloat()-(this->touchHeight/2))*-1);
    qDebug() << impact;
    QLabel *pointLabel = new QLabel(ui->touchableZone);
    pointLabel->setFixedSize(5, 5); // Taille du point
    pointLabel->setStyleSheet("background-color: red; border-radius: 3px;"); // Couleur et forme du point
    pointLabel->move(impact - QPoint(2, 2)); // Déplacer le label pour centrer le point
    pointLabel->show();
}

void MainWindow::on_shotBtn_clicked()
{
    tcp.Send_Message(1);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << event->type();
    if (obj == ui->touchableZone && event->type() == QEvent::MouseButtonPress) {
        // Cast event to QMouseEvent
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        // Check if left mouse button is pressed
        if (mouseEvent->button() == Qt::LeftButton) {
            // Convert local mouse position to global position
            //QPoint globalPos = ui->touchableZone->mapToGlobal(mouseEvent->pos());
            qDebug() << "Coordonnées du clic : " << mouseEvent->pos().x()-(this->touchWidth/2) << ", " << (mouseEvent->pos().y()-(this->touchHeight/2))*-1;
            if (this->target != nullptr) {
                // Supprimer l'ancien QLabel
                this->target->deleteLater();
            }
            this->target = new QLabel(ui->touchableZone);
            this->target->setFixedSize(5, 5); // Taille du point
            this->target->setStyleSheet("background-color: lightgreen; border-radius: 3px;"); // Couleur et forme du point
            this->target->move(mouseEvent->pos() - QPoint(2, 2)); // Déplacer le label pour centrer le point
            this->target->show();

        }
        // Return true to stop further event processing
        return true;
    }
    // Pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_shotPower_valueChanged(int value)
{
    this->tcp.Send_Message(4,3,value);
}


void MainWindow::on_canonInclinaison_valueChanged(int value)
{
    this->tcp.Send_Message(4,2,value);
}


void MainWindow::on_canonRotation_valueChanged(int value)
{
    this->tcp.Send_Message(4,1,value);
}


void MainWindow::on_windPower_valueChanged(int value)
{
    this->tcp.Send_Message(2,1,value);
}


void MainWindow::on_windDirection_valueChanged(int value)
{
    this->tcp.Send_Message(2,2,value);
}

