
#ifndef FACILITY_H
#define FACILITY_H

#include <iostream>

class Facility {
    public:
        Facility(int initial_id, std::pair<double, double> initial_coords, double initial_setup_cost, double initial_capacity) {
            id = initial_id;
            x = initial_coords.first;
            y = initial_coords.second;
            setup_cost = initial_setup_cost;
            capacity = initial_capacity;
        }

        int Id() {
            return id;
        }

        std::pair<double, double> Coords() {
            return std::make_pair(x, y);
        }

        double SetupCost() {
            return setup_cost;
        }

        double Capacity() {
            return capacity;
        }



    private:
        int id;
        double x, y;
        double setup_cost;
        double capacity;
};

#endif