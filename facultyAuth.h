#ifndef FACULTY_AUTH_H
#define FACULTY_AUTH_H

#include <iostream>
#include <mysql/mysql.h>
using namespace std;

class FacultyAuth {
public:
    void login() {
        string username, password;

        cout << "\nFaculty Login\n";
        cout << "Username: ";
        cin.ignore();
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (validateLogin(username, password)) {
            cout << "\nLogin successful. Welcome, " << username << "!\n";
            facultyMenu(username);
        } else {
            cout << "\nInvalid credentials.\n";
        }
    }

private:
    bool validateLogin(const string& user, const string& pass) {
        MYSQL* conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);
        if (!conn) {
            cout << "Database connection failed.\n";
            return false;
        }

        string query = "SELECT * FROM faculty_account WHERE username = '" + user + "' AND password = '" + pass + "'";
        bool isValid = false;

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) > 0)
                isValid = true;
            mysql_free_result(res);
        }

        mysql_close(conn);
        return isValid;
    }

    void facultyMenu(const string& username) {
        int choice;
        while (true) {
            cout << "\nFaculty Menu:\n";
            cout << "1 - View My Schedules\n";
            cout << "2 - Change Password\n";
            cout << "3 - Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1)
                viewSchedule(username);
            else if (choice == 2)
                changePassword(username);
            else if (choice == 3) {
                cout << "Logging out...\n";
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }

    void viewSchedule(const string& instructor) {
        MYSQL* conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);
        if (!conn) {
            cout << "Database connection failed.\n";
            return;
        }

        string query = "SELECT * FROM schedule WHERE instructor = '" + instructor + "'";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Failed to fetch schedules: " << mysql_error(conn) << endl;
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\nYour Scheduled Trainings:\n";
        cout << "-------------------------------------------------------------------------------\n";
        cout << "ID  | Tech   | Start Date | End Date   | Venue     | Participants | Instructor\n";
        cout << "-------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            printf("%-4s| %-7s| %-11s| %-11s| %-10s| %-13s| %-10s\n",
                   row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
        }

        mysql_free_result(res);
        mysql_close(conn);
    }

    void changePassword(const string& username) {
        string newPassword;

        cout << "Enter new password: ";
        cin.ignore();
        getline(cin, newPassword);

        MYSQL* conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);
        if (!conn) {
            cout << "Database connection failed.\n";
            return;
        }

        string query = "UPDATE faculty_account SET password = '" + newPassword + "' WHERE username = '" + username + "'";

        if (mysql_query(conn, query.c_str()) == 0 && mysql_affected_rows(conn) > 0) {
            cout << "Password updated successfully.\n";
        } else {
            cout << "Failed to update password: " << mysql_error(conn) << endl;
        }

        mysql_close(conn);
    }
};

#endif
