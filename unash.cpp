/*
Name: Christian Tampus
Class: CS-420 Operating Systems
Assignment: Exercise 3
Date: 9/14/2025
Professor: Dr. Ray
*/
//unashSkeleton.cpp -- the basic skeleton for a unash (UNA shell) program
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
char** tokenize(std::string);
void deallocate(char** argList);
bool execute(char**);
std::string readLine();
void printTest(char**);
int main()
{
	bool flag = 1;
	std::string line;
	char** argList;
	while(flag)
	{
		std::cout << "csis> ";
		line = readLine();
		argList = tokenize(line);
		//IMPORTANT: comment the next line out when done testing tokenize().
		//printTest(argList);
		flag = execute(argList);
		deallocate(argList);
	};
	return 0;
};
/*
char** tokenize(string)
	This function is responsible for accepting the command string as a parameter.
	It returns a char** pointer.  The function dynamically allocates an array of 
	char* pointers equal to the number of (space delimited) words in the command string.
	Each char* pointer is dynamically allocated to exactly store one word from the command.
	Make the string pointed to by each char* pointer is NULL terminated (C-strings).
	Make sure the array of char* pointers pointed to by the char** pointer is NULL terminated.
	(This is what will be expected by the execvp function.)	
*/
char** tokenize(std::string line)
{
	//PART 2 here
	std::vector<std::string> tokensVector;
	std::stringstream ss(line);
	std::string currentToken;
	while (ss >> currentToken)
	{
		tokensVector.push_back(currentToken);
	};
	char** lineTokenized = new char*[tokensVector.size() + 1];
	for (size_t index = 0; index < tokensVector.size(); index++)
	{
		size_t tokenSize = tokensVector[index].size();
		lineTokenized[index] = new char[tokenSize + 1];
		std::memcpy(lineTokenized[index],tokensVector[index].c_str(),tokenSize);
		lineTokenized[index][tokenSize] = '\0';
	};
	lineTokenized[tokensVector.size()] = nullptr;
	/*
	PREVIOUS IMPLEMENTATION:
	std::vector<std::string> stringTokensVector;
	std::string currentStringToken = "";
	for (int index = 0; index < line.size(); index++)
	{
		if (line[index] != ' ')
		{
			currentStringToken += line[index];
		}
		else if (currentStringToken.size() > 0)
		{
			stringTokensVector.push_back(currentStringToken);
			currentStringToken = "";
		};
	};
	if (currentStringToken.size() > 0)
	{
		stringTokensVector.push_back(currentStringToken);
	};
	char** lineTokenized = new char*[stringTokensVector.size() + 1];
	for (int index1 = 0; index1 < stringTokensVector.size(); index1++)
	{
		int tokenSize = stringTokensVector[index1].size();
		lineTokenized[index1] = new char[tokenSize + 1];
		for (int index2 = 0; index2 < stringTokensVector[index1].size(); index2++)
		{
			lineTokenized[index1][index2] = stringTokensVector[index1][index2];
		};
		lineTokenized[index1][tokenSize] = '\0';
	};
	lineTokenized[stringTokensVector.size()] = nullptr;
	*/
	return lineTokenized;
};
void deallocate(char** argList)
{
	if (argList != nullptr)
	{
		for (int index = 0; argList[index] != nullptr; index++)
		{
			delete[] argList[index];
		};
		delete[] argList;
	};
	return;
};
/*
bool execute(char** args)
	This function accepts the char** that points to the array of pointers to C-strings.  
	This function uses the fork() command to fork a brand new copy of itself.
	Then, for the child process, it makes use of execvp() to overwrite itself
	with a new program.  Call execvp properly making use of the char**.
	For the parent process (still running the shell program), it waits until
	the child process is terminated.  Make use of waitpid for this. 
*/
bool execute(char** args)
{
	//PART 3 here
	const char* UNA_SHELL_COMMANDS[] = {"exit","cd"};
	bool continue_Running_UNA_Shell = true;
	if (args != nullptr && args[0] != nullptr)
	{

		if (std::strcmp(args[0],UNA_SHELL_COMMANDS[0]) == 0)
		{
			continue_Running_UNA_Shell = false;
		}
		else if (std::strcmp(args[0],UNA_SHELL_COMMANDS[1]) == 0)
		{
			if (args[1] == nullptr)
			{
				std::cout << "csis> [SYSTEM ERROR] Change Directory Has Missing Arguments!" << std::endl;
			}
			else
			{
				if (chdir(args[1]) == 0)
				{
					std::cout << "csis> [SYSTEM MESSAGE] Changed Directory To: " << args[1] << std::endl;
				}
				else
				{
					//If chdir() returns anything but a 0 integer then an error has occured, print error message
					std::cout << "csis> [SYSTEM ERROR] chdir() Failed! Error Message: " << strerror(errno) << std::endl;
				};
			};
		}
		else
		{
			pid_t processId = fork();
			if (processId == 0)
			{
				execvp(args[0],args);
				//*If execvp() returns at all then an error has occured, print error message
				std::cout << "csis> [SYSTEM ERROR] execvp() Failed! Error Message: " << strerror(errno) << std::endl;
			}
			else if (processId > 0)
			{
				int childProcessStatus;
				pid_t waitProcessId = waitpid(processId,&childProcessStatus,0);
				if (waitProcessId < 0)
				{
					//If waitpid() returns < 0 then an error has occured, print error message
					std::cout << "csis> [SYSTEM ERROR] waitpid() Failed! Error Message: " << strerror(errno) << std::endl;
				};
			}
			else
			{
				//*If fork() returns < 0 then an error has occured, print error message
				std::cout << "csis> [SYSTEM ERROR] Fork() Failed! Error Message: " << strerror(errno) << std::endl;
			};
		};
	};
	return continue_Running_UNA_Shell;
};
std::string readLine()
{
	std::string line;
	std::getline(std::cin, line);
	return line;
};
void printTest(char** temp)
{
	int counter = 0;
	while(temp[counter] != nullptr)
	{
		std::cout << temp[counter] << std::endl;
		counter++;
	};
	return;
};