#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <QDataStream>
#include <QString>
#include <string>
#include <vector>


using namespace std;

class LevelConfig {
public:
  int nivel;
  int maxErrores;
  int maxVerificaciones;
  int personasPorNivel;
  int cantidadPersonas;
  string tipoVisitaRestringido;
  int duracionMaximaEstancia;
  int maxFallosPermitidos;
  string reglasTexto;
  string motivoViajeRestringido;
  string paisRechazar;
  string tipoPersonaRechazar;
  string nacionalidadStruct;
  string residenciaStruct;
  string paisesEnGuerra[4];
  string estadoAmor;
  int edad;
  vector<string> restricciones;

  LevelConfig();
  // Métodos de serialización
  void serialize(QDataStream &out) const;
  void deserialize(QDataStream &in);
};

#endif // LEVELCONFIG_H
