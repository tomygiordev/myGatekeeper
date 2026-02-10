#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <QDataStream>
#include <QString>
#include <QtGlobal>
#include <string>
#include <vector>

using namespace std;

class LevelConfig {
public:
  int nivel;
  int maxPosersAllowed; // Renamed from maxErrores
  int maxVerificaciones;
  int personasPorNivel;
  int cantidadPersonas;
  int duracionMaximaEstancia;
  int maxFallosPermitidos; // Legacy? Maybe redundant with maxPosersAllowed?
                           // Let's keep one. Actually maxErrores was used for
                           // "Game Over this level", maxFallosPermitidos for
                           // "Game Over global"? No, looks like Level 1 rule
                           // said "Maximo de errores permitidos: 2". Let's
                           // perform a merge: use maxPosersAllowed as the main
                           // one.

  std::vector<std::string> bannedGoals;   // Renamed from bannedGoal
  std::vector<std::string> bannedStyles;  // Renamed from bannedStyle
  std::vector<std::string> bannedBrands;  // Renamed from bannedBrand
  std::vector<std::string> bannedRoles;   // Renamed from bannedRole
  std::vector<std::string> bannedStances; // Renamed from bannedStance
  std::vector<std::string>
      bannedDeckConditions; // Renamed from bannedDeckCondition

  // Specific combo bans (keep as is or make vector of pairs? simplicity:
  // strings "Crew:Brand") Let's keep them as single for now if they are just
  // one rule, or use vectors if multiple. Rule 5 level 4: "Crew y Marca
  // prohibida: [Neon Hawks], [Nike SB]" -> 1 pair. Rule 4 level 5: "Crew/Marca:
  // [Vibe Checkers], [Walmart]" -> 1 pair. So maybe just specific strings is
  // fine, or vector of pairs. Let's use string for simplicity but rename
  // consistency.
  string bannedCrewStruct;
  string bannedBrandStruct;

  std::vector<std::string> bannedCrews;

  int edad; // Min/Max age or just "Age Restriction" value

  // Additional restrictions text for display
  vector<string> restricciones;

  LevelConfig();
  // Serialization methods
  void serialize(QDataStream &out) const;
  void deserialize(QDataStream &in);
};

#endif // LEVELCONFIG_H
