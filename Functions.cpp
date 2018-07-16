//
//  Funtions.cpp
//  crc_solver
//
//  Created by Haowen Lin on 15/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#include "Functions.h"

// this cpp helps parse input files

std::vector<int> split(const std::string &line) {
    std::vector<int> elems;
    std:: string s = line;
    std::string token;
    std::string delimiter = ",";
    size_t pos = 0;
    
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        elems.push_back(stoi(token));
        s.erase(0, pos + delimiter.length());
    }
    elems.push_back(stoi(s));
    return elems;
    
}


std::vector<string> split_str(const std::string &line) {
    std::vector<string> elems;
    std:: string s = line;
    std::string token;
    std::string delimiter = ",";
    size_t pos = 0;
    
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        elems.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    elems.push_back(s);
    return elems;
    
}


void printAssign(std::map<string,int> &myAssignments){
    for (auto it: myAssignments)
        cout << it.first <<" " << it.second <<"\n";
    cout <<endl;
}


string getKey(std::string name1,std::string name2){
    string small = (name1 < name2) ? name1: name2;
    string big = (name1 >= name2) ? name1: name2;
    string ans = small+','+big;
    return ans;
    
}
