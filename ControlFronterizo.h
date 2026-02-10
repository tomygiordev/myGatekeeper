#ifndef CONTROLFRONTERIZO_H
#define CONTROLFRONTERIZO_H

#include <QDataStream>

class ControlFronterizo {
private:
  int puntos;

public:
  ControlFronterizo();

  int getPuntos() const;
  void setPuntos(int pts);
  void incrementarPuntos(int pts);
  void decrementarPuntos(int pts);

  // Métodos de serialización
  void serialize(QDataStream &out) const;
  void deserialize(QDataStream &in);
};

#endif // CONTROLFRONTERIZO_H
