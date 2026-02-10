#ifndef PERSONA_H
#define PERSONA_H

#include <QDataStream>
#include <QString>
#include <string>

using namespace std;

class Persona {
private:
  string name;
  string crew;
  string birthDate;
  string sessionGoal;
  string stance;
  string brand;
  string skateStyle;
  string deckCondition;
  int sessionDuration;
  bool hasHelmet;
  string role;
  bool hasValidID;
  bool isBannedCrew;
  bool gender; // true: male, false: female
  QString imagePath;

public:
  Persona();
  // Updated constructor
  Persona(const string &name_, const string &crew_, const string &birthDate_,
          const string &sessGoal, const string &stance_, const string &brand_,
          const string &style, const string &deckCond, int duration,
          bool helmet, const string &role_, bool validID, bool banned, bool gen,
          const QString &imgRuta);

  // Accessors
  string getName() const;
  string getCrew() const;
  string getBirthDate() const;
  string getSessionGoal() const;
  string getStance() const;
  string getBrand() const;
  string getSkateStyle() const;
  string getDeckCondition() const;
  int getSessionDuration() const;
  bool getHasHelmet() const;
  string getRole() const;
  bool getIsBannedCrew() const;
  bool getHasValidID() const;
  bool getGender() const;
  QString getImagePath() const;

  // Mutators
  void setName(const std::string &name_);
  void setCrew(const std::string &crew_);
  void setBirthDate(const std::string &birthDate_);
  void setSessionGoal(const std::string &sessGoal_);
  void setStance(const std::string &stance_);
  void setBrand(const std::string &brand_);
  void setSkateStyle(const std::string &style_);
  void setDeckCondition(const std::string &deckCond_);
  void setSessionDuration(int duration_);
  void setHasHelmet(bool helmet_);
  void setRole(const std::string &role_);
  void setHasValidID(bool validID);
  void setIsBannedCrew(bool banned_);
  void setGender(bool gen);
  void setImagePath(const QString &ruta);

  // Serialization methods
  void serialize(QDataStream &out) const;
  void deserialize(QDataStream &in);
};

#endif // PERSONA_H
