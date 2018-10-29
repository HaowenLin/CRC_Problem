//
//  Constraints.cpp
//  crc_solver
//
//  Created by Haowen Lin on 15/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#include "Constraints.hpp"


Constraints::Constraints(string x,string y, int xDomain,int yDomain,int initalNum){
    vars[0] = x;
    vars[1] = y;
    this->xDomain = xDomain;
    this->yDomain = yDomain;
    
    matrix.resize(xDomain);
    directions.resize(xDomain);
    
    for (int i =0; i< xDomain; i++){
        matrix[i].resize(yDomain);
        directions[i].resize(yDomain);
        for (int j=0; j< yDomain; j++){
            matrix[i][j] = initalNum;
        }
    }
}


bool Constraints::checkSmooth()
{
    this-> EliminateDir();
/*
    if (vars[0] =="0" && vars[1]=="12")
    {
        cout << dirMatrix[10][20][0] <<endl;
        int a=1;
    }
*/    
    for (int i =0; i< dirMatrix.size(); i++){
        for (int j =0; j< dirMatrix[0].size(); j++){
            if (reducedMatrix[i][j] == 1){
                continue;
            }
            if (dirMatrix[i][j].size() ==0) {
                cout << " the matrix is not smooth" << std::endl;
                return false;
            }
        }
    }
    //cout << " the matrix is smooth" << std::endl;
    createRealMatrix();
    return true;
}

string Constraints::getFirstVarName(){
    return vars[0];
}

int Constraints::getConstraint(int indexX,int indexY){
    return matrix[indexX][indexY];
}

vector<Dir> Constraints::getDirPair(int indexX,int indexY){    
    vector<Dir> ans;
    if ((matrix[indexX][indexY]==-1 || matrix[indexX][indexY] ==-3)  && indexX ==0){
        ans.push_back(DN);
        return ans;
    }
    if ((matrix[indexX][indexY]==-1 || matrix[indexX][indexY] ==-3) && indexX ==matrix.size()-1){
        ans.push_back(UP);
        return ans;
    }
    
    if ((matrix[indexX][indexY]==-2 || matrix[indexX][indexY] ==-3) && indexY == 0){
        ans.push_back(RT);
        return ans;
    }
    if ((matrix[indexX][indexY]==-2 || matrix[indexX][indexY] ==-3) && indexX ==matrix[0].size()-1){
        ans.push_back(LF);
        return ans;
    }
    DirPair cur =  directions[indexX][indexY];
    ans = parsePairs(cur);
    return  ans;
}

void Constraints::createRealMatrix(){
    
    
    

    
    int row = 0;
    for (int i =0; i< xDomain; i++){
        int col = 0;
        for (int j=0; j< yDomain; j++){
            if (matrix[i][j]==0){
                directions[i][j] = dirMatrix[i-row][j-col][0];
            }
            if (matrix[i][j]==-1){
                directions[i][j]=DNUP;
            }
            if (matrix[i][j]<-1){
                directions[i][j]=LFRT;
                col++;
            }
        }
        if (matrix[i][0]==-1 || matrix[i][0]==-3){
            row++;
        }
    }

    
    
}



void Constraints::setConstraint(int indexX,int indexY)
{
    matrix[indexX][indexY] =0;
}

void Constraints::setConstraint2(vector<int> &row,int index)
{
    //matrix[indexX][indexY] =0;
    matrix[index] =row;
    
}

void Constraints::print(){
    for (int i =0; i< xDomain; i++){
        for (int j=0; j< yDomain; j++){
            cout << this->matrix[i][j] <<" " ;
        }
        cout <<endl;
    }
    
}

string Constraints::Save(){
    string ret = "";
    for (int i =0; i< xDomain; i++){
        for (int j=0; j< yDomain; j++){
            ret+= std::to_string(this->matrix[i][j]);
            ret+=" " ;
        }
        ret+="\n";
    }
    return ret;
    
}


bool Constraints::checkRowConnected()
{
    
    
    
    for (int i =0; i< xDomain; i++){
        int sum = 0;
        for (int j=0; j<yDomain; j++){
            sum += matrix[i][j];
            
        }
        if (sum ==0)
        {
            for (int j=0; j<yDomain; j++){
                matrix[i][j]--;// rows are empty,empty rows are represent by -1
                
            }
        }
    }
    
    for (int j =0; j< yDomain; j++){
        int sum = 0;
        for (int i=0; i<xDomain; i++){
            if (matrix[i][j]>0){
                sum += matrix[i][j];
            }
            //sum += matrix[i][j];
            
        }
        if (sum ==0)
        {
            for (int i=0; i<xDomain; i++){
                matrix[i][j]-=2;//cols are empty, empty cols are represent by -2
            }
            //yDomain--;
            
        }
    }
    
    int startIndex = 0;
    int endIndex = 0;
    
    
    //reduced matrix doesn't have any empty rows/cols
    for (int i=0; i< xDomain;i++){
        vector<int> temp;
        for(int j=0; j<yDomain; j++)
        {
            if (matrix[i][j]<0){continue;}
            temp.push_back(matrix[i][j]);
        }
        if (temp.size()!=0)
        {
            reducedMatrix.push_back(temp);
        }
        
    }
    //dir matrix contains allowed directions for zeros, no empty cols and rows
    vector<DirPair> full{
        LFDN,
        LFUP,
        RTDN,
        RTUP
    };
    
    dirMatrix.resize(reducedMatrix.size());
    for (int i =0; i< reducedMatrix.size(); i++){
        dirMatrix[i].resize(reducedMatrix[0].size());
        for (int j=0; j< dirMatrix[i].size(); j++){
            dirMatrix[i][j] = full;
        }
    }
    
    LeftMost.resize(reducedMatrix.size());
    RightMost.resize(reducedMatrix.size());
    
    // get first 1 in the row
    for (int i=0; i< reducedMatrix.size(); i++)
    {
        for (int j=0; j<reducedMatrix[0].size(); j++)
        {
            if (reducedMatrix[i][j]==1 )
            {
                LeftMost[i] = j;// first 1 in the rows
                break;
            }
        }
    }
    
    
    // get last 1 in the row
    for (int i =0; i< reducedMatrix.size(); i++){
        for (int j= reducedMatrix[0].size()-1; j>=0; j--){
            if (reducedMatrix[i][j]==1 ){
                RightMost[i] = j;
                break;
            }
            
        }
    }
    
    
    for (int i =0; i< reducedMatrix.size(); i++){
        for (int j= LeftMost[i]; j<RightMost[i];++j){
            if (reducedMatrix[i][j]== 0 ){
                return false;
            }
            
        }
    }// not consecutive
    
 
    
    //cout << "constraint is row convex" <<std::endl;
    //this->print();
    
    for (int i =0; i< reducedMatrix[0].size()-1; i++){
        int next = 1;
        int a1 = LeftMost[i];
        int b1 = RightMost[i];
        int a2 = LeftMost[i+next];
        int b2 = RightMost[i+next];
        
        if (b2 < a1-1){return false;}
        if (a2 > b1+1){return false;}
        
    }
    //cout << "constraint is connected" <<std::endl;
    
    return true;
}


void Constraints::EliminateDir(){
    
    //int col = dirMatrix[0].size()-1;
    //int row = dirMatrix.size()-1;
    
    
    int R_Later = RightMost[RightMost.size()-1];// left up
    int R_Later_Right = LeftMost[LeftMost.size()-1];//right up
    
    

    for (int i = reducedMatrix.size()-2 ; i>=0;i--){
        int R_i = max(R_Later,RightMost[i]);
        int R_i_Right = min(R_Later_Right,LeftMost[i]);
        R_Later = R_i;
        R_Later_Right = R_i_Right;
        for (int j =0; j<= R_i;j++){
            if (reducedMatrix[i][j]<=0)
            {
                dirMatrix[i][j].erase(std::remove(dirMatrix[i][j].begin(), dirMatrix[i][j].end(), LFUP), dirMatrix[i][j].end());
            }
            
        }
        for (int j =R_i_Right; j < reducedMatrix[0].size();j++){
            if (reducedMatrix[i][j]<=0)
            {
                dirMatrix[i][j].erase(std::remove(dirMatrix[i][j].begin(), dirMatrix[i][j].end(), RTUP), dirMatrix[i][j].end());
            }
            
        }
    } // check left/right up
    
    if (vars[0] =="0" && vars[1]=="12")
    {
        int a=1;
    }
    R_Later = RightMost[0];// left down
    R_Later_Right = LeftMost[0];//right down
    for (int i=1; i<reducedMatrix.size();i++){
        int R_i = max(R_Later,RightMost[i]);
        int R_i_Right = min(R_Later_Right,LeftMost[i]);
        R_Later = R_i;
        R_Later_Right = R_i_Right;
        for (int j =0; j<= R_i;j++){
            if (reducedMatrix[i][j]<=0)
            {
                dirMatrix[i][j].erase(std::remove(dirMatrix[i][j].begin(), dirMatrix[i][j].end(), LFDN), dirMatrix[i][j].end());
            }
            
        }
        for (int j =R_i_Right; j<reducedMatrix[0].size();j++){
            if (reducedMatrix[i][j]<=0)
            {
                dirMatrix[i][j].erase(std::remove(dirMatrix[i][j].begin(), dirMatrix[i][j].end(), RTDN), dirMatrix[i][j].end());
            }
            
        }
    }// check left/right down
    
    
    
    //first col and last col
    int lastCol = dirMatrix[0].size();
    for (int i=0; i< dirMatrix.size();i++){
        if (reducedMatrix[i][0]<=0)
        {
            // first col, remove left directions
            dirMatrix[i][0].erase(std::remove(dirMatrix[i][0].begin(), dirMatrix[i][0].end(), LFDN), dirMatrix[i][0].end());
            dirMatrix[i][0].erase(std::remove(dirMatrix[i][0].begin(), dirMatrix[i][0].end(), LFUP), dirMatrix[i][0].end());
        }
        
        // last col remove right directions
        if (reducedMatrix[i][lastCol-1] <= 0)
        {
            
            dirMatrix[i][lastCol-1].erase(std::remove(dirMatrix[i][lastCol-1].begin(), dirMatrix[i][lastCol-1].end(), RTDN), dirMatrix[i][lastCol-1].end());
            dirMatrix[i][lastCol-1].erase(std::remove(dirMatrix[i][lastCol-1].begin(), dirMatrix[i][lastCol-1].end(), RTUP), dirMatrix[i][lastCol-1].end());
        }

        
    }
    // first row and last row
    int lastRow = dirMatrix.size()-1;
    for (int j=0; j < lastCol;j++){
        
        //first row, remove up directions
        if (reducedMatrix[0][j] <=0 ){
            dirMatrix[0][j].erase(std::remove(dirMatrix[0][j].begin(), dirMatrix[0][j].end(), LFUP), dirMatrix[0][j].end());
            dirMatrix[0][j].erase(std::remove(dirMatrix[0][j].begin(), dirMatrix[0][j].end(), RTUP), dirMatrix[0][j].end());
            

        }
        if (reducedMatrix[lastRow][j] <=0 )
        {
            dirMatrix[lastRow][j].erase(std::remove(dirMatrix[lastRow][j].begin(), dirMatrix[lastRow][j].end(), LFDN), dirMatrix[lastRow][j].end());
            dirMatrix[lastRow][j].erase(std::remove(dirMatrix[lastRow][j].begin(), dirMatrix[lastRow][j].end(), RTDN), dirMatrix[lastRow][j].end());
        }

        
    }
    

    

    

    

    
    
}


vector<vector<int>>* Constraints::GetMatrix()
{
    return &matrix;
}
