// Author: Touraj Ebrahimi
#include "RenderConstants.h"
#include <iostream>
#include <fstream>
using namespace std;
RenderConsts::RenderConsts(void)
{
}

void RenderConsts::Get(const string fielName, string stringtoWrite)
{
	// Log Tof file
		
			std::ofstream myfile;
			myfile.open (fielName, std::ios::out | std::ios::app); // ,std::ios_base
			myfile << "Log [Begin].\n";
			myfile << stringtoWrite << std::endl;
			myfile << "Log [End].\n";
			myfile.close();

}

RenderConsts::~RenderConsts(void)
{
}
