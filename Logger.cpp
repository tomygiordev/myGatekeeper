#include "Logger.h"

Logger::Logger(const QString &filePath) {
  logFile.setFileName(filePath);
  if (logFile.open(QIODevice::Append | QIODevice::Text)) {
    logStream.setDevice(&logFile);
  } else {
    QMessageBox::warning(nullptr, "Error de Log",
                         "No se pudo abrir el archivo de log.");
  }
}

Logger::~Logger() {
  if (logFile.isOpen()) {
    logFile.close();
  }
}

void Logger::log(const QString &message) {
  if (logFile.isOpen()) {
    QString timeStamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    logStream << "[" << timeStamp << "] " << message << "\n";
    logStream.flush();
  }
}
