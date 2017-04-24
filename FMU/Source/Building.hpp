// Copyright 2015 Jacob Chapman

#ifndef BUILDING_H_
#define BUILDING_H_
#include <vector>
#include <string>
#include "Configuration.hpp"
#include "Occupant.hpp"
#include "Building_Zone.hpp"
#include "Building_Appliances.hpp"

 /**
  * @brief A Building
  * @details A Building
  */

class Building {
 public:
  Building();
  /**
   * [setup description]
   * @param buildingInput A ConfigStructBuilding containing all data needed for
   * the building as defined in a configuration file
   */
  void setup(const ConfigStructBuilding &buildingInput);
  void step();
  void postprocess();
  void preprocess();
  void postTimeStep();
  bool hasZone(const std::string& zoneName) const;
  double getPower() const;
  int getID() const;

  void stepAppliancesUse();
  void stepAppliancesUseBatteries(Contract_Negotiation * building_negotiation);

  void stepAppliancesNegotiationNeighbourhood(
                          const Contract_Negotiation & building_negotiation);

  void stepAppliancesNegotiation(
                          const Contract_Negotiation & building_negotiation);
  void addContactsTo(Contract_Negotiation * building_negotiation,
                    const bool battery);

  bool decisionBoolean(const double val1, const double val2) const;
  double decisionDoubleVec(const std::vector<double> & val,
                            const std::vector<double> & power,
                            const double currentState) const;

 private:
  void setOccupantGainsForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantWindowDecisionForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantLightDecisionForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantBlindDecisionForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantHeatDecisionsForZone(std::shared_ptr<Building_Zone> zone);
  void setOccupantCountForZone(std::shared_ptr<Building_Zone> zone);
  void setAppGainsForZone(std::shared_ptr<Building_Zone> zone);
  void setZones(const std::vector<std::shared_ptr<Building_Zone>> & zones);
  void buildingInteractions();

  int id;
  std::string name;
  std::vector<float> presenceProfile;
  std::vector<Occupant> population;
  std::vector<std::shared_ptr<Building_Zone>> zones;

  Building_Appliances appliances;
};

#endif  // BUILDING_H_