#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <float.h>

//#include "facility.h"
class Customer {
    public:
        Customer(int initial_id, double initial_demand, double initial_x, double initial_y) {
            id = initial_id;
            demand = initial_demand;
            x = initial_x;
            y = initial_y;
            facility_id = -1;
        }

        int Id() const {
            return id;
        }

        double Demand() const {
            return demand;
        }

        std::pair<double, double> Coords() const {
            return std::make_pair(x, y);
        }

        int GetFacilityId() {
            return facility_id;
        }

        void AssignToFacility(int new_facility_id) {
            facility_id = new_facility_id;
        }

        void UnassignFacility() {
            facility_id = -1;
        }

        bool CheckAssigned() {
            return facility_id != -1;
        }

        double GetDistTo(double to_x, double to_y) const {
            return sqrt(pow(x - to_x, 2) + pow(y - to_y, 2));
        }
    private:
        int id;
        double demand;
        double x, y;
        int facility_id;
};

#endif