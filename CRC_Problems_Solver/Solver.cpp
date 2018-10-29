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
    std::cout << "read file " << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    reader(inputfile);
    std::chrono::steady_clock::time_point end1= std::chrono::steady_clock::now();
    
    auto readtime = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin).count();
    //readtime = readtime / 1000000.0;
    std::cout << "Reading Time difference = " << readtime <<std::endl;
    checkCrcProblem();
    
    solveProcess(varNum, constNum, myConstraints, myDomains, allConstaints, myAssignments);
    std::chrono::steady_clock::time_point end2= std::chrono::steady_clock::now();
    
    auto solvertime = std::chrono::duration_cast<std::chrono::microseconds>(end2 - end1).count();
    //solvertime /= 1000000.0;
    std::cout << "Solving Time difference = " << solvertime <<std::endl;
    
    
    writer(outputfile,readtime,solvertime);
    
}


void writeRandomAssign()
{
    ofstream myfile;
    myfile.open ("/Users/haowenlin/Documents/usc/大三生活/大三下/research/cstheory/code/CRC_Generator_Solver/CRC_Generator_Solver/Output/RandomAssign.txt");
    for (auto& ass : myAssignments){
        myfile << ass.first <<"," << ass.second << "\n";
    }
    myfile.close();
    
}

void readRandomAssign()
{
    string filename = "/Users/haowenlin/Documents/usc/大三生活/大三下/research/cstheory/code/CRC_Generator_Solver/CRC_Generator_Solver/Output/RandomAssign.txt";
    std::ifstream myfile(filename);
    string line;
    if (myfile.is_open())
    {
        int var = 100;
        for (int i=0; i<var;i++)
        {
            getline (myfile,line);
            vector<string> name_domain = split_str(line);
            
            
            auto temp =myAssignments.find(name_domain[0]);
            temp->second =stoi(name_domain[1]);
            //cout << name_domain[1];
            //myAssignments.insert({name_domain[0],stoi(name_domain[1])});
            
        }
        
    }else{
        cerr << "Input file is not open\n";
        
    }
    
    
}



void solveProcess(int varNum,int constNum,std::map<string,Constraints*> myConstraints,std::map<string,int> &Domains,vector<vector<string>> allConstaints,std::map<string,int> &myAssignments){
    
    srand(time(NULL));
    
    
    for (auto it = myAssignments.begin(); it != myAssignments.end(); ++it)
    {
        int domainNum = Domains.find(it->first)->second;
        it->second=(rand() % domainNum);
    }
     
    readRandomAssign();
    //printAssign(myAssignments);
    

    //writeRandomAssign();
    
    
    
    bool violates = true;
    while (violates)
    {
        auto it = myAssignments.begin();
        //cout <<it->first <<" " <<it->second <<endl;
        //printAssign(myAssignments);
        violates = false;
        for (int i=0; i< allConstaints.size(); i++)
        {
            vector<string> constraint = allConstaints.at(i);
            string name1 = constraint.at(0);
            string name2 = constraint.at(1);
            int v1 = stoi(constraint.at(2));
            int v2 = stoi(constraint.at(3));
            int assignment1 = myAssignments.find(name1)->second;
            int assignment2 = myAssignments.find(name2)->second;
            if (assignment1==v1 && assignment2 == v2){
                violates = true;
                cout <<name1 <<" " <<name2 <<" " << v1 <<" " <<v2<<endl;
                string key = getKey(name1,name2);
                auto it = myConstraints.find(key);

                
                if (it != myConstraints.end())
                {
                    vector<Dir> direction = it ->second->getDirPair(v1, v2);
                    //cout <<direction.size() ;
                    //cout <<it->second->getConstraint(v1, v2) <<endl;
                    Dir p = direction[rand() % 2];


                    
                    
                    
                    switch (p) {
                        case LF:
                            if (name1 == it->second->getFirstVarName()){
                                assignment2--;}
                            else{
                                assignment1--;}
                            cout <<"LF" <<endl;
                            break;
                        case RT:
                            if (name1 == it->second->getFirstVarName()){
                                assignment2++;}
                            else{
                                assignment1++;}
                            cout <<"RT" <<endl;
                            break;
                        case DN:
                            if (name1 == it->second->getFirstVarName()){
                                assignment1++;}
                            else{
                                assignment2++;}
                            cout <<"DN" <<endl;
                            break;
                        case UP:
                            if (name1 == it->second->getFirstVarName()){
                                assignment1--;}
                            else{
                                assignment2--;}
                            cout <<"UP" <<endl;
                            break;
                    }
                    myAssignments[name1] = assignment1;
                    myAssignments[name2] = assignment2;
                    if (violates){
                        break;
                    }
                    
                    
                    
                    
                }else{
                    cerr << "Not created constraints matrix\n";
                }
                
            }
            
        }
        
        
        
        
    }// while vilolates
    //cout <<"final ans: " << endl;
    //printAssign(myAssignments);
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

void writer(string filename,long long readtime, long long solvetime){
    ofstream myfile;
    myfile.open (filename);
    for (auto& ass : myAssignments){
        myfile << ass.first <<" " << ass.second << "\n";
    }
    myfile << "\n" << "readtime : " << readtime << "\n" << "solvetime:  " << solvetime << "\n";
    myfile.close();
}



