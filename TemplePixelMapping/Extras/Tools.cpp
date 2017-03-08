// Author: Touraj Ebrahimi
#include "Tools.h"
#include <iostream>
#include <fstream>
using namespace std;
Tools::Tools(void)
{
}

void Tools::WriteToFile(const std::string fielName, std::string stringtoWrite)
{
	// Log Tof file
		
			std::ofstream myfile;
			myfile.open (fielName, std::ios::out | std::ios::app); // ,std::ios_base
			myfile << "Log [Begin].\n";
			myfile << stringtoWrite << std::endl;
			myfile << "Log [End].\n";
			myfile.close();

}

bool readLine(IReadFile* f, stringc& str) 
{
			char  ch;
			str = "";
			while (0 != f->read(&ch, 1))
			{
				if (ch == '\n')
				   return true;
				else
				   str += ch;
			}
			return false;
}

bool readToken(IReadFile* f, stringc& str, char tokenToSplit)
{
			char  ch;
			str = "";
			while (0 != f->read(&ch, 1))
			{
			   if (ch == tokenToSplit || ch == '\n')
				   return true;
			   else
				   str += ch;
			}
			return false;
}

Tools::~Tools(void)
{
}
