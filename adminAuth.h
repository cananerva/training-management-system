#ifndef ADMIN_AUTH_H
#define ADMIN_AUTH_H

#include <iostream>
#include <mysql/mysql.h>
using namespace std;

class AdminAuth {
public:
    void login() {
        string username, password;

        cout << "\nAdmin Login\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (username == "admin" && password == "12345") {
            cout << "\nLogin successful. Welcome, Admin!\n";
            adminMenu();
        } else {
            cout << "\nInvalid credentials.\n";
        }
    }

    void adminMenu() {
        int choice;

        while (true) {
            cout << "\nAdmin Menu:\n";
            cout << "1 - Create Schedule\n";
            cout << "2 - View Schedule\n";
            cout << "3 - Delete Schedule\n";
            cout << "4 - Update Schedule\n";
            cout << "5 - Create Faculty Account\n";
            cout << "6 - Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1)
                createSchedule();
            else if (choice == 2)
                viewSchedule();
            else if (choice == 3)
                deleteSchedule();
            else if (choice == 4)
                updateScheduleInteractive();
            else if (choice == 5)
                createFacultyAccount();
            else if (choice == 6) {
                cout << "Logged out.\n";
                break;
            } else
                cout << "Invalid option.\n";
        }
    }

    void createFacultyAccount() {
        MYSQL* conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);

        if (!conn) {
            cout << "Database connection failed.\n";
            return;
        }

        string username, password;

        cout << "\nCreate Faculty Account:\n";
        cout << "Username: ";
        cin.ignore();
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        string query = "INSERT INTO faculty_account (username, password) VALUES('" + username + "', '" + password + "')";

        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "Faculty account created successfully.\n";
        } else {
            cout << "Failed to create faculty account: " << mysql_error(conn) << endl;
        }

        mysql_close(conn);
    }

    void createSchedule() {
        MYSQL* conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);

        if (!conn) {
            cout << "Database connection failed.\n";
            return;
        }

        int batch_id, nop;
        string technology, start_date, end_date, venue, instructor;

        cout << "\nCreate New Schedule:\n";
        cout << "Batch ID: ";
        cin >> batch_id;
        cout << "Technology: ";
        cin >> technology;
        cout << "Start Date (YYYY-MM-DD): ";
        cin >> start_date;
        cout << "End Date (YYYY-MM-DD): ";
        cin >> end_date;
        cout << "Venue: ";
        cin.ignore();
        getline(cin, venue);
        cout << "Number of Participants: ";
        cin >> nop;
        cin.ignore();
        cout << "Instructor (Username): ";
        getline(cin, instructor);

        // Check if instructor exists
        string checkQuery = "SELECT * FROM faculty_account WHERE username = '" + instructor + "'";
        if (mysql_query(conn, checkQuery.c_str()) != 0) {
            cout << "Error checking instructor: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (mysql_num_rows(res) == 0) {
            cout << "Instructor not found in faculty accounts. Schedule not created.\n";
            mysql_free_result(res);
            mysql_close(conn);
            return;
        }
        mysql_free_result(res);

        string query = "INSERT INTO schedule VALUES(" +
            to_string(batch_id) + ", '" + technology + "', '" + start_date + "', '" +
            end_date + "', '" + venue + "', " + to_string(nop) + ", '" + instructor + "')";

        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "Schedule inserted successfully.\n";
        } else {
            cout << "Failed to insert schedule: " << mysql_error(conn) << endl;
        }

        mysql_close(conn);
    }

    void viewSchedule() {
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES* res;

        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);

        if (!conn) {
            cout << "Failed to connect to DB.\n";
            return;
        }

        if (mysql_query(conn, "SELECT * FROM schedule") != 0) {
            cout << "Failed to fetch data: " << mysql_error(conn) << endl;
            return;
        }

        res = mysql_store_result(conn);

        cout << "\nExisting Schedules:\n";
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

    void deleteSchedule() {
        MYSQL* conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);

        if (!conn) {
            cout << "DB connection failed.\n";
            return;
        }

        int batch_id;
        cout << "Enter Batch ID to delete: ";
        cin >> batch_id;

        string query = "DELETE FROM schedule WHERE batch_id = " + to_string(batch_id);

        if (mysql_query(conn, query.c_str()) == 0) {
            if (mysql_affected_rows(conn) > 0)
                cout << "Schedule deleted successfully.\n";
            else
                cout << "No schedule found with that ID.\n";
        } else {
            cout << "Error deleting schedule: " << mysql_error(conn) << endl;
        }

        mysql_close(conn);
    }

    void updateScheduleInteractive() {
        MYSQL* conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "training_db", 3306, NULL, 0);

        if (!conn) {
            cout << "DB connection failed.\n";
            return;
        }

        int batch_id;
        cout << "Enter Batch ID to update: ";
        cin >> batch_id;

        int choice;
        cout << "\nWhich field do you want to update?\n";
        cout << "1 - Technology\n";
        cout << "2 - Start Date\n";
        cout << "3 - End Date\n";
        cout << "4 - Venue\n";
        cout << "5 - Number of Participants\n";
        cout << "6 - Instructor\n";
        cout << "7 - Cancel\n";
        cout << "Enter your choice: ";
        cin >> choice;

        string query;
        string inputStr;
        int inputInt;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter new Technology: ";
                getline(cin, inputStr);
                query = "UPDATE schedule SET technology = '" + inputStr + "' WHERE batch_id = " + to_string(batch_id);
                break;
            case 2:
                cout << "Enter new Start Date (YYYY-MM-DD): ";
                getline(cin, inputStr);
                query = "UPDATE schedule SET start_date = '" + inputStr + "' WHERE batch_id = " + to_string(batch_id);
                break;
            case 3:
                cout << "Enter new End Date (YYYY-MM-DD): ";
                getline(cin, inputStr);
                query = "UPDATE schedule SET end_date = '" + inputStr + "' WHERE batch_id = " + to_string(batch_id);
                break;
            case 4:
                cout << "Enter new Venue: ";
                getline(cin, inputStr);
                query = "UPDATE schedule SET venue = '" + inputStr + "' WHERE batch_id = " + to_string(batch_id);
                break;
            case 5:
                cout << "Enter new Number of Participants: ";
                cin >> inputInt;
                query = "UPDATE schedule SET nop = " + to_string(inputInt) + " WHERE batch_id = " + to_string(batch_id);
                break;
            case 6:
                cout << "Enter new Instructor: ";
                cin.ignore();
                getline(cin, inputStr);
                query = "UPDATE schedule SET instructor = '" + inputStr + "' WHERE batch_id = " + to_string(batch_id);
                break;
            case 7:
                cout << "Update cancelled.\n";
                mysql_close(conn);
                return;
            default:
                cout << "Invalid choice.\n";
                mysql_close(conn);
                return;
        }

        if (mysql_query(conn, query.c_str()) == 0) {
            if (mysql_affected_rows(conn) > 0)
                cout << "Schedule updated successfully.\n";
            else
                cout << "No schedule found with that ID.\n";
        } else {
            cout << "Error updating schedule: " << mysql_error(conn) << endl;
        }

        mysql_close(conn);
    }
};

#endif
