#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <occi.h>
#include <cstring>
#include <regex>
#include "menu.h"

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

//Define the menu interface
int menu(void) {
	cout << "********************* HR Menu *********************" << endl;
	cout << "  1) Find Employee" << endl;
	cout << "  2) Employees Report" << endl;
	cout << "  3) Add Employee" << endl;
	cout << "  4) Update Employee" << endl;
	cout << "  5) Remove Employee" << endl;
	cout << "  0) Exit" << endl;
	cout << "  Select an option: ";
	int option;
	cin >> option;
	//Validate the input
	while (option < 0 || option > 5 || cin.fail()) {
		cout << "Invalid option. Please enter [0] - [5]: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> option;
	}
	//Return the option
	return option;
}

//Milestone 1
//Take the employee number from the user
int validateNumber() {
	int input;
	cout << "Please enter the Employee Number: ";
	cin >> input;
	//Validate the input
	while (input < 1000 || input > 9999 || cin.fail()) {
		cout << "Invalid input. Please enter 4 digits Employee Number: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> input;
	}
	return input;
}

//Find the employee with the given employee number and store the information in the emp structure
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) {
	// Create a statement object
	Statement* stat = conn->createStatement();
	// Execute the query to find the employee
	ResultSet* rs = stat->executeQuery("SELECT e.employeenumber, e.lastname, e.firstname, e.email, o.phone, e.extension, "
		"m.firstname || ' ' || m.lastname AS Manager, e.jobtitle, o.city "
		"FROM dbs211_employees e "
		"LEFT JOIN dbs211_offices o ON e.officecode = o.officecode "
		"LEFT JOIN dbs211_employees m ON e.reportsto = m.employeenumber "
		"WHERE e.employeenumber = " + to_string(employeeNumber));
	if (!rs->next()) {
		// If the result set is empty
		return 0;
	}
	else {
		// If the result set is not empty store the data in the struct
		emp->employeeNumber = rs->getInt(1);
		strcpy(emp->lastName, rs->getString(2).c_str());
		strcpy(emp->firstName, rs->getString(3).c_str());
		strcpy(emp->email, rs->getString(4).c_str());
		strcpy(emp->phone, rs->getString(5).c_str());
		strcpy(emp->extension, rs->getString(6).c_str());
		strcpy(emp->reportsTo, rs->getString(7).c_str());
		strcpy(emp->jobTitle, rs->getString(8).c_str());
		strcpy(emp->city, rs->getString(9).c_str());
	}
	return 1;
}

//Display the employee information
void displayEmployee(Connection* conn, struct Employee emp) {
	Statement* stat = conn->createStatement();
	cout << endl << "********* Search Result **********" << endl;
	cout << "employeeNumber = " << emp.employeeNumber << endl;
	cout << "lastName = " << emp.lastName << endl;
	cout << "firstName = " << emp.firstName << endl;
	cout << "email = " << emp.email << endl;
	cout << "phone = " << emp.phone << endl;
	cout << "extension = " << emp.extension << endl;
	cout << "reportsTo = " << emp.reportsTo << endl;
	cout << "jobTitle = " << emp.jobTitle << endl;
	cout << "city = " << emp.city << endl;
}

//Display all employees - Option 2
void displayAllEmployees(Connection* conn) {
	Statement* stat = conn->createStatement();
	ResultSet* rs = stat->executeQuery(
		"SELECT e.employeenumber AS E, e.firstname || ' ' || e.lastname AS \"Employee Name\", e.email AS Email, "
		"o.phone AS Phone, e.extension AS Ext, m.firstname || ' ' || m.lastname AS Manager "
		"FROM dbs211_employees e "
		"LEFT JOIN dbs211_offices o ON e.officecode = o.officecode "
		"LEFT JOIN dbs211_employees m ON e.reportsto = m.employeenumber "
		"ORDER BY o.officecode, e.employeenumber");

	if (!rs->next()) {
		//if the result set is empty
		cout << "There is no employees¡¯ information to be displayed." << endl;
	}
	else {
		cout << endl;
		cout << "E       Employee Name            Email                                   Phone                Ext         Manager" << endl;
		cout << "----------------------------------------------------------------------------------------------------------------------" << endl;
		//if the result set in not empty
		do {
			cout.setf(ios::left, ios::adjustfield);
			cout.width(8);
			cout << rs->getString(1);
			cout.width(25);
			cout << rs->getString(2);
			cout.width(40);
			cout << rs->getString(3);
			cout.width(20);
			cout << rs->getString(4);
			cout.width(11);
			cout << rs->getString(5);
			cout.width(30);
			cout << rs->getString(6) << endl;
		} while (rs->next());//if there is more rows,iterate
	}
}

//Milestone 2
//Insert an employee - Option 3
void insertEmployee(Connection* conn, struct Employee emp) {
	int inputEmpNum = validateNumber();
	char email[100];
	int officecode = 0;

	if (findEmployee(conn, inputEmpNum, &emp) == 1) {
		cout << "An employee with the same employee number exists." << endl;
	}
	else
	{
		emp.employeeNumber = inputEmpNum;
		cout << " Please enter the information for Employee " << emp.employeeNumber << ": " << endl;
		//cout << "Employee Number: " << emp.employeeNumber << endl;
		cin.ignore();
		cout << "Last Name: ";
		cin.getline(emp.lastName, 50);
		cout << "First Name: ";
		cin.getline(emp.firstName, 50);

		cout << "Email: ";
		while (true) {
			cin.getline(email, 100);
			//Validate the email address
			if (isValidEmail(email)) {
				break;
			}
			else {
				cout << "Invalid email address. Please enter a valid email abc@email.com: ";
			}
		}
		strcpy(emp.email, email);

		cout << "Job Title: ";
		cin.getline(emp.jobTitle, 50);
		cout << "City: ";
		cin.getline(emp.city, 50);

		Statement* stat = conn->createStatement();
		//check if the office code exists for the given city
		ResultSet* office = stat->executeQuery("SELECT officecode FROM dbs211_offices WHERE LOWER(city) = LOWER('" + string(emp.city) + "')");

		if (office->next()) {
			// If the office code exists, set officecode to the value
			officecode = office->getInt(1);
		}
		else {
			//For simplicity, assume that the office code of the new employees is 1 if the office code does not exist
			officecode = 1;
		}

		//For simplicity, assume that the manager id (reportsTo) is 102 by default. 102 or 1002?
		ResultSet* insertEmp = stat->executeQuery("INSERT INTO dbs211_employees (employeenumber, lastname, firstname, extension, email, officecode, reportsto, jobtitle) "
			"VALUES (" + to_string(emp.employeeNumber) + ", '" + emp.lastName + "', '" + emp.firstName + "', 'Unknown', '" + emp.email + "', " + to_string(officecode) + ", 1002, '" + emp.jobTitle + "')");

		if (insertEmp->getStatement()->getUpdateCount() == 1) {
			cout << "The employee has been added successfully." << endl;
		}
		else {
			cout << "Failed to insert the data." << endl;
		}
	}
}

//Update the employee - Option 4
void updateEmployee(Connection* conn, int employeeNumber) {
	Employee emp; //Create a new employee object to store the information
	char extension[10];
	//Find the employee with the given employee number and check if it exists
	while(!findEmployee(conn, employeeNumber, &emp))
	{
		cout << "Employee " << employeeNumber << " does not exist. Please enter a valid Employee Number: ";
		cin >> employeeNumber;
	}
	cin.ignore();

	// Take the phone extension from the user
	cout << "Please enter the new phone extension: ";
	while (true) {
		cin.getline(extension, 10);
		// Validate the extension
		if (isValidExtension(extension)) {
			break;
		}
		else {
			cout << "Invalid extension. Please enter extension in the format x1111: ";
		}
	}
	strcpy(emp.extension, extension);

	//Create a statement object and execute the query to update the extension
	Statement* stat = conn->createStatement();
	ResultSet* updateEmp = stat->executeQuery("UPDATE dbs211_employees "
		"SET extension = '" + string(emp.extension) + "' "
		"WHERE employeenumber = " + to_string(employeeNumber));

	if (updateEmp->getStatement()->getUpdateCount() == 1) {
		cout << "The employee has been updated successfully." << endl;
	}
	else {
		cout << "Failed to update the data." << endl;
	}
}

//Delete the employee - Option 5
void deleteEmployee(Connection* conn, int employeeNumber) {
	Employee emp{}; //Create a new employee object to store the information
	//Find the employee with the given employee number and check if it exists
	if (!findEmployee(conn, employeeNumber, &emp))
	{
		cout << "Employee " << employeeNumber << " does not exist" << endl;
		return;
	}
	else {
		//Create a statement object and execute the query to update the extension
		Statement* stat = conn->createStatement();
		ResultSet* deleteEmp = stat->executeQuery("DELETE FROM dbs211_employees "
			"WHERE employeenumber = " + to_string(employeeNumber));

		if (deleteEmp->getStatement()->getUpdateCount() == 1) {
			cout << "The employee has been deleted successfully." << endl;
		}
		else {
			cout << "Failed to delete the data." << endl;
		}
	}
}

//Validate the email address from the user input
bool isValidEmail(const char* email) {
	// define a regular expression
	const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	// Convert char array to string and match it with the regular expression
	return regex_match(string(email), pattern);
}

//Validate the extension from the user input
bool isValidExtension(const char* extension)
{
	// define a regular expression
	const regex pattern("^[xX][0-9]{4}$");
	// match the string with the regular expression
	return regex_match(string(extension), pattern);
}
