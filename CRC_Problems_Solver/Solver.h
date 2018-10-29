//
//  Solver.h
//  CRC_Problems_Solver
//
//  Created by Haowen Lin on 15/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#ifndef Solver_h
#define Solver_h
#include <stdio.h>
#include "Constraints.hpp"
#include "Functions.h"
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

void reader(std::string filename); // read in the txt file and parse it into constraints objects
void checkCrcProblem(); // check whether it is crc problems and smooth
void writer(string filename,long long readtime, long long solvetime);// write the output into outputfiles
void solveProcess(int varNum,int constNum,std::map<string,Constraints*> myConstraints,std::map<string,int> &Domains,vector<vector<string>> allConstaints,std::map<string,int> &myAssignments); //  randomized algorithms that solve the process
void solver(string inputfile,string outputfile);

void writeRandomAssign();
void readRandomAssign();

#endif /* Solver_h */
