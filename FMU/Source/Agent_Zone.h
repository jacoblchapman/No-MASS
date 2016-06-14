/*
 * File:   Agent.h
 * Author: jake
 *
 * Created on September 15, 2013, 4:37 PM
 */

#ifndef AGENT_ZONE_H
#define	AGENT_ZONE_H
#include <unordered_map>
#include <string>
#include <deque>
#include <memory>

#include "Model_Activity.h"
#include "Model_Presence.h"
#include "StateMachine.h"
#include "State.h"

#include "Agent_Action_Window.h"
#include "Agent_Action_Window_Learning.h"
#include "Agent_Action_Lights.h"
#include "Agent_Action_HeatingSetPoints_Learning.h"
#include "Agent_Action_Appliance.h"

#include "Agent_Action_Shades.h"
#include "Agent_Action_Heat_Gains.h"
/**
 * @brief The Agent understand of a zone
 * @details Contains all information about the occupants and there associated interactions with a zone
 */

class Agent_Zone {
public:
    Agent_Zone();
    void setup(const Building_Zone & buldingZone, int agentid, const agentStruct &agent);
    void step(const Building_Zone& zone, const Building_Zone& zonePrevious,
              const std::vector<double> &activities);

    void actionStep(int action, const Building_Zone &zone, bool inZone,
                    bool preZone, const std::vector<double> &activities);
    void postTimeStep();
    void postprocess();

    void setClo(double clo);
    void setMetabolicRate(double metabolicRate);

    bool getDesiredWindowState() const;
    bool getDesiredLightState() const;
    int getId() const;
    int getDesiredWindowDuration() const;
    double getDesiredShadeState() const;
    double getDesiredAppliance() const;
    double getDesiredHeatingSetPoint() const;
    double getPMV() const;
    double getHeatgains() const;


    bool isActionWindow() const;
    bool isActionLights() const;
    bool isActionShades() const;
    bool isActionHeatGains() const;
    bool isActionLearning() const;
    bool isActionAppliance() const;

private:

    bool isInBuilding() const;

    int id;
    bool ActionWindow;
    bool ActionLights;
    bool ActionShades;
    bool ActionHeatGains;
    bool ActionLearning;
    bool ActionAppliance;
    bool desiredLightState;
    bool desiredWindowState;
    double desiredShadeState;
    double desiredHeatingSetPoint;
    double desiredApplianceState;
    double heatgains;
    double clo;
    double metabolicRate;
    double pmv;
    double ppd;
    double previous_pmv;
    std::vector<int> availableActions;
    Agent_Action_Window aaw;
    Agent_Action_Window_Learning aawLearn;
    Agent_Action_Lights aal;
    Agent_Action_Shades aas;
    Agent_Action_Heat_Gains aahg;
    Agent_Action_HeatingSetPoints_Learning aalearn;
    Agent_Action_Appliance aaa;

    bool BDIEnabled = false;
};

#endif	/* AGENT_ZONE_H */