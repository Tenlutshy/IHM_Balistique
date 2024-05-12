#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Logger
{
public:
    Logger();
    static void writeLog(QString message);
};

#endif // LOGGER_H
