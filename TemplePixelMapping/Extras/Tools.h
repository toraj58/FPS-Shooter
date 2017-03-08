#pragma once
#include <string>
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace io;


class Tools
{
public:
	Tools(void);
	virtual ~Tools(void);

	static void WriteToFile(std::string fielName, std::string stringtoWrite);

	//Touraj: This method read a file line by line...It can be used in a loop to read whole file
	static bool readLine(IReadFile* f, stringc& str);

	//Touraj: This method read a file token by token...It can be used in a loop to read whole file
	static bool readToken(IReadFile* f, stringc& str, char tokenToSplit);
};

