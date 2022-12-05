#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>

using namespace std;
struct Facility{
  int number;
  double open_cost;
  double cap;
  double x_loc;
  double y_loc;
};

struct Customer{
  int number;
  double demand;
  double x_loc;
  double y_loc;
};


double distance(double x1, double y1, double x2, double y2){
  return sqrt(pow (x1-x2,2.0) + pow (y1-y2,2.0));
}


bool Is_Legal_Solution(const vector<Customer>& cust_list, const vector<Facility>& fac_list, const vector<int>& assigned_custs){

  vector<double> demands(fac_list.size(),0);
  for(int i = 0; i < assigned_custs.size(); i++){
    int my_vehicle = assigned_custs[i];
    demands[my_vehicle] += cust_list[i].demand;
  }
  bool ok = true;
  for(int i = 0; i < fac_list.size(); i++){
    if(demands[i] > fac_list[i].cap){
      cout << "Facility " << i << " is over capacity!" << endl;
      ok = false;;
    }
  }
  return ok;
}

double Cost(const vector<Facility>& fac_list, const vector<Customer>& cust_list, 
	    const vector<int> & assigned_custs){
  double total = 0;
  vector<bool> open_facs(fac_list.size(), false);
  for(int i = 0; i < assigned_custs.size(); i++){
    if(assigned_custs[i] == -1)
      return INFINITY;
    int cur_fac = assigned_custs[i];
    if(!open_facs[cur_fac]){
      open_facs[cur_fac] = true;
      total=total+fac_list[cur_fac].open_cost;
    }
    
    total = total + distance(cust_list[i].x_loc, cust_list[i].y_loc, 
			     fac_list[assigned_custs[i]].x_loc, fac_list[assigned_custs[i]].y_loc);
  }
  return total;
}

void Verify(string input_filename, string output_filename) {
  ifstream input, output;
  input.open(input_filename.data());
  output.open(output_filename.data());
  assert(input.is_open());
  assert(output.is_open());
  int num_facilities, num_customers;
  input >> num_facilities;
  input >> num_customers;

  vector<Facility> fac_list(num_facilities);
  vector<double> orig_caps(num_facilities);
  for(int i = 0; i < num_facilities; i++){
    fac_list[i].number = i;
    input >> fac_list[i].open_cost;
    input >> fac_list[i].cap;
    input >> fac_list[i].x_loc;
    input >> fac_list[i].y_loc;
    orig_caps[i] = fac_list[i].cap;
  }

  vector<Customer> cust_list(num_customers);
  for(int i = 0; i < num_customers; i++){
    cust_list[i].number = i;
    input >> cust_list[i].demand;
    input >> cust_list[i].x_loc;
    input >> cust_list[i].y_loc;
  }

  double score;
  output >> score;
  vector<int> customer_assignments(num_customers);
  for(int i = 0; i < num_customers; i++)
    output >> customer_assignments[i];
  if(Is_Legal_Solution(cust_list, fac_list, customer_assignments))
    cout << "Customer demands met" << endl;
  else
    cout << "Customer demands not met" << endl;
  double actual_cost = Cost(fac_list, cust_list, customer_assignments);
  cout << "Claimed cost: " << score << endl;
  cout << "Actual cost: " << actual_cost << endl;
}


int main(int argc, char* args[]){
    for (int i = 1; i < argc; i+=2) {    
      std::cout << "Checking file - " << string(args[i]) << "\n";
      Verify(string(args[i]), string(args[i + 1]));
      std::cout << "\n\n\n";
    }
}
