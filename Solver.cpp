//
//  solver.cpp
//  crc_solver
//
//  Created by Haowen Lin on 15/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//


#include "Solver.h"

std::map<string,Constraints*> myConstraints;
std::map<string,int> myAssignments;
int varNum = 0;
int constNum = 0;
vector<vector<string>> allConstaints;
std::map<string,int> myDomains;
vector<string> Names;


void solver(string inputfile,string outputfile)
{
    reader(inputfile);
    checkCrcProblem();
    solveProcess(varNum, constNum, myConstraints, myDomains, allConstaints, myAssignments);
    writer(outputfile);
    
}



void solveProcess(int varNum,int constNum,std::map<string,Constraints*> myConstraints,std::map<string,int> &Domains,vector<vector<string>> allConstaints,std::map<string,int> &myAssignments){
    
    srand(time(NULL));
    
    for (auto it = myAssignments.begin(); it != myAssignments.end(); ++it)
    {
        int domainNum = Domains.find(it->first)->second;
        it->second=(rand() % domainNum);
    }

    
    
    bool violates = true;
    while (violates)
    {
        printAssign(myAssignments);
        violates = false;
        for (int i=0; i< allConstaints.size(); i++)
        {
            vector<string> constraint = allConstaints.at(i);
            //if (constraint.at(0) ==0){
            string name1 = constraint.at(0);
            string name2 = constraint.at(1);
            int v1 = stoi(constraint.at(2));
            int v2 = stoi(constraint.at(3));
            int assignment1 = myAssignments.find(name1)->second;
            int assignment2 = myAssignments.find(name2)->second;
            if (assignment1==v1 && assignment2 == v2){
                violates = true;
                string key = getKey(name1,name2);
                auto it = myConstraints.find(key);
                if (it != myConstraints.end())
                {
                    vector<Dir> direction = it ->second->getDirPair(v1, v2);
                    Dir p = direction[rand() % 2];
                    
                    switch (p) {
                        case LF:
                            if (name1 == it->second->getFirstVarName()){
                                assignment2--;}
                            else{
                                assignment1--;}
                            break;
                        case RT:
                            if (name1 == it->second->getFirstVarName()){
                                assignment2++;}
                            else{
                                assignment1++;}
                            break;
                        case DN:
                            if (name1 == it->second->getFirstVarName()){
                                assignment1++;}
                            else{
                                assignment2++;}
                            break;
                        case UP:
                            if (name1 == it->second->getFirstVarName()){
                                assignment1--;}
                            else{
                                assignment2--;}
                            break;
                    }
                    myAssignments[name1] = assignment1;
                    myAssignments[name2] = assignment2;
                    
                    
                }else{
                    cerr << "Not created constraints matrix\n";
                }
                
            }
            //}
        }
        
        
        
    }// while vilolates
    cout <<"final ans: " << endl;
    printAssign(myAssignments);
}


void reader(std::string filename){
    
    std::ifstream myfile(filename);
    string line;
    if (myfile.is_open())
    {
        getline (myfile,line);
        varNum = stoi(line);
        for (int i=0; i<varNum;i++)
        {
            getline (myfile,line);
            vector<string> name_domain = split_str(line);
            myAssignments.insert({name_domain[0],INT_MAX});
            myDomains[name_domain[0]] = stoi(name_domain[1]);
            
        }
        getline (myfile,line);
        constNum = stoi(line);
        
        for (int i=0; i<constNum;i++)
        {
            getline (myfile,line);
            vector<string> name_constrains = split_str(line);
            allConstaints.push_back(name_constrains);
            string key = getKey(name_constrains[0], name_constrains[1]);
            auto it = myConstraints.find(key);
            if (it == myConstraints.end()){
                //it->second->setConstraint2(cur_constraint,z);
                int domain1 = myDomains.find(name_constrains[0])->second;
                int domain2 = myDomains.find(name_constrains[1])->second;
                
                Constraints* cur = new Constraints(name_constrains[0],name_constrains[1],domain1,domain2,1);
                myConstraints[key] = cur;
                
            }
            myConstraints.find(key)->second->setConstraint(stoi(name_constrains[2]),stoi(name_constrains[3]));
        }
        
    }else{
        cerr << "Input file is not open\n";
    
    }
    
    
    
    
}

void checkCrcProblem(){
    for (auto& constraints : myConstraints){
        constraints.second->checkRowConnected();
        constraints.second->checkSmooth();
    }
    
}

void writer(string filename){
    ofstream myfile;
    myfile.open (filename);
    for (auto& ass : myAssignments){
        myfile << ass.first <<" " << ass.second << "\n";
    }
    myfile.close();
}



