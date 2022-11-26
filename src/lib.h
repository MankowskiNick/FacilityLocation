#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <vector>
#include <fstream>
#include "facility.h"
#include "customer.h"

void InputMapper(std::ifstream& fin, std::vector<Facility>& facilities, std::vector<Customer>& customers);
double CalculateFacilityLocations(std::vector<Facility>& facilities, std::vector<Customer>& customers);

#endif