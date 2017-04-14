// Copyright 2016 Jacob Chapman

#ifndef APPLIANCE_LARGE_LEARNING_H_
#define APPLIANCE_LARGE_LEARNING_H_

#include <vector>
#include <queue>
#include <string>
#include "QLearning_Appliance.h"
#include "Appliance_Large.h"


struct profileStruct {
    std::vector<double> power;
    int startTime = -1;
    double maxTimeRequired = 0;
    double cost = 0;
    bool isLearningPeriod = false;
    int nonLearningStep = 0;
    unsigned int learningStep = 0;
    int requestedTime;
};


/**
 * @brief Large appliances learning class
 * @details This will handle the appliance learning model for profile shifting
 */

class Appliance_Large_Learning : public Appliance_Large {
 public:
  Appliance_Large_Learning();
  void setup(applianceStruct a);
  void step();
  void postprocess();
  double getRequiredTime(int hourOfDay) const;
  void addToCost(const double cost);
  void setHoulyTimeRequired(const std::vector<double> & houlyTimeRequired);
  void setFile(std::string file);
  bool isModelOn();

protected:
  virtual double getPowerAt(const int timestep);
  virtual void calculateProfile();

  std::queue<profileStruct> powerProfile;
  std::string file;
  std::vector<double> profileCSV;

 private:

  void beforeClear();
  int databaseIDactual;
  std::vector<double> houlyTimeRequired;
  QLearning_Appliance qLearning;

  void stepApplianceOffAndNotLearning(const int hourOfTheDay);

  void calculateLearntStartTime();
  void startLearningPeriod(const int hourOfTheDay);
  void stopLearningPeriod(const int hourOfTheDay);
  void saveActualProfile();
  double calculateReward();
  bool learnStepLessThanProfile() const;
  void eraseFirstPowerProfile();
};

#endif  // APPLIANCE_LARGE_LEARNING_H_
