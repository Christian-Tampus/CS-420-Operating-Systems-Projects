/*
Name: Christian Tampus
Class: CS-420 Operating Systems
Assignment: Exercise 2
Date: 9/5/2025
Professor: Dr. Ray
*/
#include <sys/types.h>
#include <sys/sysctl.h>
#include <err.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;


//printNumCPUs prints out the number of CPUs: sysctl HW_NCPU
void printNumCPUs(int[], size_t&); //I HAVE PROVIDED THIS FUNCTION

/************
 * The below function declarations are given and may not be changed.
 * Your assignment is to provide the implementations for these.
*/
//printPageSize prints out the software page size: sysctl HW_PAGESIZE
void printPageSize(int[], size_t&);

//printPhysicalMem prints out the total physical memory: sysctl HW_PHYSMEM
void printPhysicalMem(int[], size_t&);

//printMachineClass print out the machine class: sysctl HW_MACHINE
void printMachineClass(int[], size_t&, char*);

//printModel prints out the machine model: sysctl HW_MODEL
void printModel(int[], size_t&, char*);

//helper method to get buffer size
int getBufferSize(int mib[], size_t& len);

//helper method to print c-strings
void printCString(int mib[], int mibSize, char* p, size_t& len, size_t mallocSize, std::string customOutput);

//Do not change the main()
int main()
{
	int mib[2];
	size_t len;
	int choice=0;
	char* p;
	
	while(choice != 6)
	{
		cout<<"Options List: "<<endl;
		cout<<"  1: Number of CPUs"<<endl;
	     cout<<"  2: Page Size"<<endl;
		cout<<"  3: Total Physical Memory (bytes)"<<endl;
		cout<<"  4: Machine Class"<<endl;
		cout<<"  5: Model"<<endl;	
		cout<<"  6: Quit"<<endl;
		cout<<"Make Selection: ";
		cin>>choice;
		
		switch(choice)
		{
			case 1:
				printNumCPUs(mib, len);
				break;
			case 2: 
				printPageSize(mib, len);
				break;
			case 3:
				printPhysicalMem(mib, len);
				break;
			case 4: 
				printMachineClass(mib, len, p);
				break;
			case 5:
				printModel(mib, len, p);
				break;
			default:
				cout<<endl;
				cout<<"Good bye!"<<endl;
		}
	}
}
void printNumCPUs(int mib[], size_t& len)
{
	int cpuCount;
	
	mib[0] = CTL_HW;
	mib[1] = HW_NCPU;
	
	len = sizeof(cpuCount);
    //Important Notes:
	//Parameter 1: int *name (int array, typically 2 describing the path of the sysctl hierarchy)
    //Parameter 2: u_int namelen (unsigned int holding the size of the array *name of parameter 1)
    //Parameter 3: void *oldp (output buffer for where data is stored)
    //Parameter 4: size_t *oldlenp (size of output buffer for input and output, in bytes example int = 4 bytes, char = 1 byte)
    //Parameter 5: void *newp (pointer to a buffer if you want to set/write to the system, NULl/nullptr if only getting data/info)
    //Parameter 6: size_t newlen (how many bytes are in *newp buffer if set/write the size must mach the data you want to write, 0 if only getting data/info)
    //sysctl() return type is an integer, 0 = Success, -1 = Failure/error occured
	if(sysctl(mib, 2, &cpuCount, &len, nullptr, 0) == -1)
	{
		err(1, "sysctl");
	}
	else
	{
		std::cout << std::endl;
        std::cout << "-------------------------" << std::endl;
        std::cout << "[SYSTEM MESSAGE] Number Of CPUs: " << cpuCount << std::endl << std::endl;
	};
	return;
};
void printPageSize(int mib[], size_t& len)
{
	mib[0] = CTL_HW;
	mib[1] = HW_PAGESIZE;
	int pageFileSize = 0;
	len = sizeof(pageFileSize);
	int sysctlResult = sysctl(mib,2,&pageFileSize,&len,nullptr,0);
	if (sysctlResult == 0)
	{
		std::cout << std::endl;
    	std::cout << "-------------------------" << std::endl;
		std::cout << "[SYSTEM MESSAGE] Software Page Size: " << pageFileSize << std::endl << std::endl;
	}
	else
	{
		err(1, "sysctl");
	};
	return;
};
void printPhysicalMem(int mib[], size_t& len)
{
	mib[0] = CTL_HW;
	mib[1] = HW_PHYSMEM;
	uint64_t totalPhysicalMemory = 0;
	len = sizeof(totalPhysicalMemory);
	int sysctlResult = sysctl(mib,2,&totalPhysicalMemory,&len,nullptr,0);
    if (sysctlResult == 0)
    {
		std::cout << std::endl;
    	std::cout << "-------------------------" << std::endl;
		std::cout << "[SYSTEM MESSAGE] Total Physical Memory (In Bytes): " << totalPhysicalMemory << std::endl << std::endl;
    }
    else
    {
		err(1, "sysctl");
    };
    return;
};
void printMachineClass(int mib[], size_t& len, char* p)
{
	mib[0] = CTL_HW;
	mib[1] = HW_MACHINE;
	if (getBufferSize(mib,len) == 0)
	{
		printCString(mib,2,p,len,len,"Machine Class");
	}
	else
	{
		err(1, "sysctl");
	};
	return;
};
void printModel(int mib[], size_t& len, char* p)
{
	mib[0] = CTL_HW;
	mib[1] = HW_MODEL;
	if (getBufferSize(mib,len) == 0)
	{
		printCString(mib,2,p,len,len,"Machine Model");
	}
	else
	{
		err(1, "sysctl");
	};
};
int getBufferSize(int mib[], size_t& len)
{
	int sysctlGetBufferSize = sysctl(mib,2,nullptr,&len,nullptr,0);
	return sysctlGetBufferSize;
};
void printCString(int mib[], int mibSize, char* p, size_t& len, size_t mallocSize, std::string customOutput)
{
	p = static_cast<char*>(malloc(mallocSize));
	if (p != nullptr)
	{
		int sysctlResult = sysctl(mib,mibSize,p,&len,nullptr,0);
		if (sysctlResult == 0)
		{
			std::cout << std::endl;
			std::cout << "-------------------------" << std::endl;
			std::cout << "[SYSTEM MESSAGE] " << customOutput << ": " << p << std::endl << std::endl;
		}
		else
		{
			err(1, "systcl");
		};
	}
	else
	{
		std::cout << "[SYSTEM MESSAGE] Mallocation Failed!" << std::endl;
	};
	free(p);
	return;
};