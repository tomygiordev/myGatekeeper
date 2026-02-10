#include "ControlFronterizo.h"

// Implementación de ControlFronterizo
ControlFronterizo::ControlFronterizo() : puntos(0) {}
int ControlFronterizo::getPuntos() const { return puntos; }
void ControlFronterizo::setPuntos(int pts) { puntos = pts; }
void ControlFronterizo::incrementarPuntos(int pts) { puntos += pts; }
void ControlFronterizo::decrementarPuntos(int pts) { puntos -= pts; }
// Métodos de serialización
void ControlFronterizo::serialize(QDataStream &out) const { out << puntos; }
void ControlFronterizo::deserialize(QDataStream &in) { in >> puntos; }
