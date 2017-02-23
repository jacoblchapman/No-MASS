// Copyright 2015 Jacob Chapman

#include <limits.h>
#include "tests/Gen.h"
#include "SimulationConfig.h"
#include "Building_Appliances.h"
#include "gtest/gtest.h"

/*
struct appLargeStruct {
    std::string name;
    int id;
    std::vector<double> priority;
    std::vector<double> timeRequired
    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double cost;
    double epsilon = 0.1;   // probability of a random action selection
    double alpha = 0.3;     // learning rate
    double gamma = 0.1;     // discount factor (how soon do you care)
    bool update = false;
    std::vector<int> activities;
};
*/

class Test_Appliance_Group_Large : public ::testing::Test {
 protected:
    Appliance_Group_Large large;
    std::vector<appLargeStruct> AppliancesLarge;
    Contract_Negotiation local_negotiation;
    Contract_Negotiation neigh_negotiation;
    Contract_Negotiation global_negotiation;
    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Group_Large::SetUp() {
  SimulationConfig::reset();
  SimulationConfig::parseConfiguration(testFiles +
                                                  "DSM1house.xml");
  SimulationConfig::FileLargeAppliance = testFiles + "AppliancesLarge.xml";
  SimulationConfig::FolderSmallAppliance = testFiles + "SmallAppliances/";
  SimulationConfig::setStepCount(-1);
  Utility::setSeed(0);
  appLargeStruct a;
  a.name = "test_app_";
  a.id = 0;
  a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  AppliancesLarge.push_back(a);
}

void Test_Appliance_Group_Large::AfterConfiguration() {}

TEST_F(Test_Appliance_Group_Large, power) {

  large.setup(AppliancesLarge, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    SimulationConfig::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    large.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);
    large.reset();
  }
}


TEST_F(Test_Appliance_Group_Large, powerSupply) {

  large.setup(AppliancesLarge, 0, "test_building_");

  for(int i = 0; i < 10000; i++) {
    SimulationConfig::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    Contract c;
    c.supplied = 10;

    global_negotiation.submit(c);

    large.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    if(power > c.supplied){
        EXPECT_NEAR(recieved, c.supplied, 0.01);
    } else if(power > 0){
        EXPECT_NEAR(recieved, power, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }
      large.reset();
  }

}

TEST_F(Test_Appliance_Group_Large, Appliances) {
  for(int i = 0; i < 5; i++) {
    appLargeStruct a;
    a.name = "test_app_" + std::to_string(i+1) + "_";
    a.id = i + 1;
    a.priority = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    AppliancesLarge.push_back(a);
  }
  for(int build = 0; build < 40; build++) {
      large.setup(AppliancesLarge, build, "test_building_" + std::to_string(build) + "_");
  }
  for(int i = 0; i < 10000; i++) {
    SimulationConfig::step();
    large.step(&local_negotiation);
    double power = large.getPower();
    double supply = large.getSupply();
    double recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    local_negotiation.process();
    large.localNegotiation(local_negotiation);
    local_negotiation.clear();

    large.addGlobalContactsTo(&neigh_negotiation);
    neigh_negotiation.process();
    large.neighbourhoodNegotiation(neigh_negotiation);
    neigh_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    EXPECT_NEAR(recieved, 0, 0);

    Contract c;
    c.supplied = power;

    global_negotiation.submit(c);

    large.addGlobalContactsTo(&global_negotiation);
    global_negotiation.process();
    large.globalNegotiation(global_negotiation);
    global_negotiation.clear();
    power = large.getPower();
    supply = large.getSupply();
    recieved = large.getReceived();
    EXPECT_TRUE(power >= 0);
    EXPECT_TRUE(supply >= 0);
    EXPECT_NEAR(supply, 0, 0);
    if(power > 0){
        EXPECT_NEAR(recieved, c.supplied, 0.01);
    }else {
        EXPECT_NEAR(recieved, 0, 0.01);
    }
    large.reset();
  }

}
