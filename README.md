# Training Management System

## 📌 Overview

**Training Management System** is a fully functional, console-based C++ application designed for managing and organizing training sessions in educational institutions or corporate environments. It allows an administrator to create and manage training schedules, while faculty members can securely log in to view and manage their assigned sessions.

This system is built using a modular architecture, relies on a **MySQL database** for persistent data storage, and uses **CMake** to manage the build process.


##  Tech Stack

| Component       | Technology         |
|----------------|--------------------|
| Language        | C++                |
| Database        | MySQL              |
| Build System    | CMake              |
| IDE             | Visual Studio Code |
| OS Compatibility| macOS, Linux       |

##  Project Structure

```
training-management-system/
├── include/                    # Header files for modular logic
│   ├── adminAuth.h             # Admin functionalities (login, schedule management, account creation)
│   └── facultyAuth.h           # Faculty functionalities (login, view schedule, update password)
├── main.cpp                    # Entry point and CLI interface
├── CMakeLists.txt              # CMake build configuration
├── build/                      # Generated binary and object files
└── README.md                   # Documentation
```

>  **Important Note:**  
> When downloading this project, make sure both `adminAuth.h` and `facultyAuth.h` are placed inside the `include/` folder.  
> GitHub may not preserve empty folders. If these files are missing or not in the correct path, the project **will not compile**.

##  Setup Instructions

### 1. Install MySQL and MySQL Client

```bash
brew install mysql mysql-client
```

### 2. Create the Database and Tables

```sql
-- Log in to MySQL:
mysql -u root

-- Create the database and tables:
CREATE DATABASE training_db;
USE training_db;

CREATE TABLE faculty_account (
    username VARCHAR(50) PRIMARY KEY,
    password VARCHAR(50)
);

CREATE TABLE schedule (
    batch_id INT PRIMARY KEY,
    technology VARCHAR(50),
    start_date DATE,
    end_date DATE,
    venue VARCHAR(50),
    nop INT,
    instructor VARCHAR(50)
);
```

### 3. Build the Project

```bash
mkdir build
cd build
cmake ..
make
```

### 4. Run the Application

```bash
./training_login
```

##  Admin Features

1. **Login** (Username: `admin`, Password: `12345`)
2. **Create Faculty Account** – Adds a new faculty username and password to the database
3. **Create Schedule** – Add training sessions (instructor must match an existing faculty account)
4. **View All Schedules** – Display all training sessions
5. **Update Schedule** – Modify individual fields (e.g. venue, date, technology, etc.)
6. **Delete Schedule** – Remove a training by batch ID
7. **Logout**

##  Faculty Features

1. **Login** – Authenticated via username and password stored in the database
2. **View Schedule** – Shows only sessions assigned to the logged-in instructor
3. **Change Password** – Allows faculty to update their login password
4. **Logout**

##  Authentication & Data Logic

- Admin credentials are hardcoded for simplicity
- Faculty credentials are securely stored in MySQL
- Schedule entries must reference a valid faculty username as the instructor
- All user input is validated, and database queries are executed via the `mysql.h` API

##  Development Notes

- Ensure MySQL is running with: `mysql.server start`
- After editing header files, re-run `make` to recompile
- The `include/` folder **must** contain both `adminAuth.h` and `facultyAuth.h` to avoid compile errors

##  Acknowledgements

This project was inspired by [Kishan Kumar Rai](https://github.com/kishankumarray)'s CSV-based training management system.  
While the original idea and structure provided a strong starting point, this version is:

- Rewritten from scratch
- Migrated from CSV to a **MySQL database**
- Built with a **CMake-based project structure**

Thanks to the original creator for openly sharing their inspiration.


