#include "LevelConfig.h"

// Implementación de LevelConfig
LevelConfig::LevelConfig()
    : nivel(0), maxErrores(0), maxVerificaciones(3), personasPorNivel(0) {}

// Métodos de serialización
void LevelConfig::serialize(QDataStream &out) const {
  out << nivel << maxErrores << maxVerificaciones << personasPorNivel;
  out << static_cast<quint32>(restricciones.size());
  for (const auto &restriccion : restricciones) {
    out << QString::fromStdString(restriccion);
  }
}
void LevelConfig::deserialize(QDataStream &in) {
  in >> nivel >> maxErrores >> maxVerificaciones >> personasPorNivel;
  quint32 size;
  in >> size;
  restricciones.clear();
  for (quint32 i = 0; i < size; ++i) {
    QString restriccion;
    in >> restriccion;
    restricciones.emplace_back(restriccion.toStdString());
  }
}
