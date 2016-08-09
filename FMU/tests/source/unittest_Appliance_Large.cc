// Copyright 2015 Jacob Chapman
#include <limits.h>
#include <vector>
#include "Gen.h"

#include "SimulationConfig.h"
#include "Appliance_Large.h"

#include "gtest/gtest.h"

class Test_Appliance_Large_Usage : public ::testing::Test {
 protected:
    Appliance_Large al;

    virtual void SetUp();
    virtual void AfterConfiguration();
};

void Test_Appliance_Large_Usage::SetUp() {
  SimulationConfig::parseConfiguration("SimulationConfig.xml");
  SimulationConfig::stepCount = -1;
  al.setID(1);
  al.setup();
}

void Test_Appliance_Large_Usage::AfterConfiguration() {
}

TEST_F(Test_Appliance_Large_Usage, power) {
  SimulationConfig::step();
  al.step();
  ASSERT_FALSE(al.isOn());
  ASSERT_FALSE(al.powerAt(0));
  std::vector<int> v = {8};
  for (int i = 1; i < 100000; i++) {
    SimulationConfig::step();
    al.hasActivities(v);
    al.step();
    if (al.isOn()) {
      ASSERT_GT(al.powerAt(i), 0);
    } else {
      ASSERT_EQ(al.powerAt(i), 0);
    }
  }
}