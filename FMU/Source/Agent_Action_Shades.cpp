// Copyright 2015 Jacob Chapman

#include <iostream>
#include <vector>
#include "DataStore.h"
#include "Agent_Action_Shades.h"
#include "SimulationConfig.h"

Agent_Action_Shades::Agent_Action_Shades() {}

void Agent_Action_Shades::setClosedDuringWashing(
  bool ShadeClosedDuringWashing) {
  this->ShadeClosedDuringWashing = ShadeClosedDuringWashing;
}

void Agent_Action_Shades::setClosedDuringSleep(bool ShadeClosedDuringSleep) {
  this->ShadeClosedDuringSleep = ShadeClosedDuringSleep;
}

void Agent_Action_Shades::setup(int shadeID) {
    shadeStruct ws = SimulationConfig::shades.at(shadeID);
    m_blindUsage.setFullVars(ws.afullraise, ws.boutfullraise, ws.bsfullraise,
                  ws.bsfulllower, ws.boutfulllower, ws.afulllower);
    m_blindUsage.setDurationVars(ws.aSFlower, ws.bSFlower, ws.shapelower);
    m_blindUsage.setArrivalVars(ws.a01arr, ws.b01inarr, ws.b01sarr, ws.a10arr,
                  ws.b10inarr, ws.b10sarr);
    m_blindUsage.setInterVars(ws.a01int, ws.b01inint, ws.b01sint, ws.a10int,
                  ws.b10inint, ws.b10sint);
}

void Agent_Action_Shades::step(const Building_Zone& zone, const bool inZone,
    const bool previouslyInZone, const std::vector<double> & activities) {
  double shadingFraction = zone.getBlindState();
  // we take the previous timestep shading state to compute Lumint
  // Evg: Outdoor illuminance in the horizontal plane without obstructions (lux)
  float Evg =
    DataStore::getValue("EnvironmentSiteExteriorHorizontalSkyIlluminance");
  // Lumint: Indoor illuminance next to window (lux)
  float Lumint = zone.getDaylightingReferencePoint1Illuminance();

  if (inZone && !previouslyInZone) {
    shadingFraction = m_blindUsage.arrival(shadingFraction, Lumint, Evg);
  } else {
    shadingFraction = m_blindUsage.departure(shadingFraction, Lumint, Evg);
  }

  int stepCount = SimulationConfig::getStepCount();
  if (ShadeClosedDuringSleep && activities.at(stepCount) == 0) {
      shadingFraction = 0;
  }
  if (ShadeClosedDuringWashing && activities.at(stepCount) == 6) {
      shadingFraction = 0;
  }
  result = shadingFraction;
}
