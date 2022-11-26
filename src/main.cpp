// Nick Mankowski
// CS 490 - Fall 2022
// Prof. Sean McCulloch
// Facility Location

// Instructions to run
//  1) Build the program
//  2) Run the executable generated with the filenames you 
//     would like to generate outputs for.  For example, 
//     "./FacilityLocation fl_3_1 fl_16_1 fl_16_2"
//     would generate output files for the the three listed input files.
//  3) The output files will match the names of the input files in the same directory, 
//     but will have "OUTPUT_" before the input file name.  For example, 
//     "./FacilityLocation fl_3_1 fl_16_1 fl_16_2"
//     will generate OUTPUT_fl_3_1, OUTPUT_fl_16_1, OUTPUT_fl_16_2

#include <iostream>
#include <fstream>

#include <time.h>

#include "iolib.h"
#include "lib.h"
#include "facility.h"
#include "customer.h"

// Take arguments from command line
int main(int argc, char* args[]) {

    // Look at each file listed in the command line arguments(did this so I don't have to keep running again and typing different file names)
    // Argument 1 is the "./FacilityLocation" command, so we have to start looking at args[1] to get file names
    for (int fileIndex = 1; fileIndex < argc; fileIndex++) {    

        // Declare a file input stream & file output stream
        std::ifstream fin;
        std::ofstream fout;

        // Assign a name to the output file, it will match the input file  
        // but with "OUTPUT_" at the beginning as to not overwrite it.
        // We need to find the file name WITHOUT the directory
        std::string inputFile, outputFile;

        // Supply with the file name from console, and the inputFile & outputFile to set
        SetIOFiles(args[fileIndex], inputFile, outputFile); 

        // Open the input file
        fin.open(inputFile.data());

        // Assert that the input file successfully opened
        AssertInputFileOpenSuccess(fin.fail(), inputFile);

        // Map input data to proper data structures
        std::vector<Facility> facilities;
        std::vector<Customer> customers;
        InputMapper(fin, facilities, customers);
        

        // Find optimal facility locations
        double result = CalculateFacilityLocations(facilities, customers);
        std::cout << inputFile << "\n     -result=" << result << "\n\n";

        // Open the output file
        fout.open(outputFile.data());

        // TODO: Output data
        //std::string output = "";
        fout << result << "\n";
        for (int i = 0; i < customers.size(); i++) {
            fout << customers[i].GetFacilityId() << " ";
        }
        fout << "\n";

        // Close the input & output files
        fin.close();
        fout.close();
    }

    return 0;
}