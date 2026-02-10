#include "Persona.h"

Persona::Persona() {}

Persona::Persona(const string &name_, const string &crew_,
                 const string &birthDate_, const string &sessGoal,
                 const string &stance_, const string &brand_,
                 const string &style, const string &deckCond, int duration,
                 bool helmet, const string &role_, bool validID, bool banned,
                 bool gen, const QString &imgRuta)
    : name(name_), crew(crew_), birthDate(birthDate_), sessionGoal(sessGoal),
      stance(stance_), brand(brand_), skateStyle(style),
      deckCondition(deckCond), sessionDuration(duration), hasHelmet(helmet),
      role(role_), hasValidID(validID), isBannedCrew(banned), gender(gen),
      imagePath(imgRuta) {}

// Accessors
string Persona::getName() const { return name; }
string Persona::getCrew() const { return crew; }
string Persona::getBirthDate() const { return birthDate; }
string Persona::getSessionGoal() const { return sessionGoal; }
string Persona::getStance() const { return stance; }
string Persona::getBrand() const { return brand; }
string Persona::getSkateStyle() const { return skateStyle; }
string Persona::getDeckCondition() const { return deckCondition; }
int Persona::getSessionDuration() const { return sessionDuration; }
bool Persona::getHasHelmet() const { return hasHelmet; }
string Persona::getRole() const { return role; }
bool Persona::getIsBannedCrew() const { return isBannedCrew; }
bool Persona::getHasValidID() const { return hasValidID; }
bool Persona::getGender() const { return gender; }
QString Persona::getImagePath() const { return imagePath; }

// Mutators
void Persona::setName(const std::string &name_) { name = name_; }
void Persona::setCrew(const std::string &crew_) { crew = crew_; }
void Persona::setBirthDate(const std::string &birthDate_) {
  birthDate = birthDate_;
}
void Persona::setSessionGoal(const std::string &sessGoal_) {
  sessionGoal = sessGoal_;
}
void Persona::setStance(const std::string &stance_) { stance = stance_; }
void Persona::setBrand(const std::string &brand_) { brand = brand_; }
void Persona::setSkateStyle(const std::string &style_) { skateStyle = style_; }
void Persona::setDeckCondition(const std::string &deckCond_) {
  deckCondition = deckCond_;
}
void Persona::setSessionDuration(int duration_) { sessionDuration = duration_; }
void Persona::setHasHelmet(bool helmet_) { hasHelmet = helmet_; }
void Persona::setRole(const std::string &role_) { role = role_; }
void Persona::setHasValidID(bool validID) { hasValidID = validID; }
void Persona::setIsBannedCrew(bool banned_) { isBannedCrew = banned_; }
void Persona::setGender(bool gen) { gender = gen; }
void Persona::setImagePath(const QString &ruta) { imagePath = ruta; }

// Serialization methods
void Persona::serialize(QDataStream &out) const {
  out << QString::fromStdString(name) << QString::fromStdString(crew)
      << QString::fromStdString(birthDate)
      << QString::fromStdString(sessionGoal) << QString::fromStdString(stance)
      << QString::fromStdString(brand) << QString::fromStdString(skateStyle)
      << QString::fromStdString(deckCondition) << sessionDuration << hasHelmet
      << QString::fromStdString(role) << hasValidID << isBannedCrew << gender
      << imagePath;
}

void Persona::deserialize(QDataStream &in) {
  QString name_, crew_, birthDate_, sessGoal, stance_, brand_, style, deckCond,
      role_, imgRuta;

  in >> name_ >> crew_ >> birthDate_ >> sessGoal >> stance_ >> brand_ >>
      style >> deckCond >> sessionDuration >> hasHelmet >> role_ >>
      hasValidID >> isBannedCrew >> gender >> imgRuta;

  name = name_.toStdString();
  crew = crew_.toStdString();
  birthDate = birthDate_.toStdString();
  sessionGoal = sessGoal.toStdString();
  stance = stance_.toStdString();
  brand = brand_.toStdString();
  skateStyle = style.toStdString();
  deckCondition = deckCond.toStdString();
  role = role_.toStdString();
  imagePath = imgRuta;
}
