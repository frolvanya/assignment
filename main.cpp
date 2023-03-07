#include <iostream>
#include <iomanip>
#include <occi.h>

using oracle::occi::Connection;
using oracle::occi::Environment;

using namespace oracle::occi;
using namespace std;

void displayHeader(const char title[])
{
    cout << setw(35) << setfill('-') << ""
         << " " << title << " " << setw(35) << setfill('-') << "" << endl;

    cout << setw(15) << setfill(' ') << left << "Employee ID";
    cout << setw(20) << setfill(' ') << left << "First Name";
    cout << setw(20) << setfill(' ') << left << "Last Name";
    cout << setw(20) << setfill(' ') << left << "Phone";
    cout << setw(4) << setfill(' ') << left << "Extension"
         << "" << endl;

    cout << setw(15) << setfill(' ') << left << "-------------";
    cout << setw(20) << setfill(' ') << left << "------------------";
    cout << setw(20) << setfill(' ') << left << "-------------------";
    cout << setw(20) << setfill(' ') << left << "-------------------";
    cout << setw(4) << setfill(' ') << left << "---------" << endl;
}

void displayData(ResultSet* rs)
{
    do {
        cout << setw(15) << setfill(' ') << left << rs->getInt(1);
        cout << setw(20) << setfill(' ') << left << rs->getString(2);
        cout << setw(20) << setfill(' ') << left << rs->getString(3);
        cout << setw(20) << setfill(' ') << left << rs->getString(4);
        cout << setw(5) << setfill(' ') << left << rs->getString(5) << endl;
    } while (rs->next());

    cout << endl;
}

int main(void)
{
    /* OCCI Variables */
    Environment* env = nullptr;
    Connection* conn = nullptr;
    Statement* stmt = nullptr;
    ResultSet* rs = nullptr;

    /* Used Variables */
    string user = "username";
    string pass = "password";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);

        stmt = conn->createStatement(
            "SELECT e.EMPLOYEENUMBER AS \"Employee ID\", e.FIRSTNAME AS \"First Name\", e.LASTNAME "
            "AS \"Last Name\", o.PHONE AS \"Phone\", e.EXTENSION AS \"Extention\" FROM EMPLOYEES "
            "e, OFFICES o WHERE e.OFFICECODE = o.OFFICECODE AND LOWER(o.CITY) = 'san francisco' "
            "ORDER BY e.EMPLOYEENUMBER");
        rs = stmt->executeQuery();

        if (!rs->next()) {
            cout << "ResultSet is empty." << endl;
        } else {
            displayHeader("Report 1 (Employee Report)");
            displayData(rs);
        }

        stmt = conn->createStatement(
            "SELECT e.EMPLOYEENUMBER AS \"Employee ID\", e.FIRSTNAME AS \"First Name\", e.LASTNAME "
            "AS \"Last Name\", o.PHONE AS \"Phone\", e.EXTENSION AS \"Extention\" FROM EMPLOYEES "
            "e, OFFICES o WHERE e.OFFICECODE = o.OFFICECODE AND e.EMPLOYEENUMBER IN (SELECT "
            "e2.REPORTSTO FROM EMPLOYEES e2) ORDER BY e.EMPLOYEENUMBER");
        rs = stmt->executeQuery();

        if (!rs->next()) {
            cout << "ResultSet is empty." << endl;
        } else {
            displayHeader("Report 2 (Manager Report)");
            displayData(rs);
        }

        conn->terminateStatement(stmt);
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    } catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

    return 0;
}
