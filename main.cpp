#include <iostream>
#include <occi.h>
#include <fstream>
#include "menu.h"

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

int main() {
	//OCCI Variables
	Environment* env = nullptr;
	Connection* conn = nullptr;
	//User Variables
	string str;
	string usr {};
	string pass {};
	string srv {}；
	int option;
	//int empNum;
	//Employee emp{};
	
	//Read Account info from file
	std::ifstream infile("account.txt");

    // Check if the file is open
    if (infile.is_open()) {
        // Read the entire line
        std::string line;
        if (std::getline(infile, line)) {
            // Use a string stream to parse the line
            std::stringstream ss(line);
            std::getline(ss, usr, ',');
            std::getline(ss, pass, ',');
            std::getline(ss, srv, ',');
        }

        // Close the file
        infile.close();
    } else {
        std::cerr << "Error: Could not open the file for reading." << std::endl;
        return 1; // Exit with error code
    }

	try
	{
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(usr, pass, srv);
		//cout << "Connection is Successful!" << endl;

		do {
			cout << endl;
			option = menu();
			Employee emp{};
			int empNum;
			switch (option)
			{
			case 1:
				//Find Employee
				empNum = validateNumber();
				if (!findEmployee(conn, empNum, &emp)) {
					cout << "Employee " << empNum << " does not exist." << endl;
				}
				else {
					displayEmployee(conn, emp);
				}
				break;

			case 2:
				//Employees Report
				displayAllEmployees(conn);
				break;

			case 3:
				//Add Employee
				insertEmployee(conn, emp);
				break;

			case 4:
				//Update Employee
				empNum = validateNumber();
				updateEmployee(conn, empNum);
				break;

			case 5:
				//Remove Employee
				empNum = validateNumber();
				deleteEmployee(conn, empNum);
				break;

			case 0:
				//Exit
				cout << "Exiting the system!" << endl;
				return 0;

			}
		} while (option != 0);


		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << "\n";
		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}

	return 0;
}

