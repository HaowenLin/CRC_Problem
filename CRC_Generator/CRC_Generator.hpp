//
//  CRC_Instance_Generator.hpp
//  CRC_Problems_Solver
//
//  Created by Haowen Lin on 18/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#ifndef CRC_Generator_hpp
#define CRC_Generator_hpp

#include <stdio.h>
#include <map>
#include <set>
#include <random>
#include <algorithm>
#include <fstream>
#include "../CRC_Problems_Solver/Constraints.hpp"
using namespace std;


class CRC_Generator{
    
private:
    int varNum;
    int consNum;
    int domainSize;
    int constOutputNum = 0; // store constraint number for output 
    string filename ;
    set<pair <int,int>> signalMatrix; // store which two variables to pick
    std::map<string,int> myDomains;   // store random generate domains
    std::map<string,Constraints*> myConstaintStore; // store constaints that randomly generated
    std::map<string,int> assignment;// random assignment at first

    
    
public:
    
    CRC_Generator(int var, int constNum, int domainSize,string outputfile);
    void crc_generator_wrapper();
    void GenerateRandomDomain(int range); // Generate random domains
    void PickConstaint(); // generate constaint
    void GenerateConstrain(string X_i, string X_j, int domain1, int domain2);
    void OverriteEntire(Constraints* cur); // overrtite entire row/col with certain probability
    void GenerateOutput(string filename); // generate output file
    void OutputMatrix(string filename);
    void GenerateRandomAssignment();
    
    
};
#endif /* CRC_Instance_Generator_hpp */
