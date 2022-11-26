
#ifndef FACILITY_H
#define FACILITY_H

#include <iostream>
#include <math.h>

#include "customer.h"
//class Customer;

class Facility {
    public:
        Facility(int initial_id, double initial_setup_cost, double initial_capacity, double initial_x, double initial_y) {
            id = initial_id;
            x = initial_x;
            y = initial_y;
            setup_cost = initial_setup_cost;
            max_capacity = initial_capacity;
            capacity_left = initial_capacity;
        }

        int Id() const {
            return id;
        }

        std::pair<double, double> Coords() const {
            return std::make_pair(x, y);
        }

        double SetupCost() const {
            return setup_cost;
        }

        double Capacity() const {
            return capacity_left;
        }

        double GetScore(const std::vector<Customer>& customers) const {
            double result = 0.0f;
            if (customer_ids.size() > 0) {
                result += setup_cost;
                for (int i = 0; i < customer_ids.size(); i++) {
                    int customer_id = customer_ids[i];
                    double customer_x, customer_y;
                    for (int j = 0; j < customers.size(); j++) {
                        if (customer_id == customers[j].Id()) {
                            std::pair<double, double> coords = customers[j].Coords();
                            customer_x = coords.first;
                            customer_y = coords.second;
                        }
                    }
                    result += GetDistTo(customer_x, customer_y);
                }
            }
            return result;
        }

        bool AssignCustomer(Customer& new_cust) {
            if (new_cust.Demand() <= capacity_left) {
                new_cust.AssignToFacility(Id());
                customer_ids.push_back(new_cust.Id());
                capacity_left -= new_cust.Demand();
                return true;
            }
            return false;
        }

        bool RemoveCustomer(Customer& remove_cust) {        
            // Get the id of the customer to remove
            int remove_cust_id = remove_cust.Id();

            // Default item index, since -1 is impossible as a vect position this will indicate an error
            int item_index = -1;
            int list_size = customer_ids.size();
            for (int i = 0; i < list_size; i++) {

                // If we've found the item we need to remove it by shifting all of 
                // the elements to the right and then decrease vector size by 1.
                if (customer_ids[i] == remove_cust_id && item_index == -1) {
                    item_index = i;
                    for (int j = i; j < list_size; j++) {
                        customer_ids[j] = customer_ids[j + 1];
                    }
                    list_size--;
                    customer_ids.resize(list_size);
                }
            }
            
            // If item_index is still -1, the element wasn't found so we should return an error.
            if (item_index != -1) {
                remove_cust.UnassignFacility();
                capacity_left += remove_cust.Demand();
                return true;
            }
            return false;
        }

        bool operator==(Facility facility2) {
            return Id() == facility2.Id();
        }
        bool operator>=(Facility facility2) {
            return Capacity() >= facility2.Capacity();
        }
        bool operator>(Facility facility2) {
            return Capacity() > facility2.Capacity();
        }
        bool operator<=(Facility facility2) {
            return Capacity() <= facility2.Capacity();
        }
        bool operator<(Facility facility2) {
            return Capacity() < facility2.Capacity();
        }

        void SeedCustomerList(std::vector<Customer>& customers) {
            for (int i = 0; i < customers.size(); i++) {
                if (!customers[i].CheckAssigned() && capacity_left - customers[i].Demand() >= 0) {
                    customers[i].AssignToFacility(Id());
                    AssignCustomer(customers[i]);
                }
            }
        }

    private:
        int id;
        double x, y;
        double setup_cost;
        double max_capacity;
        double capacity_left;


        std::vector<int> customer_ids;

        double GetDistTo(double to_x, double to_y) const {
            return sqrt(pow(x - to_x, 2) + pow(y - to_y, 2));
        }
};

#endif