#ifndef VIBECHECK_H
#define VIBECHECK_H

#include <QDataStream>

class VibeCheck {
private:
  int reputation; // Replaces 'puntos'

public:
  VibeCheck();

  int getReputation() const;
  void setReputation(int rep);
  void increaseReputation(int amount);
  void decreaseReputation(int amount);

  // Serialization methods
  void serialize(QDataStream &out) const;
  void deserialize(QDataStream &in);
};

#endif // VIBECHECK_H
