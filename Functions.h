//
//  Functions.h
//  CRC_Problems_Solver
//
//  Created by Haowen Lin on 15/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#ifndef Functions_h
#define Functions_h
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;
std::vector<int> split(const std::string &line);
std::vector<string> split_str(const std::string &line);
void printAssign(std::map<string,int> &myAssignments);
string getKey(std::string name1,std::string name2);

#endif /* Functions_h */
