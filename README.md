# Clubhouse Management System

A console-based facility management and booking application developed in C++ with a MySQL/MariaDB database for a university Workshop 1 project.

## Features

- Administrator and customer login
- Customer registration and profile management
- Facility creation, updates, availability and capacity management
- Facility booking with date, time and pax validation
- Resident discount calculation
- Booking history, cancellation and printable-style receipts
- Facility-based sales reports
- Password, phone number, date and time input validation

## Technology

- C++
- MySQL C API
- MySQL/MariaDB
- Visual Studio 2022 with the MSVC v143 toolset
- Windows console API

## Project structure

```text
clubhouse-management-system/
├── database/
│   └── clubhouse_demo.sql
├── src/
│   ├── main.cpp
│   ├── main.h
│   ├── ClubHouse_ManagementSystem.vcxproj
│   ├── ClubHouse_ManagementSystem.vcxproj.filters
│   └── packages.config
├── .gitignore
├── ClubHouse_ManagementSystem.sln
└── README.md
```

## Requirements

- Windows 10 or Windows 11
- Visual Studio 2022 with **Desktop development with C++**
- MSVC v143 x64/x86 build tools
- XAMPP, MySQL or MariaDB on port `3306`
- A local MySQL `root` account with a blank password for the default demo configuration

## Setup

1. Start Apache and MySQL in XAMPP.
2. Open phpMyAdmin and import `database/clubhouse_demo.sql`. The script creates and selects the `restoranaliff` database automatically.
3. Open `ClubHouse_ManagementSystem.sln` in Visual Studio.
4. Allow Visual Studio to restore the `libmysql-cpp` NuGet package if prompted.
5. Select `Debug` and `x64`.
6. Build the solution, then run it with `Ctrl + F5`.

## Demo accounts

| Role | Username | Password |
| --- | --- | --- |
| Administrator | `demo_admin` | `Admin@123` |
| Resident customer | `resident_demo` | `Demo@123` |
| Non-resident customer | `visitor_demo` | `Demo@123` |

All records in the included SQL file are fictional demonstration data.

## Database connection

The current university-project version connects to:

```text
Host: localhost
Port: 3306
Database: restoranaliff
Username: root
Password: blank
```

If your local MySQL account uses a password, update the connection parameters in `src/main.cpp` before building.

## Security note

This repository preserves the original academic implementation, including plain-text demo passwords and direct SQL query construction. In a production system, passwords should be securely hashed, secrets should be stored outside source code, and all database operations should use parameterized queries.

## Author

Vimalan Ramakrishnan — Bachelor of Computer Science (Computer Security), Universiti Teknikal Malaysia Melaka.
