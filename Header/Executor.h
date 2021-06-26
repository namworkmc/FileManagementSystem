#ifndef _EXECUTOR_
#define _EXECUTOR_

#include <iostream>
#include <sstream>
#include <codecvt>

#include "Display.h"
#include "MFT.h"
#include "NTFSPartitionBootSector.h"
#include "BSFAT32.h"

class Executor
{
private:
	LPCWSTR driver;
public:
	Executor(LPCWSTR driver)
	{
		this->driver = driver;
	}

	void execute();
};



#endif // _EXECUTE_
