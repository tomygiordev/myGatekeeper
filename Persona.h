#ifndef PERSONA_H
#define PERSONA_H

#include <QDataStream>
#include <QString>
#include <string>


using namespace std;

class Persona {
private:
  string nombreApellido;
  string nacionalidad;
  string fechaNac;
  string TipoV;
  string ECivil;
  string paisResi;
  string motivo;
  int duracion;
  bool viajaSolo;
  string tipoPersona;
  bool documentacionValida;
  bool paisRestringido;
  bool genero;        // true: hombre, false: mujer
  QString rutaImagen; // Ruta de la imagen asociada

public:
  Persona();
  // Constructor con parámetros
  Persona(const string &nomApe, const string &nac, const string &fecha,
          const string &tipoVis, const string &estCivil,
          const string &paisResidencia, const string &proposito, int dur,
          bool solo, const string &tipoPers, bool docValida, bool paisRestring,
          bool gen, const QString &imgRuta);

  // Métodos de acceso
  string obtenerNombreApellido() const;
  string obtenerNacionalidad() const;
  string obtenerFechaNac() const;
  string obtenerTipoV() const;
  int obtenerDuracion() const;
  string obtenerECivil() const;
  string obtenerPaisResi() const;
  string obtenerMotivo() const;
  bool obtenerViajaSolo() const;
  string getTipoPersona() const;
  bool obtenerPaisRestringido() const;
  bool obtenerDocumentacionValida() const;
  void setGenero(bool gen);
  bool getGenero() const;
  void setRutaImagen(const QString &ruta);
  QString getRutaImagen() const;

  // Métodos modificadores
  void setNombreApellido(const std::string &nombreApellido_);
  void setNacionalidad(const std::string &nacionalidad_);
  void setFechaNac(const std::string &fechaNac_);
  void setTipoV(const std::string &tipoV_);
  void setECivil(const std::string &eCivil_);
  void setPaisResi(const std::string &paisResi_);
  void setMotivo(const std::string &motivo_);
  void setDuracion(int duracion_);
  void setViajaSolo(bool viajaSolo_);
  void setTipoPersona(const std::string &tipoPersona_);
  void setDocumentacionValida(bool docValida_);
  void setPaisRestringido(bool paisRestringido_);

  // Métodos de serialización
  void serialize(QDataStream &out) const;
  void deserialize(QDataStream &in);
};

#endif // PERSONA_H
