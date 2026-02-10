#include "VibeCheck.h"

// Implementation of VibeCheck
VibeCheck::VibeCheck() : reputation(0) {}

int VibeCheck::getReputation() const { return reputation; }

void VibeCheck::setReputation(int rep) { reputation = rep; }

void VibeCheck::increaseReputation(int amount) { reputation += amount; }

void VibeCheck::decreaseReputation(int amount) { reputation -= amount; }

// Serialization methods
void VibeCheck::serialize(QDataStream &out) const { out << reputation; }
void VibeCheck::deserialize(QDataStream &in) { in >> reputation; }
