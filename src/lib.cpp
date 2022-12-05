#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "customer.h"
#include "facility.h"
#include "quicksort.h"

#define TIME_MAX 60
#define START_TEMP_SCALAR 300
#define TEMP_MIN 1

void InputMapper(std::ifstream& fin, std::vector<Facility>& facilities, std::vector<Customer>& customers) {
    int num_facilities = 0;
    int num_customers = 0;

    facilities = std::vector<Facility>();
    customers = std::vector<Customer>();

    fin >> num_facilities;
    fin >> num_customers;

    for (int i = 0; i < num_facilities; i++) { 
        double cur_setup_cost = 0.0f;
        double cur_capacity_cap = 0.0f;
        double cur_x = 0.0f;
        double cur_y = 0.0f;

        fin >> cur_setup_cost;
        fin >> cur_capacity_cap;
        fin >> cur_x;
        fin >> cur_y;

        facilities.push_back(Facility(i, cur_setup_cost, cur_capacity_cap, cur_x, cur_y));
    }

    for (int i = 0; i < num_customers; i++) {
        double cur_demand = 0.0f;
        double cur_x = 0.0f;
        double cur_y = 0.0f;
        
        fin >> cur_demand;
        fin >> cur_x;
        fin >> cur_y;

        customers.push_back(Customer(i, cur_demand, cur_x, cur_y));
    }
}


// Get the index of the minimum element
template<typename T>
int GetMinIndex(std::vector<T>& vect) {
    int index = 0;
    T min_result = vect[0];

    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] < min_result) {
            index = i;
            min_result = vect[i];
        }
    }
    return index;
}

double ScoreResult(std::vector<Facility>& facilities, std::vector<Customer>& customers) {
    double result = 0.0f;
    for (int i = 0; i < facilities.size(); i++) {
        result += facilities[i].GetScore(customers);
    }
    return result;
}

void PickClosestFacility(Customer& cur_cust, std::vector<Facility>& facilities) {
    int facility_index = -1;
    std::vector<double> facility_distances;
    std::vector<int> facility_ids;
    for (int i = 0; i < facilities.size(); i++) {
        std::pair<double, double> facility_coords = facilities[i].Coords();
        double facility_x = facility_coords.first;
        double facility_y = facility_coords.second;
        double current_dist = cur_cust.GetDistTo(facility_x, facility_y);
        facility_distances.push_back(current_dist);
        facility_ids.push_back(facilities[i].Id());
    }
    QuickSort2<double, int>::Sort(facility_distances, facility_ids);

    for (int i = 0; i < facility_distances.size(); i++) {
        int facility_id = facility_ids[i];
        if (facilities[facility_id].AssignCustomer(cur_cust)) {
            break;
        }
    }
}

double GetGreedySolution(std::vector<Facility>& facilities, std::vector<Customer>& customers) {
    for (int i = 0; i < customers.size(); i++) {
        PickClosestFacility(customers[i], facilities);
    }
    return ScoreResult(facilities, customers);
}

void MoveCustomer(std::vector<Facility>& facilities, std::vector<Customer>& customers, int cust_index) {
    
    int old_facility_id = customers[cust_index].GetFacilityId();
    int new_facility_id = old_facility_id;

    while (new_facility_id == old_facility_id)
        new_facility_id = rand() % facilities.size();

    if (old_facility_id != -1)
        facilities[old_facility_id].RemoveCustomer(customers[cust_index]);
    if (facilities[new_facility_id].AssignCustomer(customers[cust_index])) {
        return;
    } else {
        while(!facilities[new_facility_id].AssignCustomer(customers[cust_index])) {
            int remove_id = facilities[new_facility_id].GetCustomerIds()[0];
            facilities[new_facility_id].RemoveCustomer(customers[remove_id]);
            MoveCustomer(facilities, customers, remove_id);
        }
        return;
    }
}

void MoveCustomers(std::vector<Facility>& facilities, std::vector<Customer>& customers) {
    int cust_id = rand() % customers.size();
    MoveCustomer(facilities, customers, cust_id);
}

// Perform simulated annealing
double Anneal(std::vector<Facility>& facilities, std::vector<Customer>& customers, int cur_num_changes) {
    
    double current_best = ScoreResult(facilities, customers);
    double pre_annealing_check = ScoreResult(facilities, customers);
    // Simulated Annealing
    // Initialize temperature
    double initial_temp = (START_TEMP_SCALAR * START_TEMP_SCALAR) / double(facilities.size());
    double temp = double(initial_temp);
    
    // Start simulated annealing
    int iteration = 1;
    while (temp > TEMP_MIN) {
        
        std::vector<Facility> annealing_list = std::vector<Facility>(facilities);
        std::vector<Customer> annealing_customers = std::vector<Customer>(customers);

        // Make a random change
        // Relocate the specified number of cities in the order(in the vector, obviously)
        for (int i = 0; i < cur_num_changes; i++) {
            MoveCustomers(annealing_list, annealing_customers);
        }

        double check = ScoreResult(annealing_list, annealing_customers);

        double take_probability = exp(-1 * abs(check - current_best) / (temp + 1));
        double random = double(rand()) / double(RAND_MAX);
        if (check < current_best || random < take_probability) {
            current_best = check;
            facilities = std::vector<Facility>(annealing_list);
            customers = std::vector<Customer>(annealing_customers);
        }
        temp = initial_temp / double(iteration);
        iteration++;

    }
    return current_best;
}

double CalculateFacilityLocations(std::vector<Facility>& facilities, std::vector<Customer>& customers) {
    srand(time(NULL));
    // Get a greedy solution
    double greedy_result = GetGreedySolution(facilities, customers);

    std::vector< std::vector<Facility> > facility_annealing_results;
    std::vector< std::vector<Customer> > customer_annealing_results;
    std::vector<double> annealing_results;

    facility_annealing_results.push_back(facilities);
    customer_annealing_results.push_back(customers);
    annealing_results.push_back(greedy_result);

    for (int i = 0; i < 20; i++) {
        std::vector<Facility> cur_facilities = std::vector<Facility>(facilities);
        std::vector<Customer> cur_customers = std::vector<Customer>(customers);

        double cur_result = Anneal(cur_facilities, cur_customers, i);

        annealing_results.push_back(cur_result);
        customer_annealing_results.push_back(cur_customers);
        facility_annealing_results.push_back(cur_facilities);
    }

    int min_index = GetMinIndex<double>(annealing_results);
    facilities = std::vector<Facility>(facility_annealing_results[min_index]);
    customers = std::vector<Customer>(customer_annealing_results[min_index]);

    return ScoreResult(facilities, customers);
}