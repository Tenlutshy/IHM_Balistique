#include "logger.h"

Logger::Logger() {}

void Logger::writeLog(QString message) {
    QFile file("logfile.log");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur: Impossible d'ouvrir le fichier de journalisation";
        return;
    }

    QTextStream out(&file);
    QDateTime timestamp = QDateTime::currentDateTime();
    out << timestamp.toString(Qt::ISODate) << " - " << message << "\n";
}
