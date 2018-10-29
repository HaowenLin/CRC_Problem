//
//  Constraints.hpp
//  crc_solver
//
//  Created by Haowen Lin on 15/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#ifndef Constraints_hpp
#define Constraints_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;



enum Dir
{   LF=1,
    RT=2,
    DN=4,
    UP=8 };

enum DirPair
{
    LFRT=1,
    LFDN=2,
    LFUP=3,
    RTDN=4,
    RTUP=5,
    DNUP=6,
};


class Constraints
{
    
private:
    vector<vector<int>>matrix; // contains the original matrix
    vector<vector<vector<DirPair>>> dirMatrix; // contains the reduced directions
    vector<int>LeftMost;
    vector<int>RightMost;
    vector<vector<int>>reducedMatrix;// contains the matrix remove empty rows/cols
    vector<vector<DirPair>> directions;// contains the original directions for all 0's
    
    
    
public:
    
    int m_11=-1;
    int m_12=-1;
    int m_21=-1;
    int m_22=-1;
    int xDomain = 0;
    int yDomain = 0;
    string vars[2];
    
    Constraints(string x,string y, int xDomain,int yDomain,int initalNum); // constructors
    string getFirstVarName(); // get first var name , the row var
    void setConstraint(int indexX,int indexY);// set value to be 0
    void setConstraint2(vector<int> &row,int index);
    int getConstraint(int indexX,int indexY);
    vector<Dir> getDirPair(int indexX,int indexY);
    
    bool checkSmooth();
    bool checkRowConnected();
    void EliminateDir();
    
    void print();
    std::string Save();
    // get direction matrix
    void createRealMatrix();
    vector<vector<int>>* GetMatrix();
    
    std::vector<Dir>parsePairs(int num)// parse the directions pairs into vectors s
    {
        vector<Dir> ret;
        switch (num) {
            case 1:
                ret.push_back(LF);
                ret.push_back(RT);
                break;
            case 2:
                ret.push_back(LF);
                ret.push_back(DN);
                break;
            case 3:
                ret.push_back(LF);
                ret.push_back(UP);
                break;
            case 4:
                ret.push_back(RT);
                ret.push_back(DN);
                break;
            case 5:
                ret.push_back(RT);
                ret.push_back(UP);
                break;
            case 6:
                ret.push_back(DN);
                ret.push_back(UP);
                break;
        }
        return ret;
        
    }
    
};



#endif /* Constraints_hpp */
