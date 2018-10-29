//
//  CRC_Instance_Generator.cpp
//  CRC_Problems_Solver
//
//  Created by Haowen Lin on 18/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#include "CRC_Generator.hpp"

std::random_device rd;
std::mt19937 gen(rd());



// constructor
CRC_Generator::CRC_Generator(int var, int constNum, int domainSize,string outputfile)
{
    this->varNum = var; // how many variables 
    this->consNum = constNum;
    this->domainSize = domainSize;
    this->filename = outputfile;
    for (int i=0; i< var; ++i)
    {
        for (int j=i+1; j< var; ++j )
        {
            signalMatrix.insert(make_pair(i, j));
        }
    }
    
    
    GenerateRandomDomain(domainSize);
    GenerateRandomAssignment();
    
}

// generate domain for each variable within a certain range
void CRC_Generator::GenerateRandomDomain(int range)
{
    std::uniform_int_distribution<> domainGene(1, range);
    for (int i=0; i < varNum; ++i )
    {
        int num = domainGene(gen);
        myDomains[std::to_string(i)] = num;
        
//        
//        ///////!!!!!!!!!!!!!!!!!!!!!!!! testing 
//        myDomains[std::to_string(i)] = 8;
        
        
    }
    
}


void CRC_Generator::GenerateRandomAssignment()
{
    srand(time(NULL));
    for (int i=0; i< varNum; ++i)
    {
        auto it  = myDomains.find(std::to_string(i));
        if (it != myDomains.end()){
            int domain = it->second;
            int randomAssign = rand() % domain;
            assignment[std::to_string(i)] = randomAssign;
        }else{
            cerr << "Could not find " << i << " in the domain map " << std::endl;
        }
    }
    
    for (auto it = assignment.begin(); it !=assignment.end(); it++)
    {
        auto curDo = myDomains.find(it->first);
        cout << it ->first <<" " << it -> second << " " << curDo->second <<endl;
    }
    
    
    ofstream myfile;
    myfile.open ("/Users/haowenlin/Documents/usc/大三生活/大三下/research/cstheory/code/CRC_Generator_Solver/CRC_Generator_Solver/Output/True.txt");
    for (auto& ass : assignment){
        myfile << ass.first <<"," << ass.second << "\n";
    }
    myfile.close();
}



// random pick two variables and generate constraint matrix
void CRC_Generator::PickConstaint()
{
    for (int con=0; con < consNum; con++)
    {
        if (signalMatrix.size() ==0)
        {
            cout << "No remaining choices of two variables" << std::endl;
            return;
        }
        
        // iterate to get the pair of variables
        std::uniform_int_distribution<> constraintGene(0,signalMatrix.size()-1);
        int curConsraint = constraintGene(gen);
        auto iter = signalMatrix.begin();
        std::advance(iter, curConsraint);
        int first = iter -> first;
        int second = iter -> second;
        signalMatrix.erase(iter);
        
        int domain1 = 0;
        int domain2 = 0;
        
        // get domain of related variables
        auto it  = myDomains.find(std::to_string(first));
        if (it != myDomains.end()){
            domain1 = it->second;
        }else{
            cerr << "Could not find " << first << " in the domain map " << std::endl;
        }
        
        it  = myDomains.find(std::to_string(second));
        if (it != myDomains.end()){
            domain2 = it->second;
        }else{
            cerr << "Could not find " << second << " in the domain map " << std::endl;
        }
        // generate matrix
        
        
        
        GenerateConstrain(std::to_string(first), std::to_string(second), domain1, domain2);
        
    
    }
    

    
    
    
    

    
    
    
}

void CRC_Generator::GenerateConstrain(string X_i, string X_j, int domain1, int domain2)
{

    string key = X_i + ','+ X_j;
    
    
    Constraints* cur = new Constraints(X_i,X_j,domain1,domain2,1);
    
    std::uniform_int_distribution<> marker1Gene(0,domain1-1);
    std::uniform_int_distribution<> marker2Gene(0,domain2-1);
    
    int m_11 = marker1Gene(gen);
    int m_12 = marker1Gene(gen);
    int m_21 = marker2Gene(gen);
    
    
    int m_22 = marker2Gene(gen);
    
    cur ->m_11  = m_11;
    cur ->m_12  = m_12;
    cur ->m_21  = m_21;
    cur ->m_22  = m_22;
    
    
    
    int d_i = 0;
    int d_j = 0;
    
    auto it  = assignment.find(X_i);
    if (it != assignment.end()){
        d_i = it->second;
    }else{
        cerr << "Could not find " << X_i << " in the randomassignment " << std::endl;
    }
    
    it  = assignment.find(X_j);
    if (it != assignment.end()){
        d_j = it->second;
    }else{
        cerr << "Could not find " << X_j << " in the randomassignment " << std::endl;
    }
    
    
    //int a=1;
    
    if (m_12 !=0 && m_21 != domain2-1)// first quadrant
    {
        

        if (d_i< m_12 && d_j > m_21)
        {
            if (d_i != 0)
            {
                for (int i= m_21+1; i < domain2; ++i) // row 1
                {
                    if (cur->getConstraint(0, i) == 1)
                    {
                        constOutputNum ++;
                    }
                    cur->setConstraint(0, i);
                }
            }
            else
            {
                for (int i= d_j+1; i < domain2; ++i) // row 1
                {
                    if (cur->getConstraint(0, i) == 1)
                    {
                        constOutputNum ++;
                    }
                    cur->setConstraint(0, i);
                }
                
            }
            
            
            
            std::uniform_int_distribution<> qRandomPicker(d_j,domain2-1);// generate random number [d_j, |D_J|]
            int q = qRandomPicker(gen);
            int numToPick1 = d_i-1;// does not include
            int numToPick2 = m_12-d_i ;
            std::uniform_int_distribution<> endBand1(m_21,q); //random [m_21,q]
            std::uniform_int_distribution<> endBand2(q,domain2-1);// random[q,|DJ|]
            
            
            if (numToPick1 > 0 )
            {
                int* ends1 = new int[numToPick1];
                for (int i=0; i< numToPick1; ++i)
                {
                    ends1[i] = endBand1(gen);
                    
                }
                std::sort(ends1,ends1+numToPick1);
                for (int i=0; i< numToPick1; ++i)// d_i-2 rows
                {
                    int pivotal = ends1[i];
                    for (int j= pivotal+1; j< domain2; ++j)
                    {
                        if (cur->getConstraint(i+1, j) == 1)
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(i+1, j);
                    }
                    
                }// fill in di-2 rows
                
            }
            
            if (numToPick2 > 0 )
            {
                int* ends2 = new int[numToPick2];
                for (int i=0; i< numToPick2; ++i)
                {
                    ends2[i] = endBand2(gen);
                    
                }
                
                std::sort(ends2,ends2+numToPick2);
                
                for (int i=0; i< numToPick2; ++i)// m_12-d_i
                {
                    int pivotal = ends2[i];
                    for (int j= pivotal+1; j< domain2; ++j)
                    {
                        if (cur->getConstraint(i+d_i, j) == 1)
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(i+d_i, j);
                    }
                    
                }// fill in m_12-d_i rows
                
            }
            
            
         
            
        }else
        {
            for (int i= m_21+1; i < domain2; ++i) // row 1
            {
                if (cur->getConstraint(0, i) == 1)
                {
                    constOutputNum ++;
                }
                cur->setConstraint(0, i);
            }
            
            int numToPick = m_12-1;
            
            if (numToPick >0 )
            {
                int* ends = new int[numToPick];
                std::uniform_int_distribution<> quadrantPicker(m_21,domain2-1);
                for (int i=0; i< numToPick; ++i)
                {
                    ends[i] = quadrantPicker(gen);
                    
                }
                std::sort(ends,ends+numToPick);
                for (int i=0; i< numToPick; ++i)
                {
                    int pivotal = ends[i];
                    for (int j= pivotal+1; j< domain2; ++j)
                    {
                        if (cur->getConstraint(i+1, j) == 1)
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(i+1, j);
                    }
                    
                }
                
                
            
            }

        }

            

            
            
            
            
            

        
    }// first quadrant
    
    
    
    // second
    
    
    
    if (m_12 != domain1-1 && m_22 != domain2-1) // second quadrant
    {

        if (d_i >  m_12 && d_j > m_22)
        
        {
            
            if (d_i !=domain1-1 )
            {
                for (int i= m_22+1; i < domain2; ++i)
                {
                    if (cur->getConstraint(domain1-1, i) == 1 )
                    {
                        constOutputNum ++;
                    }
                    cur->setConstraint(domain1-1, i);
                }
            }else
            {
                for (int i= d_j+1; i < domain2; ++i)
                {
                    if (cur->getConstraint(domain1-1, i) == 1 )
                    {
                        constOutputNum ++;
                    }
                    cur->setConstraint(domain1-1, i);
                }
            }// first row end
            
            
            std::uniform_int_distribution<> qRandomPicker(d_j,domain2-1);// generate random number [d_j, |D_J|]
            int q = qRandomPicker(gen);
            int numToPick1 = domain1 - d_i - 2;// does not include d_i
            int numToPick2 = d_i-m_12 ;// include d_i
            std::uniform_int_distribution<> endBand1(m_22,q); //random [m_21,q]
            std::uniform_int_distribution<> endBand2(q,domain2-1);// random[q,|DJ|]
            
            
            if (numToPick1 >0 )
            {
                int* ends1 = new int[numToPick1];
                for (int i=0; i< numToPick1; ++i)
                {
                    ends1[i] = endBand1(gen);
                    
                }
                std::sort(ends1,ends1+numToPick1);
                
                for (int i=0; i< numToPick1; ++i)
                {
                    int pivotal = ends1[i];
                    for (int j= pivotal+1; j< domain2; ++j)
                    {
                        if (cur->getConstraint(domain1-i-2, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(domain1-i-2, j);
                    }
                    
                }
                
            }
            
            if (numToPick2 >0)
            {
                int* ends2 = new int[numToPick2];
                
                for (int i=0; i< numToPick2; ++i)
                {
                    ends2[i] = endBand2(gen);
                    
                }
                
                std::sort(ends2,ends2+numToPick2);
                
                
                
                for (int i=0; i< numToPick2; ++i)
                {
                    int pivotal = ends2[i];
                    for (int j= pivotal+1; j< domain2; ++j)
                    {
                        if (cur->getConstraint(d_i-i, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(d_i-i, j);
                    }
                    
                }
                
            }// numtopick2 > 0

            

            
        }else
        {
            
            
            
            // row bottom
            for (int i= m_22+1; i < domain2; ++i)
            {
                if (cur->getConstraint(domain1-1, i) == 1 )
                {
                    constOutputNum ++;
                }
                cur->setConstraint(domain1-1, i);
            }
            
            int numToPick = domain1-m_12-2;
            
            if (numToPick > 0)
            {
                int* ends = new int[numToPick];
                std::uniform_int_distribution<> quadrantPicker(m_22,domain2-1);
                for (int i=0; i< numToPick; ++i)
                {
                    ends[i] = quadrantPicker(gen);
                    
                }
                std::sort(ends,ends+numToPick);
                for (int i=0; i< numToPick; ++i)
                {
                    int pivotal = ends[i];
                    for (int j= pivotal+1; j< domain2; ++j)
                    {
                        if (cur->getConstraint(domain1-i-2, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(domain1-i-2, j);
                    }
                    
                }
            }// if

            
            
        }// end else

        
        
    }// second quadrant
    
    
    
    
    if (m_11 != domain1-1 && m_22 != 0) // third quadrant
    {
        
        

        if (d_i > m_11 && d_j < m_22)
        {
            
            // first row exception
            
            if (d_i == domain1-1)
            {
                for (int i= 0; i < d_j; ++i)
                {
                    if (cur->getConstraint(domain1-1, i) == 1 )
                    {
                        constOutputNum ++;
                    }
                    cur->setConstraint(domain1-1, i);
                }
                
            }else
            {
                for (int i= 0; i < m_22; ++i)
                {
                    if (cur->getConstraint(domain1-1, i) == 1 )
                    {
                        constOutputNum ++;
                    }
                    cur->setConstraint(domain1-1, i);
                }
            }
            
            
            // third quadrant exception
            
            
            
            std::uniform_int_distribution<> qRandomPicker(0,d_j);// generate random number [0, d_j]
            int q = qRandomPicker(gen);
            int numToPick1 = d_i-m_11;// does include d_i
            int numToPick2 = domain1-d_i-2;// does not include d_i
            
            std::uniform_int_distribution<> endBand1(0,q); //random [0,q]
            std::uniform_int_distribution<> endBand2(q,m_22);// random[q,m_22]
            
            
            
            if (numToPick1 > 0)
            {
                int* ends1 = new int[numToPick1];
                for (int i=0; i< numToPick1; ++i)
                {
                    ends1[i] = endBand1(gen);
                    
                }
                std::sort(ends1,ends1+numToPick1);
                
                for (int i=0; i< numToPick1; ++i)
                {
                    int pivotal = ends1[i];
                    for (int j= 0; j< pivotal; ++j)
                    {
                        if (cur->getConstraint(m_11+1+i, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(m_11+1+i, j);
                    }
                    
                }
                
                
            }
            
            if (numToPick2 >0 )
            {
                int* ends2 = new int[numToPick2];
                for (int i=0; i< numToPick2; ++i)
                {
                    ends2[i] = endBand2(gen);
                    
                }
                
                std::sort(ends2,ends2+numToPick2);
                
                
                
                for (int i=0; i< numToPick2; ++i)
                {
                    int pivotal = ends2[i];
                    for (int j= 0; j< pivotal; ++j)
                    {
                        if (cur->getConstraint(d_i+i+1, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(d_i+i+1, j);
                    }
                    
                }
                
                
            }
            

            
            
            
            

            
            
            
        }else
        {
            for (int i= 0; i < m_22; ++i)
            {
                if (cur->getConstraint(domain1-1, i) == 1 )
                {
                    constOutputNum ++;
                }
                cur->setConstraint(domain1-1, i);
            }
            
            int numToPick = domain1-m_11-2;
            int* ends = new int[numToPick];
            std::uniform_int_distribution<> quadrantPicker(0,m_22);
            
            
            if ( numToPick > 0)
            {
                for (int i=0; i< numToPick; ++i)
                {
                    ends[i] = quadrantPicker(gen);
                    
                }
                std::sort(ends,ends+numToPick);
                for (int i=0; i< numToPick; ++i)
                {
                    int pivotal = ends[i];
                    for (int j= 0 ; j< pivotal; ++j)
                    {
                        if (cur->getConstraint(m_11+1+i, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(m_11+1+i, j);
                    }
                    
                }
            }
            
            
            
        }
        
        


        
    }// end of last
    
    
    // 4th
    if (m_11 !=0 && m_21 != 0 )// fourth quadrant
    {

        
        if (d_i < m_11 && d_j < m_21)
        {

                
                // first row exception
                if (d_i == 0)
                {
                    for (int i= 0; i < d_j; ++i)
                    {
                        if (cur->getConstraint(0, i) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(0, i);
                    }
                    
                }else
                {
                    for (int i= 0; i < m_21; ++i)
                    {
                        if (cur->getConstraint(0, i) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(0, i);
                    }
                }
                
                
                // 4th quadrant exception
                
                
                
                std::uniform_int_distribution<> qRandomPicker(0,d_j);// generate random number [0, d_j]
                int q = qRandomPicker(gen);
                int numToPick1 = m_11-d_i;// does include 
                int numToPick2 = d_i-1;
                
                std::uniform_int_distribution<> endBand1(0,q); //random [0,q]
                std::uniform_int_distribution<> endBand2(q,m_21);// random[q,m_21]
            
            
                
            
                if (numToPick1 > 0 )
                {
                    int* ends1 = new int[numToPick1];
                    for (int i=0; i< numToPick1; ++i)
                    {
                        ends1[i] = endBand1(gen);
                        
                    }
                    std::sort(ends1,ends1+numToPick1);
                    
                    for (int i=0; i< numToPick1; ++i)
                    {
                        int pivotal = ends1[i];
                        for (int j= 0; j< pivotal; ++j)
                        {
                            if (cur->getConstraint(m_11-i-1, j) == 1 )
                            {
                                constOutputNum ++;
                            }
                            cur->setConstraint(m_11-i-1, j);
                        }
                        
                    }
                    
                    
                }
            
            if (numToPick2 > 0)
            {
                int* ends2 = new int[numToPick2];
                for (int i=0; i< numToPick2; ++i)
                {
                    ends2[i] = endBand2(gen);
                    
                }
                
                std::sort(ends2,ends2+numToPick2);
                
                
                
                for (int i=0; i< numToPick2; ++i)
                {
                    int pivotal = ends2[i];
                    for (int j= 0; j< pivotal; ++j)
                    {
                        if (cur->getConstraint(d_i-i-1, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(d_i-i-1, j);
                    }
                    
                }
            
            }
                

                
                
                
                
                

            
            
            
            
            
        }else
        {
            for (int i= 0; i < m_21; ++i)
            {
                if (cur->getConstraint(0, i) == 1 )
                {
                    constOutputNum ++;
                }
                cur->setConstraint(0, i);
            }
            
            int numToPick = m_11-1;
            int* ends = new int[numToPick];
            std::uniform_int_distribution<> quadrantPicker(0,m_21);
            
            
            if (numToPick > 0)
            {
                for (int i=0; i< numToPick; ++i)
                {
                    ends[i] = quadrantPicker(gen);
                    
                }
                std::sort(ends,ends+numToPick);
                for (int i=0; i< numToPick; ++i)
                {
                    int pivotal = ends[i];
                    for (int j= 0; j< pivotal; ++j)
                    {
                        if (cur->getConstraint(m_11-i-1, j) == 1 )
                        {
                            constOutputNum ++;
                        }
                        cur->setConstraint(m_11-i-1, j);
                    }
                    
                }
                
            }
            
            
        }
        
        
        

        

        
    }
    
    
    
    myConstaintStore[key] = cur;
    cur->print();
//    OverriteEntire(cur);
//    cout <<endl;
//    cur->print();
}

void CRC_Generator::crc_generator_wrapper()
{
    PickConstaint();
    
    GenerateOutput(filename);
    OutputMatrix("/Users/haowenlin/Documents/usc/大三生活/大三下/research/cstheory/code/CRC_Generator_Solver/CRC_Generator_Solver/Output/matrix.txt");
    
}

void CRC_Generator::OverriteEntire(Constraints* cur)
{
    srand (time(NULL));
    double probability = 1;
   
    
    for (int i=0; i< cur->xDomain; i++)
    {
        double result = rand() % 100;
        if(result < probability)
        {
            for (int j=0; j< cur->yDomain; j++)
            {
                cur->setConstraint(i, j);
            }
            
        }
        
    }
    
    for (int j=0; j< cur->yDomain; j++)
    {
        double result = rand() % 100;
        if(result < probability)
        {
            for (int i=0; i< cur->xDomain; i++)
            {
                cur->setConstraint(i, j);
            }
            
        }
        
    }
    

    
}

void CRC_Generator::OutputMatrix(string filename)
{
    ofstream myfile;
    myfile.open (filename);
    if (myfile.is_open())
    {

        for (auto constIter = myConstaintStore.begin(); constIter != myConstaintStore.end(); constIter++)
        {
            myfile << constIter->first <<endl;
            myfile << "m_11: " << constIter->second->m_11 <<" m_12: "<<constIter->second->m_12<< " m_21: " <<constIter->second->m_21 << " m_22" <<constIter->second->m_22<<endl;
            myfile << constIter->second->Save() <<endl;
            
            

        }
        
        
        
    }else
    {
        cerr << "Could not open outputfile: " << filename << std::endl;
    }
    myfile.close();
}
void CRC_Generator::GenerateOutput(string filename)
{
    ofstream myfile;
    myfile.open (filename);
    if (myfile.is_open())
    {
        myfile << varNum << endl;
        for(auto domainIter = myDomains.begin(); domainIter != myDomains.end(); domainIter ++)
        {
            myfile << domainIter->first <<"," << domainIter->second <<endl;
        }
        myfile << constOutputNum << endl;
        cout <<"con nu: " << constOutputNum << endl;
        for (auto constIter = myConstaintStore.begin(); constIter != myConstaintStore.end(); constIter++)
        {
            std::string::size_type pos = constIter->first.find(',');
            string X_i = constIter->first.substr(0, pos);
            string X_j = constIter->first.substr(pos+1);
            Constraints* cur  = constIter->second;
            vector<vector<int>> *curMatrix = cur->GetMatrix();
            for (int i=0; i< cur->xDomain;++i)
            {
                for (int j=0; j< cur->yDomain; ++j)
                {
                    if (curMatrix->at(i)[j]==0 )
                    {
                        myfile << X_i << "," << X_j <<"," << i << "," << j << endl;
                    }
                }
                
            }
            
            
        }
        
        
        
    }else
    {
        cerr << "Could not open outputfile: " << filename << std::endl;
    }
    myfile.close();
    
    
    
}
