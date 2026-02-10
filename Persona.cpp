#include "Persona.h"

Persona::Persona() {}

Persona::Persona(const string &nomApe, const string &nac, const string &fecha,
                 const string &tipoVis, const string &estCivil,
                 const string &paisResidencia, const string &proposito, int dur,
                 bool solo, const string &tipoPers, bool docValida,
                 bool paisRestring, bool gen, const QString &imgRuta)
    : nombreApellido(nomApe), nacionalidad(nac), fechaNac(fecha),
      TipoV(tipoVis), ECivil(estCivil), paisResi(paisResidencia),
      motivo(proposito), duracion(dur), viajaSolo(solo), tipoPersona(tipoPers),
      documentacionValida(docValida), paisRestringido(paisRestring),
      genero(gen), rutaImagen(imgRuta) {}

// Métodos de acceso
string Persona::obtenerNombreApellido() const { return nombreApellido; }
string Persona::obtenerNacionalidad() const { return nacionalidad; }
string Persona::obtenerFechaNac() const { return fechaNac; }
string Persona::obtenerTipoV() const { return TipoV; }
int Persona::obtenerDuracion() const { return duracion; }
string Persona::obtenerECivil() const { return ECivil; }
string Persona::obtenerPaisResi() const { return paisResi; }
string Persona::obtenerMotivo() const { return motivo; }
bool Persona::obtenerViajaSolo() const { return viajaSolo; }
string Persona::getTipoPersona() const { return tipoPersona; }
bool Persona::obtenerPaisRestringido() const { return paisRestringido; }
bool Persona::obtenerDocumentacionValida() const { return documentacionValida; }
void Persona::setGenero(bool gen) { genero = gen; }
bool Persona::getGenero() const { return genero; }
void Persona::setRutaImagen(const QString &ruta) { rutaImagen = ruta; }
QString Persona::getRutaImagen() const { return rutaImagen; }

// Métodos modificadores
void Persona::setNombreApellido(const std::string &nombreApellido_) {
  nombreApellido = nombreApellido_;
}
void Persona::setNacionalidad(const std::string &nacionalidad_) {
  nacionalidad = nacionalidad_;
}
void Persona::setFechaNac(const std::string &fechaNac_) {
  fechaNac = fechaNac_;
}
void Persona::setTipoV(const std::string &tipoV_) { TipoV = tipoV_; }
void Persona::setECivil(const std::string &eCivil_) { ECivil = eCivil_; }
void Persona::setPaisResi(const std::string &paisResi_) {
  paisResi = paisResi_;
}
void Persona::setMotivo(const std::string &motivo_) { motivo = motivo_; }
void Persona::setDuracion(int duracion_) { duracion = duracion_; }
void Persona::setViajaSolo(bool viajaSolo_) { viajaSolo = viajaSolo_; }
void Persona::setTipoPersona(const std::string &tipoPersona_) {
  tipoPersona = tipoPersona_;
}
void Persona::setDocumentacionValida(bool docValida_) {
  documentacionValida = docValida_;
}
void Persona::setPaisRestringido(bool paisRestringido_) {
  paisRestringido = paisRestringido_;
}
// Métodos de serialización
void Persona::serialize(QDataStream &out) const {
  out << QString::fromStdString(nombreApellido)
      << QString::fromStdString(nacionalidad)
      << QString::fromStdString(fechaNac) << QString::fromStdString(TipoV)
      << QString::fromStdString(ECivil) << QString::fromStdString(paisResi)
      << QString::fromStdString(motivo) << duracion << viajaSolo
      << QString::fromStdString(tipoPersona) << documentacionValida
      << paisRestringido << genero << rutaImagen;
}
void Persona::deserialize(QDataStream &in) {
  QString nomApe, nac, fecha, tipoVis, estCivil, paisResidencia, proposito,
      tipoPers, imgRuta;
  in >> nomApe >> nac >> fecha >> tipoVis >> estCivil >> paisResidencia >>
      proposito >> duracion >> viajaSolo >> tipoPers >> documentacionValida >>
      paisRestringido >> genero >> imgRuta;

  nombreApellido = nomApe.toStdString();
  nacionalidad = nac.toStdString();
  fechaNac = fecha.toStdString();
  TipoV = tipoVis.toStdString();
  ECivil = estCivil.toStdString();
  paisResi = paisResidencia.toStdString();
  motivo = proposito.toStdString();
  tipoPersona = tipoPers.toStdString();
  rutaImagen = imgRuta;
}
