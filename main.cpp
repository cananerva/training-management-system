#include <iostream>
#include "include/adminAuth.h"
#include "include/facultyAuth.h"

using namespace std;

int main() {
    int choice;

    while (true) {
        cout << "\nTraining Schedule Management System\n";
        cout << "1 - Admin Login\n";
        cout << "2 - Faculty Login\n";
        cout << "3 - Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            AdminAuth admin;
            admin.login();
        } else if (choice == 2) {
            FacultyAuth faculty;
            faculty.login();
        } else if (choice == 3) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
