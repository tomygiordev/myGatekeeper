#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>


class Logger {
private:
  QFile logFile;
  QTextStream logStream;

public:
  Logger(const QString &filePath);
  ~Logger();
  void log(const QString &message);
};

#endif // LOGGER_H
