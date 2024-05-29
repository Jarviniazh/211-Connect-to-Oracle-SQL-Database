#include <iostream>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;


struct Employee {
	int employeeNumber;
	char lastName[50];
	char firstName[50];
	char email[100];
	char phone[50];
	char extension[10];
	char reportsTo[100];
	char jobTitle[50];
	char city[50];
};

//Milestone 1
//Define the menu interface and return the option selected by the user
int menu(void);

//Take the employee number from the user and validate it, return the validated number - Option 1
int validateNumber();

//Find the employee with the given employee number and store the information in the emp structure - Option 1
//Return 0 if the employee does not exist, 1 if the employee exists
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);

//Display the employee information
void displayEmployee(Connection* conn, struct Employee emp);

//Display all the employees in the database with their information - Option 1
void displayAllEmployees(Connection* conn);

//Milestone 2
//Insert a new employee into the database - Option 3
void insertEmployee(Connection* conn, struct Employee emp);
//Update the employee information (Extension only) - Option 4
void updateEmployee(Connection* conn, int employeeNumber);
//Delete the employee from the database - Option 5
void deleteEmployee(Connection* connn, int employeeNumber);

//Validate the email address
bool isValidEmail(const string& email);
//Validate the phone extension
bool isValidExtension(const char* extension);
