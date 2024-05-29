//#include <iostream>
//#include <occi.h>
//
//using oracle::occi::Environment;
//using oracle::occi::Connection;
//using namespace oracle::occi;
//using namespace std;
//
//int main() {
//	//OCCI Variables
//	Environment* env = nullptr;
//	Connection* conn = nullptr;
//	//User Variables
//	string str;
//	string usr = "dbs211_241ngg24";
//	string pass = "15977761";
//	string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";
//
//	try
//	{
//		env = Environment::createEnvironment(Environment::DEFAULT);
//		conn = env->createConnection(usr, pass, srv);
//		//cout << "Connection is Successful!" << endl;
//
//		//Practice SELECT
//		Statement* stat = conn->createStatement();
//
//		//Q1 Solution
//		//Display Employee Number, First Name, Last Name, Phone Number, and Extension of all Employees who work in San Francisco.
//		//Sort the report according to the employee number
//		ResultSet* rs1 = stat->executeQuery(
//			"SELECT e.employeenumber,e.firstname,e.lastname,o.phone,e.extension "
//					"FROM dbs211_employees e "
//					"LEFT OUTER JOIN dbs211_offices o ON e.officecode = o.officecode "
//					"WHERE LOWER(o.city)='san francisco' "
//					"ORDER BY employeenumber");
//
//		cout << endl << "-------------------------" <<" Report 1 (Employee Report) " << "-----------------------------------" << endl;
//		cout <<"Employee ID   First Name         Last Name          Phone             Extension"<<endl;
//		cout <<"------------  -----------------  -----------------  ----------------  ---------"<<endl;
//
//		if (!rs1->next()) {
//			//if the result set is empty
//			cout << "Result Set is empty." << endl;
//		}
//		else {
//			//if the result set in not empty
//			do {
//				cout.setf(ios::left, ios::adjustfield);
//				cout.width(14);
//				cout << rs1->getString(1);
//				cout.width(19);
//				cout << rs1->getString(2);
//				cout.width(19);
//				cout << rs1->getString(3);
//				cout.width(18);
//				cout << rs1->getString(4);
//				cout.width(11);
//				cout << rs1->getString(5) << endl;
//			} while (rs1->next());//if there is more rows,iterate
//		}
//
//		//Q2 Solution
//		//Display Employee Number, Last Name, Phone Number, and Extension for all managers.
//		ResultSet* rs2 = stat->executeQuery(
//			"SELECT m.employeenumber, m.firstname, m.lastname, o.phone, m.extension "
//					"FROM dbs211_employees m "
//					"LEFT OUTER JOIN dbs211_offices o ON m.officecode = o.officecode "
//					"LEFT OUTER JOIN dbs211_employees l ON m.reportsto = l.employeenumber "
//					"WHERE m.employeenumber IN( "
//						"SELECT e.reportsto "
//						"FROM dbs211_employees e "
//						"WHERE e.reportsto IS NOT NULL) "
//					"OR m.reportsto IS NULL "
//					"ORDER BY m.employeenumber");
//
//		cout << endl << "-------------------------" << " Report 2 (Manager Report) " << "-----------------------------------" << endl;
//		cout << "Employee ID   First Name         Last Name          Phone             Extension" << endl;
//		cout << "------------  -----------------  -----------------  ----------------  ---------" << endl;
//
//		if (!rs2->next()) {
//			//if the result set is empty
//			cout << "Result Set is empty." << endl;
//		}
//		else {
//			//if the result set in not empty
//			do {
//				cout.setf(ios::left, ios::adjustfield);
//				cout.width(14);
//				cout << rs2->getString(1);
//				cout.width(19);
//				cout << rs2->getString(2);
//				cout.width(19);
//				cout << rs2->getString(3);
//				cout.width(18);
//				cout << rs2->getString(4);
//				cout.width(11);
//				cout << rs2->getString(5) << endl;
//			} while (rs2->next());//if there is more rows,iterate
//		}
//
//
//		env->terminateConnection(conn);
//		Environment::terminateEnvironment(env);
//	}
//	catch (SQLException& sqlExcp)
//	{
//		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
//	}
//
//	return 0;
//}