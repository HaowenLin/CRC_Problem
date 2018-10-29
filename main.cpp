//
//  main.cpp
//  CRC_Generator_Solver
//
//  Created by Haowen Lin on 25/7/2018.
//  Copyright © 2018 HaowenLin. All rights reserved.
//

#include <iostream>
#include "CRC_Problems_Solver/Solver.h"
#include "CRC_Generator/CRC_Generator.hpp"

int main(int argc, const char * argv[]) {
    
    string inputfile = "/Users/haowenlin/Documents/usc/大三生活/大三下/research/cstheory/code/CRC_Generator_Solver/CRC_Generator_Solver/Output/out.txt";
    string outputfile = "/Users/haowenlin/Documents/usc/大三生活/大三下/research/cstheory/code/CRC_Generator_Solver/CRC_Generator_Solver/Output/assignments.txt";
    string outputGenerator = "/Users/haowenlin/Documents/usc/大三生活/大三下/research/cstheory/code/CRC_Generator_Solver/CRC_Generator_Solver/Output/out.txt";
    
    bool generateCtr = false;
    bool solverCtr = true;
    
    if (generateCtr)
    {
        CRC_Generator newGenerator(100,2475,50,outputGenerator);
        newGenerator.crc_generator_wrapper();
    }
    if (solverCtr)
    {
        
        solver(inputfile, outputfile);
    }

    return 0;
}
