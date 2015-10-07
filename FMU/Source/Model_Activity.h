/*
 * File:   Model_Activity.h
 * Author: jake
 *
 * Created on September 17, 2013, 3:13 PM
 */

#ifndef MODEL_ACTIVITY_H
#define	MODEL_ACTIVITY_H

#include <vector>
#include <SimulationConfig.h>

class Model_Activity {
public:
    Model_Activity();
    std::vector<double> getAgentActivities(const int agentID);

private:
    void parseConfiguration(const std::string filename);
    double multinominalActivity(const double *p) const;
    void multinominalP(double p[4][7][24][10], const int agentID) const;

    std::vector<double> disaggregate(const int agentID) const;
    std::vector<double> multinominal(const int agentID) const;
    std::string getSeasonString(const int month) const;
    int getSeasonInt(const int month) const;
    std::string getDay(const int day) const;
    std::map<int, std::map<std::string, std::vector<double>>> dictionary;

};

#endif	/* MODEL_ACTIVITY_H */
