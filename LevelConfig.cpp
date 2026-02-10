#include "LevelConfig.h"

// Implementación de LevelConfig
LevelConfig::LevelConfig()
    : nivel(0), maxPosersAllowed(0), maxVerificaciones(3), personasPorNivel(0),
      duracionMaximaEstancia(0), maxFallosPermitidos(0), edad(0) {}

// Métodos de serialización
void LevelConfig::serialize(QDataStream &out) const {
  out << nivel << maxPosersAllowed << maxVerificaciones << personasPorNivel
      << cantidadPersonas << duracionMaximaEstancia << maxFallosPermitidos
      << edad;

  // Serialize vectors
  out << static_cast<quint32>(bannedGoals.size());
  for (const auto &s : bannedGoals)
    out << QString::fromStdString(s);

  out << static_cast<quint32>(bannedStyles.size());
  for (const auto &s : bannedStyles)
    out << QString::fromStdString(s);

  out << static_cast<quint32>(bannedBrands.size());
  for (const auto &s : bannedBrands)
    out << QString::fromStdString(s);

  out << static_cast<quint32>(bannedRoles.size());
  for (const auto &s : bannedRoles)
    out << QString::fromStdString(s);

  out << static_cast<quint32>(bannedStances.size());
  for (const auto &s : bannedStances)
    out << QString::fromStdString(s);

  out << static_cast<quint32>(bannedDeckConditions.size());
  for (const auto &s : bannedDeckConditions)
    out << QString::fromStdString(s);

  out << QString::fromStdString(bannedCrewStruct)
      << QString::fromStdString(bannedBrandStruct);

  out << static_cast<quint32>(bannedCrews.size());
  for (const auto &s : bannedCrews)
    out << QString::fromStdString(s);

  out << static_cast<quint32>(restricciones.size());
  for (const auto &restriccion : restricciones) {
    out << QString::fromStdString(restriccion);
  }
}

void LevelConfig::deserialize(QDataStream &in) {
  in >> nivel >> maxPosersAllowed >> maxVerificaciones >> personasPorNivel >>
      cantidadPersonas >> duracionMaximaEstancia >> maxFallosPermitidos >> edad;

  quint32 size;
  QString str;

  // Deserialize vectors
  in >> size;
  bannedGoals.clear();
  for (quint32 i = 0; i < size; ++i) {
    in >> str;
    bannedGoals.push_back(str.toStdString());
  }

  in >> size;
  bannedStyles.clear();
  for (quint32 i = 0; i < size; ++i) {
    in >> str;
    bannedStyles.push_back(str.toStdString());
  }

  in >> size;
  bannedBrands.clear();
  for (quint32 i = 0; i < size; ++i) {
    in >> str;
    bannedBrands.push_back(str.toStdString());
  }

  in >> size;
  bannedRoles.clear();
  for (quint32 i = 0; i < size; ++i) {
    in >> str;
    bannedRoles.push_back(str.toStdString());
  }

  in >> size;
  bannedStances.clear();
  for (quint32 i = 0; i < size; ++i) {
    in >> str;
    bannedStances.push_back(str.toStdString());
  }

  in >> size;
  bannedDeckConditions.clear();
  for (quint32 i = 0; i < size; ++i) {
    in >> str;
    bannedDeckConditions.push_back(str.toStdString());
  }

  QString bCrewStruct, bBrandStruct;
  in >> bCrewStruct >> bBrandStruct;
  bannedCrewStruct = bCrewStruct.toStdString();
  bannedBrandStruct = bBrandStruct.toStdString();

  in >> size;
  bannedCrews.clear();
  for (quint32 i = 0; i < size; ++i) {
    in >> str;
    bannedCrews.push_back(str.toStdString());
  }

  in >> size;
  restricciones.clear();
  for (quint32 i = 0; i < size; ++i) {
    QString restriccion;
    in >> restriccion;
    restricciones.emplace_back(restriccion.toStdString());
  }
}
