#include <iostream>
#include <mysql.h>
#include <string>
#include <conio.h>
#include <iomanip>
#include "main.h"
#include <sstream> 
#include <vector>
#include <ctime>
#include <limits>
#include <windows.h>
#include <regex>


using namespace std;


bool isValidTimeFormat(const std::string& timeStr) {
    std::regex pattern(R"(^(?:[01]\d|2[0-3]):[0-5]\d:[0-5]\d$)");
    return std::regex_match(timeStr, pattern);
}

bool isValidDateFormat(const string& date) {
    // Must be exactly 10 characters: YYYY-MM-DD
    if (date.length() != 10) return false;

    // Check format: YYYY-MM-DD
    if (date[4] != '-' || date[7] != '-') return false;

    // Check digits
    for (int i = 0; i < date.length(); i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    // Extract year, month, day as integers
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    // Basic range check
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}

bool isNumber(const std::string& str) {
    return std::regex_match(str, std::regex("^[0-9]+(\\.[0-9]+)?$"));
}

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
string UserID;

void Register();
void Login();
void adminmenu();
void customermenu();
void viewFacilities(); // Function declaration for viewing facilities
void adminfasi();
void adminmain1();
void custmain1();
void AdminLogin();
void CustLogin();
void booking();
void makeBooking(MYSQL* conn);
void viewBookingHistory(MYSQL* conn);
void adminmanage();
void cancelbooking();
void admincust();
void viewBookingReceipt(MYSQL* conn, int booking_id);
void showSalesReport(MYSQL* conn);

string SessionCustomerID;

class db_response
{
public:
    static void ConnectionFunction()
    {
        conn = mysql_init(0);
        if (conn)
            cout << "Database Connected!" << endl;
        else
            cout << "Failed To Connect!" << endl;

        conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);
        if (conn)
            cout << "Database connected to Mysql" << endl;
        else
            cout << "Failed To Connect to specified database" << endl;
    }
};

void showWelcomeClubhouse() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11); // Light cyan

    cout << R"(

 \ \      / __|  |      __|   _ \   \  |  __|   __ __| _ \      __|  |     |  | _ )  |  |   _ \  |  |   __|  __| 
  \ \ \  /  _|   |     (     (   | |\/ |  _|       |  (   |    (     |     |  | _ \  __ |  (   | |  | \__ \  _|  
   \_/\_/  ___| ____| \___| \___/ _|  _| ___|     _| \___/    \___| ____| \__/ ___/ _| _| \___/ \__/  ____/ ___| 
                                                                                                                 
                  W   E   L   C   O   M   E     T   O     C   L   U   B   H   O   U   S   E

)";
    SetConsoleTextAttribute(hConsole, 7); // Reset to white
}

int main()
{
    system("cls");
    system("title Clubhouse Management System");
    db_response::ConnectionFunction();

    int choice;
    while (true) {
        system("cls");
        showWelcomeClubhouse(); //  WELCOME MESSAGE ALWAYS SHOWS FIRST
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 11);
        cout << "                                    +==========================================+\n";
        SetConsoleTextAttribute(hConsole, 14); // Yellow
        cout << "                                    |   === Welcome To Clubhouse System ===   |\n";
        SetConsoleTextAttribute(hConsole, 11); // Cyan (or your next desired color)
        cout << "                                    +------------------------------------------+\n";
        SetConsoleTextAttribute(hConsole, 7);
        cout << "                                    |              1. Admin                   |\n";
        cout << "                                    |              2. Customer                |\n";
        cout << "                                    |              0. Exit                    |\n";
        SetConsoleTextAttribute(hConsole, 11);
        cout << "                                    +==========================================+\n";
        SetConsoleTextAttribute(hConsole, 7);
        cout << "\n";
        cout << "                                               ENTER YOUR CHOICE: ";
        cin >> choice;

        SetConsoleTextAttribute(hConsole, 11);
        cout << "\n";
        cout << "                                  =================================================\n";
        SetConsoleTextAttribute(hConsole, 14);
        cout << "                                  |               Clubhouse System 2025            |\n";
        cout << "                                  |Designed by:     BOYOPA (VIMALAN)               |\n";
        SetConsoleTextAttribute(hConsole, 11);
        cout << "                                  =================================================\n";
        SetConsoleTextAttribute(hConsole, 7);

        Sleep(3000); // Wait for 3 seconds

        switch (choice) {
        case 1:
            adminmain1();
            break;
        case 2:
            custmain1();
            break;
        case 0:
            cout << "\nExiting program..." << endl;
            return 0;
        case 4:
            admincust();
            break;
        default:
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\n";
            cout << "                                    +---------------------------------------+\n";
            cout << "                                    |         INVALID CHOICE. TRY AGAIN!    |\n";
            cout << "                                    +---------------------------------------+\n";
            SetConsoleTextAttribute(hConsole, 7);
            Sleep(1500);
            break;
        }
    }

    return 0;
}

bool isValidPassword(const string& password) {
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    for (char ch : password) {
        if (isupper(ch)) hasUpper = true;
        else if (islower(ch)) hasLower = true;
        else if (isdigit(ch)) hasDigit = true;
        else if (ispunct(ch)) hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

bool isValidPhone(const string& phone) {
    // Check length
    if (phone.length() < 9 || phone.length() > 11)
        return false;

    // Check that all characters are digits
    for (char ch : phone) {
        if (!isdigit(ch)) {
            return false;  // Reject letters, symbols, etc.
        }
    }

    return true;
}

string getMaskedPassword() {
    string password;
    char ch;

    cout << "Enter Password: ";
    while (true) {
        ch = _getch();  // Reads without echoing

        if (ch == 13) { // Enter key
            cout << endl;
            break;
        }
        else if (ch == 8) { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Erase star
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    return password;
}




//ADMIN
void adminmain1() {
    system("cls");
    system("title Clubhouse Management System");
    db_response::ConnectionFunction();

    int choice;
    while (true) {
        system("cls");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int choice;

        cout << "\n\n";

        // Top border line
        SetConsoleTextAttribute(hConsole, 11); // Cyan
        cout << "                        ********************************************************************************\n";
        SetConsoleTextAttribute(hConsole, 7);  // Default

        cout << "                        *                                                                              *\n";
        cout << "                        *                              ADMIN MENU                                      *\n";
        cout << "                        *                                                                              *\n";
        cout << "                        *                 1. Admin Login                                               *\n";
        cout << "                        *                 2. Back to Main Menu                                         *\n";
        cout << "                        *                                                                              *\n";

        // Bottom border line
        SetConsoleTextAttribute(hConsole, 11); // Cyan
        cout << "                        ********************************************************************************\n";
        SetConsoleTextAttribute(hConsole, 7);  // Default

        // Input prompt
        cout << "\n                              >> ENTER YOUR CHOICE: ";
        cin >> choice;
        switch (choice) {
        case 1:
            AdminLogin();
            break;
        case 2:
            return; // Go back
        default:
            system("cls");

            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);
            cout << "                                        +-----------------------------------------------+\n";
            cout << "                                        |     INVALID CHOICE. PLEASE TRY AGAIN!         |\n";
            cout << "                                        +-----------------------------------------------+\n";
            SetConsoleTextAttribute(hConsole, 7);
            Sleep(1500);
            break;
        }
    }
}

void showLoadingScreen() {
    cout << "\nLoading";
    for (int i = 0; i < 3; ++i) {
        Sleep(800);  
        cout << ".";
        cout.flush(); // Make sure the dot prints immediately
    }
    cout << "\n\n";
}

void showWelcomeBox(const string& name) {
    system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    string message = "WELCOME, " + name + " (ADMIN)!";
    int screenWidth = 120;
    int boxWidth = message.length() + 6;
    int padding = (screenWidth - boxWidth) / 2;

    // Print top border in cyan
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << string(padding, ' ') << string(boxWidth, '=') << endl;

    // Print message line in yellow
    SetConsoleTextAttribute(hConsole, 14); // Yellow
    cout << string(padding, ' ') << "|  " << message << "  |" << endl;

    // Print bottom border in cyan
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << string(padding, ' ') << string(boxWidth, '=') << endl;

    // Reset color before loading
    SetConsoleTextAttribute(hConsole, 7); // Default

    // Loading animation
    cout << "\n" << string(padding, ' ') << "LOADING";
    for (int i = 0; i < 3; ++i) {
        cout << ".";
        Sleep(800);
    }
    cout << "\n\n";
}

void AdminLogin() {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    string username, password;

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n";
    cout << "                       +------------------------------------------------------+\n";
    cout << "                       |                    ADMIN LOGIN                      |\n";
    cout << "                       +------------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cin.ignore();
    cout << "\n";
    cout << "Enter Username: ";
    getline(cin, username);
    password = getMaskedPassword(); // masked password input
    SetConsoleTextAttribute(hConsole, 7);
    Sleep(1500);

    // MySQL query
    string checkAdmin = "SELECT * FROM admin WHERE Username = '" + username + "' AND Password = '" + password + "'";
    const char* ca = checkAdmin.c_str();
    qstate = mysql_query(conn, ca);

    if (!qstate) {
        res = mysql_store_result(conn);
        if (mysql_num_rows(res) == 1) {
            showWelcomeBox(username);   // Show boxed and centered welcome
            adminmenu();                // Proceed to admin menu
            return;
        }
    }

    SetConsoleTextAttribute(hConsole, 12); // Red for error
    cout << "                          +---------------------------------------------------+\n";
    cout << "                          |     LOGIN FAILED.Invalid username or password     |\n";
    cout << "                          +---------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7); // Reset to white
    Sleep(1500);
}
void showSalesReport(MYSQL* conn) {
    system("cls");

    MYSQL_RES* res;
    MYSQL_ROW row;
    const char* query = "SELECT fasi_id, name, SUM(amount) as total_sales FROM booking GROUP BY fasi_id, name";

    if (mysql_query(conn, query)) {
        cout << "Query failed: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\n+========================== SALES REPORT ==========================+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "| Facility ID | Customer Name           | Total Sales (RM)        |\n";
    cout << "+=================================================================+\n";
    double grandTotal = 0.0;

    while ((row = mysql_fetch_row(res))) {
        double facilityTotal = atof(row[2]);
        grandTotal += facilityTotal;

        cout << "| " << setw(11) << left << row[0]
            << "| " << setw(24) << left << row[1]
            << "| " << "RM" << setw(23) << fixed << setprecision(2) << facilityTotal << "|\n";
    }
    cout << "+=================================================================+\n";
    
    cout << "| TOTAL SALES: " << grandTotal << "RM" << right << setw(51) << fixed << setprecision(2) << " |\n";
   
    cout << "+=================================================================+\n";
    SetConsoleTextAttribute(hConsole, 7); 

    mysql_free_result(res);
    system("pause");
    adminmenu();
}
void adminmenu()
{
    while (true) {
        system("cls");

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Get console screen width
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        int screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

        const int menuWidth = 30;
        const int leftPadding = (screenWidth - menuWidth) / 2;

        auto centerText = [&](const string& text, bool highlight = false) {
            if (highlight) SetConsoleTextAttribute(hConsole, 11);  // Cyan
            else SetConsoleTextAttribute(hConsole, 7);             // Reset
            cout << string(leftPadding, ' ') << text << endl;
            };

        // Display Menu
        centerText("==============================", true);
        centerText("|        Admin Menu          |");
        centerText("==============================", true);
        centerText("| 1. Manage Facilities       |");
        centerText("| 2. Manage Bookings         |");
        centerText("| 3. Manage Customer         |");
        centerText("| 4. Sales Report            |");
        centerText("| 0. Back                    |");
        centerText("==============================", true);

        SetConsoleTextAttribute(hConsole, 7);
        cout << string(leftPadding, ' ') << "Enter your choice (0 - 4): ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            adminfasi();
            break;
        case 2:
            adminmanage();
            break;
        case 3:
            admincust();
            break;
        case 4:
            showSalesReport(conn);
            break;
        case 0:
            return;  // Exit back to where adminmenu() was called (e.g., after login)
        default:
            SetConsoleTextAttribute(hConsole, 4);
            cout << string(leftPadding, ' ') << "+-------------------------------------------+\n";
            cout << string(leftPadding, ' ') << "|     INVALID CHOICE. PLEASE TRY AGAIN!     |\n";
            cout << string(leftPadding, ' ') << "+-------------------------------------------+\n";
            SetConsoleTextAttribute(hConsole, 7);
            Sleep(1500);
            break;
        }
    }
}

string simpleHash(const string& input) {
    srand((unsigned int)time(0));  // Seed randomness
    string hashed;
    string letters = "abcdefghijklmnopqrstuvwxyz";

    for (char ch : input) {
        int ascii = (int)ch + 17;
        hashed += to_string(ascii);
        hashed += letters[rand() % letters.size()]; // Add random letter
    }

    // Add some final 2 random characters for more randomness
    hashed += letters[rand() % 26];
    hashed += to_string(rand() % 10);

    // Limit the output to 14 characters for display
    return hashed.substr(0, 14);
}

void admincust() {
    system("cls");
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate;

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

    if (conn) {
        string query = "SELECT * FROM customer";
        qstate = mysql_query(conn, query.c_str());

        if (!qstate) {
            res = mysql_store_result(conn);
            cout << left
                << setw(5) << "ID"
                << setw(15) << "Name"
                << setw(15) << "Phone"
                << setw(20) << "Address"
                << setw(15) << "Username"
                << setw(15) << "Password"
                << setw(8) << "Status" << endl;
            cout << string(95, '-') << endl;

            while ((row = mysql_fetch_row(res))) {
                string hashedPwd = simpleHash(row[5]);
                cout << left
                    << setw(5) << row[0]
                    << setw(15) << row[1]
                    << setw(15) << row[2]
                    << setw(20) << row[3]
                    << setw(15) << row[4]
                    << setw(15) << hashedPwd
                    << setw(8) << row[6] << endl;
            }
        }
        else {
            cout << "Query failed: " << mysql_error(conn) << endl;
        }
        mysql_close(conn);
    }
    else {
        cout << "Connection Failed: " << mysql_error(conn) << endl;
        return;
    }

    int choice;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +======================================+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "  |         << Manage Customer >>        |\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +--------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "  | 1. Update Customer                   |\n";
    cout << "  | 2. Delete Customer                   |\n";
    cout << "  | 3. Back                              |\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +======================================+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "Your option: ";
    cin >> choice;

    if (choice == 1) {
        string cust_id;
        cout << "Enter Customer ID to update: ";
        cin >> cust_id;
        cin.ignore();

        vector<string> fields = { "phone", "address", "status" };
        vector<string> updates;

        while (true) {
            cout << "\n";
            SetConsoleTextAttribute(hConsole, 11);
            cout << " +===========================================================+\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << " |            Select the column you want to update           |\n";
            SetConsoleTextAttribute(hConsole, 11);
            cout << " +===========================================================+\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << " |   1. Phone Number                                         |\n";
            cout << " |   2. Address                                              |\n";
            cout << " |   3. Status (1=Resident, 0=Non-resident)                 |\n";
            cout << " |   4. Done updating                                        |\n";
            SetConsoleTextAttribute(hConsole, 11);
            cout << " +===========================================================+\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << "Enter your choice (1-4): ";

            int opt;
            cin >> opt;
            cin.ignore();

            if (opt == 4) break;

            if (opt < 1 || opt > 3) {
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Invalid option. Try again.\n";
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            string value;
            cout << "Enter new value: ";
            getline(cin, value);

            // Phone number validation (Choice 1)
            if (opt == 1) {
                if (!regex_match(value, regex("^[0-9]{9,11}$"))) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Invalid phone number. Must be 9-11 digits.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
            }

            // Address validation (Choice 2) — basic check
            if (opt == 2) {
                if (value.empty()) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Address cannot be empty.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
            }

            // Status validation (Choice 3)
            if (opt == 3) {
                if (value != "1" && value != "0") {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Invalid status. Use 1 for Resident or 0 for Non-resident.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
            }

            string update = fields[opt - 1] + " = '" + value + "'";
            updates.push_back(update);
        }

        // Execute update
        if (!updates.empty()) {
            string updateQuery = "UPDATE customer SET ";
            for (size_t i = 0; i < updates.size(); ++i) {
                updateQuery += updates[i];
                if (i != updates.size() - 1) updateQuery += ", ";
            }
            updateQuery += " WHERE id = '" + cust_id + "'";
            

            conn = mysql_init(0);
            conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);
            if (conn) {
                qstate = mysql_query(conn, updateQuery.c_str());
                if (!qstate) {
                    cout << "\nCustomer updated successfully.\n";
                }
                else {
                    cout << "Update failed: " << mysql_error(conn) << endl;
                }
                mysql_close(conn);
            }
            else {
                cout << "Connection failed: " << mysql_error(conn) << endl;
            }
        }

        system("pause");
        admincust(); return;

    }
    else if (choice == 2) {
        string cust_id;
        cout << "Enter Customer ID to delete: ";
        cin >> cust_id;

        char confirm;
        cout << "Are you sure you want to delete customer " << cust_id << "? (y/n): ";
        cin >> confirm;
        if (tolower(confirm) == 'y') {
            conn = mysql_init(0);
            conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);
            if (conn) {
                string deleteQuery = "DELETE FROM customer WHERE id = '" + cust_id + "'";
                qstate = mysql_query(conn, deleteQuery.c_str());

                if (!qstate && mysql_affected_rows(conn) > 0) {
                    cout << "Customer deleted successfully.\n";
                }
                else {
                    cout << "Deletion failed or customer not found.\n";
                }
                mysql_close(conn);
            }
            else {
                cout << "Connection failed: " << mysql_error(conn) << endl;
            }
        }
        else {
            cout << "Deletion cancelled.\n";
        }

        system("pause");
        admincust(); return;

    }
    else if (choice == 3) {
        return;
    }
    else {
        cout << "Invalid choice.\n";
        system("pause");
        admincust(); return;
    }
}

void adminfasi()
{
    system("cls");
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate;

    // Initialize connection
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

    if (conn) {
        // Query to get all facilities
        string query = "SELECT * FROM facilities";
        qstate = mysql_query(conn, query.c_str());

        if (!qstate) {
            res = mysql_store_result(conn);

            // Print header
            cout << left
                << setw(12) << "Facility ID"
                << setw(20) << "Name"
                << setw(12) << "Status"
                << setw(15) << "Max Capacity"
                << setw(12) << "Rate (RM)"
                << setw(15) << "Open Time"
                << setw(15) << "Close Time" << endl;
            cout << string(100, '-') << endl;

            // Print each row
            while ((row = mysql_fetch_row(res))) {
                cout << left
                    << setw(12) << row[0]
                    << setw(20) << row[1]
                    << setw(12) << row[2]
                    << setw(15) << row[3]
                    << setw(12) << row[4]
                    << setw(15) << row[5]
                    << setw(15) << row[6] << endl;
            }

        }
        else {
            cout << "Query failed: " << mysql_error(conn) << endl;
        }
    }
    else {
        cout << "Connection Failed: " << mysql_error(conn) << endl;
    }

    // Close connection
    mysql_close(conn);
    int choice;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "\n\n\n";

    // Top border
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << "  +---------------------------------------------+\n";

    // Title
    SetConsoleTextAttribute(hConsole, 14); // Yellow
    cout << "  |           MANAGE FACILITIES                 |\n";

    // Divider
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << "  +---------------------------------------------+\n";

    // Menu options
    SetConsoleTextAttribute(hConsole, 7);  // Default white
    cout << "  | 1. Create Facilities                        |\n";
    cout << "  | 2. Update Facilities                        |\n";
    cout << "  | 3. Delete Facilities                        |\n";
    cout << "  | 4. Back                                     |\n";

    // Bottom border
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << "  +---------------------------------------------+\n";

    // Prompt
    SetConsoleTextAttribute(hConsole, 7); // Reset to white
    cout << "    Enter your choice (1 - 4): ";
    cin >> choice;

    if (choice == 1) {
        MYSQL* conn;
        int qstate;

        // Initialize connection
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (conn) {
            // User input
            string name, status, max_capacity, hourly_rate, opening_time, closing_time;

            // Facility Name (not empty)
            do {
                cout << "Enter Facility Name: ";
                getline(cin >> ws, name);
                if (name.empty()) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Name cannot be empty!\n";
                    SetConsoleTextAttribute(hConsole, 7);
                }
            } while (name.empty());

            // Status validation
            do {
                cout << "Enter Status (Available/Closed): ";
                getline(cin >> ws, status);
                transform(status.begin(), status.end(), status.begin(), ::tolower);
                if (status != "available" && status != "closed") {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Invalid status. Enter 'Available' or 'Closed'.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                }
            } while (status != "available" && status != "closed");

            // Capitalize first letter
            status[0] = toupper(status[0]);

            // Max Capacity validation
            do {
                cout << "Enter Max Capacity: ";
                getline(cin >> ws, max_capacity);
                if (!all_of(max_capacity.begin(), max_capacity.end(), ::isdigit)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Max Capacity must be a number.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    max_capacity = "";
                }
            } while (max_capacity.empty());

            do {
                cout << "Enter Hourly Rate: ";
                getline(cin >> ws, hourly_rate);

                try {
                    stod(hourly_rate); // attempt to convert to double
                }
                catch (...) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Invalid hourly rate. Please enter a numeric value.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    hourly_rate = "";
                }
            } while (hourly_rate.empty());

            // Hourly Rate validation
            regex timeFormat("^([01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d$");
            do {
                cout << "Enter Opening Time (HH:MM:SS): ";
                getline(cin >> ws, opening_time);
                if (!regex_match(opening_time, timeFormat)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Invalid time format. Please use HH:MM:SS (24-hour format).\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    opening_time = "";
                }
            } while (opening_time.empty());

            do {
                cout << "Enter Closing Time (HH:MM:SS): ";
                getline(cin >> ws, closing_time);

                if (!regex_match(closing_time, timeFormat)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Invalid time format. Please use HH:MM:SS (24-hour format).\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    closing_time = "";
                    continue;
                }

            } while (closing_time.empty());
            // SQL query
            stringstream ss;
            ss << "INSERT INTO facilities (name, status, max_capacity, hourly_rate, opening_time, closing_time) VALUES ("
                << "'" << name << "', "
                << "'" << status << "', "
                << "'" << max_capacity << "', "
                << "'" << hourly_rate << "', "
                << "'" << opening_time << "', "
                << "'" << closing_time << "')";

            string query = ss.str();
            const char* q = query.c_str();

            // Execute query
            qstate = mysql_query(conn, q);
            if (!qstate) {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 10);  // Green
                cout << "\nFacility added successfully!\n";
                SetConsoleTextAttribute(hConsole, 7);

                Sleep(2000); // Pause for 2 seconds (2000 milliseconds)

                adminfasi(); // Return to manage facilities menu
                return;      // Prevent fallthrough to the bottom adminfasi() call
            }
            else {
                cout << "Insert failed: " << mysql_error(conn) << endl;
                system("pause");
            }

        }
        else {
            cout << "Connection Failed: " << mysql_error(conn) << endl;
        }

        // Close connection
        mysql_close(conn);
        adminmenu();
    }
    else if (choice == 2) {
        MYSQL* conn;
        int qstate;

        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

        if (conn) {
            string facility_id;
            cout << "Enter Facility ID to update: ";
            cin >> facility_id;

            string checkQuery = "SELECT * FROM facilities WHERE facility_id = '" + facility_id + "'";
            qstate = mysql_query(conn, checkQuery.c_str());
            MYSQL_RES* checkRes = mysql_store_result(conn);

            if (mysql_num_rows(checkRes) == 0) {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 4);
                cout << "\nNo Facility ID found. Please try again with a valid ID.\n";
                SetConsoleTextAttribute(hConsole, 7);
                mysql_free_result(checkRes);
                mysql_close(conn);
                system("pause");
                return; // Exit the update option
            }

            mysql_free_result(checkRes); // Facility exists, continue

            vector<string> columns = { "name", "status", "max_capacity", "hourly_rate", "opening_time", "closing_time" };
            vector<string> updates;
            string openingTimeNew = "";
            string closingTimeNew = "";

            while (true) {
                cout << "\n";
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 11);
                cout << " +===========================================================+\n";
                SetConsoleTextAttribute(hConsole, 7);
                cout << " |            Select the column you want to update           |\n";
                SetConsoleTextAttribute(hConsole, 11);
                cout << " +===========================================================+\n";
                SetConsoleTextAttribute(hConsole, 7);
                cout << " |   1. Name                                                 |\n";
                cout << " |   2. Status                                               |\n";
                cout << " |   3. Max Capacity                                         |\n";
                cout << " |   4. Hourly Rate                                          |\n";
                cout << " |   5. Opening Time (24-hour format HH:MM:SS)               |\n";
                cout << " |   6. Closing Time (24-hour format HH:MM:SS)               |\n";
                cout << " |   7. Done updating                                        |\n";
                SetConsoleTextAttribute(hConsole, 11);
                cout << " +===========================================================+\n";
                SetConsoleTextAttribute(hConsole, 7);
                cout << " |   Enter your choice (1-7): ";

                int choice;
                cin >> choice;

                if (choice == 7) {
                    break;
                }

                if (choice < 1 || choice > 6) {
                 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                 SetConsoleTextAttribute(hConsole, 4);
                 cout << "Invalid choice! Please select between 1 and 7.\n";
                 SetConsoleTextAttribute(hConsole, 7);
                 continue;
                }

                cin.ignore(); // clear buffer
                cout << "Enter the new value: ";
                string new_value;
                getline(cin, new_value);

                // Time validation for Opening and Closing
                if (choice == 5 || choice == 6) {
                    if (!regex_match(new_value, regex("^([01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d$"))) {
                        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Invalid time format! Use 24-hour format (HH:MM:SS).\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        Sleep(2000); //2second
                        continue;
                    }

                    if (choice == 5) {
                        openingTimeNew = new_value;
                    }
                    else {
                        closingTimeNew = new_value;
                    }

                    // Check that closing > opening (only if both entered)
                    if (!openingTimeNew.empty() && !closingTimeNew.empty()) {
                        if (closingTimeNew <= openingTimeNew) {
                            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                            SetConsoleTextAttribute(hConsole, 4);
                            cout << "Closing time must be after opening time!\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            Sleep(2000);
                            continue;
                        }
                    }
                }

                string update_part = columns[choice - 1] + " = '" + new_value + "'";
                updates.push_back(update_part);
            }


            if (updates.empty()) {
                cout << "No updates specified. Exiting.\n";
                mysql_close(conn);

            }

            // Build the final UPDATE query
            string query = "UPDATE facilities SET ";
            for (size_t i = 0; i < updates.size(); i++) {
                query += updates[i];
                if (i != updates.size() - 1) {
                    query += ", ";
                }
            }
            query += " WHERE facility_id = '" + facility_id + "'";

            // Execute the query
            qstate = mysql_query(conn, query.c_str());

            if (!qstate) {
                if (mysql_affected_rows(conn) > 0) {
                    cout << "Facility updated successfully.\n";
                }
                else {
                    cout << "No matching facility found or value unchanged.\n";
                }
            }
            else {
                cout << "Update failed: " << mysql_error(conn) << endl;
            }
        }
        else {
            cout << "Database connection failed: " << mysql_error(conn) << endl;
        }

        mysql_close(conn);

    }
    else if (choice == 3) {
        MYSQL* conn;
        int qstate;

        // Initialize MySQL connection
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

        if (conn) {
            string facility_id;
            cout << "Enter Facility ID to delete: ";
            cin >> facility_id;

            char confirm;
            cout << "Are you sure you want to delete facility ID " << facility_id << "? (y/n): ";
            cin >> confirm;

            if (tolower(confirm) == 'y') {
                // Build and execute DELETE query
                char query[256];
                snprintf(query, sizeof(query),
                    "DELETE FROM facilities WHERE facility_id = '%s'", facility_id.c_str());

                qstate = mysql_query(conn, query);

                if (!qstate) {
                    if (mysql_affected_rows(conn) > 0) {
                        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                        SetConsoleTextAttribute(hConsole, 10);
                        cout << "Facility deleted successfully." << endl;
                        SetConsoleTextAttribute(hConsole, 4);
                        Sleep(2000);
                    }
                    else {
                        cout << "No facility found with ID " << facility_id << "." << endl;
                    }
                }
                else {
                    cout << "Deletion failed: " << mysql_error(conn) << endl;
                }
            }
            else {
                cout << "Deletion canceled." << endl;
            }
        }
        else {
            cout << "Database connection failed: " << mysql_error(conn) << endl;
        }

        mysql_close(conn);
        adminfasi();
    }
    else if (choice == 4) {
        adminmenu();
    }
    else {
        cout << "";
    }
    adminfasi();
}

void adminmanage()
{
    system("cls");
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate;

    // Connect to DB
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

    if (!conn) {
        cout << "Database connection failed.\n";
        return;
    }

    // Updated query: join facilities to get facility name
    const char* query = R"(
    SELECT 
        b.book_id,
        f.name AS facility_name,
        b.name AS customer_name,
        b.cust_id,
        b.date,
        b.pax,
        b.entry_time,
        b.exit_time,
        b.duration,
        b.amount
    FROM booking b
    JOIN facilities f ON b.fasi_id = f.facility_id
    ORDER BY b.date DESC
)";

    if (mysql_query(conn, query) == 0) {
        res = mysql_store_result(conn);

        cout << "\n Booking History:\n";
        cout << "------------------------------------------------------------------------------------------\n";
        cout << left << setw(5) << "ID"
            << setw(20) << "Facility"
            << setw(20) << "Name"
            << setw(12) << "Date"
            << setw(6) << "Pax"
            << setw(10) << "Entry"
            << setw(10) << "Exit"
            << setw(10) << "Hours" << endl;
        cout << "------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << left << setw(5) << row[0]
                << setw(20) << row[1]
                << setw(20) << row[2]
                << setw(12) << row[4]  // skip row[3] (cust_id)
                << setw(6) << row[5]
                << setw(10) << row[6]
                << setw(10) << row[7]
                << setw(10) << row[8] << endl; // skip row[9] (amount)
        }

        mysql_free_result(res);
    }
    else {
        cerr << " Failed to retrieve booking history: " << mysql_error(conn) << endl;
    }



    // --- CRUD Options ---
    int choice;

    cout << "\n\n\n";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);

    cout << "==============================" << endl;
    SetConsoleTextAttribute(hConsole, 7);
    cout << "|      Manage Booking        |" << endl;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "==============================" << endl;
    SetConsoleTextAttribute(hConsole, 7);
    cout << "| 1. Update Booking          |" << endl;
    cout << "| 2. Delete Booking          |" << endl;
    cout << "| 3. Back                    |" << endl;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "==============================" << endl;
    SetConsoleTextAttribute(hConsole, 7);
    cout << "Enter your choice (1 - 3): ";
    cin >> choice;

    if (choice == 1) {
        // UPDATE
        string book_id;
        cout << "Enter Booking ID to update: ";
        cin >> book_id;

        // Validate if booking ID exists
        string checkQuery = "SELECT * FROM booking WHERE book_id = '" + book_id + "'";
        qstate = mysql_query(conn, checkQuery.c_str());

        MYSQL_RES* checkRes = mysql_store_result(conn); // store ONCE

        if (qstate != 0 || checkRes == nullptr || mysql_num_rows(checkRes) == 0) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);  // Red
            cout << "\n No such booking ID found!\n";
            SetConsoleTextAttribute(hConsole, 7);  // Reset
            mysql_free_result(checkRes); // clean up
            mysql_close(conn);
            system("pause");
            adminmanage();
            return;
        }
        

        vector<string> columns = { "fasi_id", "Name", "date", "pax", "entry_time", "exit_time", "duration" };

        int col_choice;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 11);
        cout << "\n\n";
        cout << "===================================================" << endl;
        SetConsoleTextAttribute(hConsole, 7);
        cout << "|     Select the Field to Update                  |" << endl;
        SetConsoleTextAttribute(hConsole, 11);
        cout << "===================================================" << endl;
        SetConsoleTextAttribute(hConsole, 7);
        cout << "| 1. Booking ID                  | 5. Entry Time   |" << endl;
        cout << "| 2. Facility Name               | 6. Exit Time    |" << endl;
        cout << "| 3. Date                        | 7. Duration     |" << endl;
        cout << "| 4. Pax                         | 0. Back         |" << endl;
        SetConsoleTextAttribute(hConsole, 11);
        cout << "===================================================" << endl;
        SetConsoleTextAttribute(hConsole, 7);
        cout << "Enter your choice (0 - 7): ";
        cin >> col_choice;

        if (col_choice == 0) {
            mysql_close(conn);
            adminmanage(); // go back
            return;
        }

        if (col_choice < 1 || col_choice > 7) {
            cout << "Invalid choice.\n";
            mysql_close(conn);
            return;
        }

        cin.ignore(); // clear input buffer
        string new_value;
        cout << "Enter new value for " << columns[col_choice - 1] << ": ";
        getline(cin, new_value);

        // Validate input based on field
        string field = columns[col_choice - 1];
        while (true) {
            if (field == "entry_time" || field == "exit_time") {
                if (!isValidTimeFormat(new_value)) {
                    SetConsoleTextAttribute(hConsole, 4);  // Red
                    cout << " Invalid time format! Use HH:MM:SS (24-hour).\n";
                    SetConsoleTextAttribute(hConsole, 7);  // Reset
                    cout << " Re-enter value: ";
                    getline(cin, new_value);
                    continue;
                }
            }
            else if (field == "date") {
                if (!isValidDateFormat(new_value)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << " Invalid date format! Use YYYY-MM-DD.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << " Re-enter value: ";
                    getline(cin, new_value);
                    continue;
                }
            }
            else if (field == "pax" || field == "duration") {
                if (!isNumber(new_value)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << " Invalid number format. Use numeric values only.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << " Re-enter value: ";
                    getline(cin, new_value);
                    continue;
                }
            }
            else if (new_value.empty()) {
                SetConsoleTextAttribute(hConsole, 4);
                cout << " Value cannot be empty.\n";
                SetConsoleTextAttribute(hConsole, 7);
                cout << " Re-enter value: ";
                getline(cin, new_value);
                continue;
            }
            break;  // All validations passed
        }

        // Perform update
        string query = "UPDATE booking SET " + field + " = '" + new_value + "' WHERE book_id = '" + book_id + "'";
        qstate = mysql_query(conn, query.c_str());

        if (!qstate && mysql_affected_rows(conn) > 0)
            cout << " Booking updated successfully.\n";
        else
            cout << " Update failed: " << mysql_error(conn) << endl;
    }
    else if (choice == 2) {
        // DELETE
        string book_id;
        cout << "Enter Booking ID to delete: ";
        cin >> book_id;

        char confirm;
        cout << "Are you sure you want to delete booking ID " << book_id << "? (y/n): ";
        cin >> confirm;

        if (tolower(confirm) == 'y') {
            string query = "DELETE FROM booking WHERE book_id = '" + book_id + "'";
            qstate = mysql_query(conn, query.c_str());

            if (!qstate && mysql_affected_rows(conn) > 0)
                cout << "Booking deleted successfully.\n";
            else
                cout << "Deletion failed: " << mysql_error(conn) << endl;
        }
        else {
            cout << "Deletion cancelled.\n";
        }
    }
    else if (choice == 3) {
        mysql_close(conn);
        adminmenu();
        return;
    }

    mysql_close(conn);
    system("pause");
    adminmanage(); // Re-open after action
}




//CUSTOMER

void Register()
{
    system("cls");
    string name, phone, username, password, add;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
    cout << "                                    +=========================================================+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "                                    |                CUSTOMER REGISTRATION                    |\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "                                    +=========================================================+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cin.ignore();

    cout << "                (1)  Enter Name: ";
    getline(cin, name);

    cout << "                (2)  Enter Address: ";
    getline(cin, add);

    while (true) {
        cout << "                (3)  Enter Phone Number (9-11 digits only): ";
        getline(cin, phone);

        if (isValidPhone(phone)) break;

        SetConsoleTextAttribute(hConsole, 4); // Red
        cout << "                |  Invalid phone number. Try again.\n";
        SetConsoleTextAttribute(hConsole, 7); // Reset to default
    }

    cout << "                (4)  Enter Username: ";
    getline(cin, username);

    while (true) {
        cout << "                (5)  Enter Password: ";
        getline(cin, password);

        if (isValidPassword(password)) {
            SetConsoleTextAttribute(hConsole, 10); // Green
            cout << "                            |  PASSWORD IS VALID.\n";
            SetConsoleTextAttribute(hConsole, 7); // Reset
            break;
        }
        else {
            SetConsoleTextAttribute(hConsole, 4); // Red
            cout << "                |  Password must contain uppercase, lowercase, digit,\n";
            cout << "                |  and special character. Try again.\n";
            SetConsoleTextAttribute(hConsole, 7); // Reset
        }
    }
 
    // Check username existence
    string checkUser_query = "SELECT * FROM customer WHERE Username = '" + username + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            cout << "                |   Username already exists. Try again...\n";
            system("pause");
            Register(); // re-register
        }
        else
        {
            string Register_query =
                "INSERT INTO customer(Name, Phone, `Add`, Username, Password) VALUES('" +
                name + "','" + phone + "','" + add + "','" + username + "','" + password + "')";

            const char* q = Register_query.c_str();
            qstate = mysql_query(conn, q);

            if (!qstate)
            {
                cout << "\n\n";
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "                        +=========================================================+\n";
                cout << "                        |             You have been registered successfully!      |\n";
                cout << "                        |                   WELCOME TO CLUBHOUSE, '" << name << "'     \n";
                cout << "                        +=========================================================+\n";
                SetConsoleTextAttribute(hConsole, 7);
            }
            else
            {
                cout << " |  Registration failed! Error: " << mysql_errno(conn) << "\n";
            }
        }
    }
    else
    {
        cout << "|  Query failed! Error: " << mysql_errno(conn) << "\n";
    }

    system("pause");
    CustLogin();
}

void CustLogin() {
    system("cls");
    string username, password;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11); // 11 = Cyan
    cout << "================= [ ";

    // Yellow text
    SetConsoleTextAttribute(hConsole, 14); // 14 = Yellow
    cout << "CUSTOMER LOGIN ACCESS PANEL";

    // Cyan closing line
    SetConsoleTextAttribute(hConsole, 11); // back to Cyan
    cout << " ] =================\n\n";

    // Reset to default
    SetConsoleTextAttribute(hConsole, 7);

    cin.ignore();
    cout << "> Username: ";
    getline(cin, username);
    password = getMaskedPassword();  // Make sure this does NOT print its own prompt

    string query = "SELECT ID, Username FROM customer WHERE Username = '" + username + "' AND Password = '" + password + "'";
    const char* q = query.c_str();

    if (mysql_query(conn, q) == 0) {
        res = mysql_store_result(conn);

        if (res && mysql_num_rows(res) == 1) {
            row = mysql_fetch_row(res);
            SessionCustomerID = row[0]; // Store customer ID in session
            string dbUsername = row[1];

            cout << "\n------------------------------------------------------------\n";
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 11);
            cout << ">> AUTHENTICATION SUCCESSFUL\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << ">> USER LOGGED IN AS: " << dbUsername << "\n";
            cout << "------------------------------------------------------------\n\n";
            SetConsoleTextAttribute(hConsole, 10);
            cout << "[System Status] :: Online\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << "[Redirecting to Dashboard...]\n";

            mysql_free_result(res);
            Sleep(3000); // 3 second pause
            customermenu(); // Proceed to customer menu
            return;
        }
    }

    // Failed login
    cout << "\n------------------------------------------------------------\n";
    SetConsoleTextAttribute(hConsole, 4);
    cout << ">> AUTHENTICATION FAILED\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << ">> Invalid username or password.\n";
    cout << "------------------------------------------------------------\n";

    if (res) mysql_free_result(res);
    Sleep(2000); //2 second pause
}

void custmain1() {
    system("cls");
    system("title Clubhouse Management System");
    db_response::ConnectionFunction();

    int choice;
    while (true)
    {
        system("cls");

        // Top-Middle Box
        string pad = "                 "; // 16 spaces of horizontal padding
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 11);
        cout << pad << "                     +===============================================+\n";
        SetConsoleTextAttribute(hConsole, 7);
        cout << pad << "                     |                                               |\n";
        cout << pad << "                     |     Welcome To Clubhouse Management System    |\n";
        cout << pad << "                     |                                               |\n";
        cout << pad << "                     |     1. Customer Login                         |\n";
        cout << pad << "                     |     2. Register                               |\n";
        cout << pad << "                     |     3. Back                                   |\n";
        cout << pad << "                     |                                               |\n";
        SetConsoleTextAttribute(hConsole, 11);
        cout << pad << "                     +===============================================+\n";
        SetConsoleTextAttribute(hConsole, 7);
        cout << pad << "                                  Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            CustLogin();
            break;
        case 2:
            Register();
            break;
        case 3:
            return; // go back to previous menu
        default:
            system("cls");
            cout << "\n\n";
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);
            cout << pad <<         "+-------------------------------------------+\n";
            cout << pad <<         "|       INVALID CHOICE. PLEASE TRY AGAIN    |\n";
            cout << pad <<         "+-------------------------------------------+\n";
            SetConsoleTextAttribute(hConsole, 7);
            Sleep(1000); // 1 second pause
        }
    }
}


int getInt() {
    string input;
    int number;
    while (true) {
        getline(cin, input);
        try {
            number = stoi(input);
            return number;
        }
        catch (exception& e) {
            cout << "Invalid input. Please enter a valid number: ";
        }
    }
}

void customermenu()
{

    system("cls"); // Clears the screen


    //session code
    string customerID;
    string customerName;
    string customerUsername;
    string customerEmail;
    string customerPhone;
    
    string query1 = "SELECT ID, Name, Username, Phone FROM customer WHERE ID = '" + SessionCustomerID + "'";
    const char* q1 = query1.c_str();
    if (mysql_query(conn, q1) != 0) {
        cerr << "Query failed: " << mysql_error(conn) << endl;
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    if (res && mysql_num_rows(res) == 1) {
        row = mysql_fetch_row(res);
        customerID = row[0];
        customerName = row[1];
        customerUsername = row[2];
        customerPhone = row[3];
    }
    else {
        cout << "Customer details not found." << endl;
    }




    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 11); // Cyan 
    cout << " +==============================================================+\n";
    cout << " |             CUSTOMER MENU (ID: " << customerName << ")             |\n";
    cout << " +==============================================================+\n";

    SetConsoleTextAttribute(hConsole, 7); // Reset 
    cout << " |  1. View Facilities                                         |\n";
    cout << " |  2. Book a Facility                                         |\n";
    cout << " |  3. View Booking History                                    |\n";
    cout << " |  4. Cancel Booking                                          |\n";
    cout << " |  0. Logout                                                  |\n";
    cout << " +==============================================================+\n";
    cout << " |  Enter your choice: ";

    int choice1;
    choice1 = getInt();  

    if (choice1 == 1) {
        viewFacilities();
    }
    else if (choice1 == 2) {
        makeBooking(conn);
    }
    else if (choice1 == 3) {
        viewBookingHistory(conn);
    }
    else if (choice1 == 4) {
        cancelbooking();
    }
    else if (choice1 == 0) {
        custmain1();
        return;
    }
    else {
        cout << "Invalid choice. Try again." << endl;
    }

    //system("pause"); // Shows "Press any key to continue..." on Windows
}

void viewFacilities()
{
    system("cls");
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate;

    // Initialize connection
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

    if (conn) {
        // Query to get all facilities
        string query = "SELECT * FROM facilities";
        qstate = mysql_query(conn, query.c_str());

        if (!qstate) {
            res = mysql_store_result(conn);

            // Print header
            cout << left
                << setw(12) << "Facility ID"
                << setw(20) << "Name"
                << setw(12) << "Status"
                << setw(15) << "Max Capacity"
                << setw(12) << "Rate"
                << setw(15) << "Open Time"
                << setw(15) << "Close Time" << endl;
            cout << string(100, '-') << endl;

            // Print each row
            while ((row = mysql_fetch_row(res))) {
                cout << left
                    << setw(12) << row[0]
                    << setw(20) << row[1]
                    << setw(12) << row[2]
                    << setw(15) << row[3]
                    << setw(12) << row[4]
                    << setw(15) << row[5]
                    << setw(15) << row[6] << endl;
            }
            //customermenu();
        }
        else {
            cout << "Query failed: " << mysql_error(conn) << endl;
        }
    }
    else {
        cout << "Connection Failed: " << mysql_error(conn) << endl;
    }

    // Close connection
    mysql_close(conn);
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
    customermenu();

}

// Function to insert a booking
void makeBooking(MYSQL* con) {
    system("cls");

    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;
    char query[1024];

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

    if (!conn) {
        cout << "Database connection failed: " << mysql_error(conn) << endl;
        return;
    }

    system("cls"); // Clears the screen


    //session code
    string customerID;
    string customerName;
    string customerUsername;
    string customerEmail;
    string customerPhone;
    int status;
    string query1 = "SELECT ID, Name, Username, Phone,status  FROM customer WHERE ID = '" + SessionCustomerID + "'";
    const char* q1 = query1.c_str();
    if (mysql_query(conn, q1) != 0) {
        cerr << "Query failed: " << mysql_error(conn) << endl;
        return;
    }
    MYSQL_RES* res2 = mysql_store_result(conn);
    MYSQL_ROW row2;

    if (res2 && mysql_num_rows(res2) == 1) {
        row2 = mysql_fetch_row(res2);
        customerID = row2[0];
        customerName = row2[1];
        customerUsername = row2[2];
        customerPhone = row2[3];
        status = stoi(row2[4]);
    }
    else {
        cout << "Customer details not found." << endl;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n";
    cout << "                       +=========================================================+\n";
    cout << "                       |          GENERAL INFORMATION OF FACILITY (DAILY)        |\n";
    cout << "                       +=========================================================+\n";
    SetConsoleTextAttribute(hConsole, 7);
    // Display all facilities
    const char* getFacilities = "SELECT * FROM facilities";
    if (mysql_query(conn, getFacilities) == 0) {
        res = mysql_store_result(conn);

        cout << left
            << setw(12) << "Facility ID"
            << setw(20) << "Name"
            << setw(12) << "Status"
            << setw(15) << "Max Capacity"
            << setw(12) << "Rate"
            << setw(15) << "Open Time"
            << setw(15) << "Close Time" << endl;
        cout << string(115, '-') << endl;

        while ((row = mysql_fetch_row(res))) {
            string facility_id = row[0];
            int max_capacity = atoi(row[3]);
            int total_booked = 0;

            // Query to get sum of pax for this facility
            string paxQuery = "SELECT IFNULL(SUM(pax), 0) FROM booking WHERE fasi_id = '" + facility_id + "'";
            if (mysql_query(conn, paxQuery.c_str()) == 0) {
                MYSQL_RES* paxRes = mysql_store_result(conn);
                MYSQL_ROW paxRow = mysql_fetch_row(paxRes);
                if (paxRow) {
                    total_booked = atoi(paxRow[0]);
                }
                mysql_free_result(paxRes);
            }

            int remaining = max_capacity - total_booked;

            cout << left
                << setw(12) << row[0]
                << setw(20) << row[1]
                << setw(12) << row[2]
                << setw(15) << row[3]
                << setw(12) << row[4]
                << setw(15) << row[5]
                << setw(15) << row[6] << endl;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return;
    }

    char fasi_id[10], cust_id[10], date[11], payment_method[20];
    int max_capacity = 0, current_total_pax = 0, pax;
    string facility_name;
    double hourly_rate = 0.0, total_hours = 0.0, total_cost = 0.0;

    while (true) {
        cout << "\nEnter Facility ID (or type '0' to go back): ";
        cin >> fasi_id;

        if (strcmp(fasi_id, "0") == 0) {
            mysql_close(conn);
            customermenu(); // go back to main customer menu
            return;
        }

        snprintf(query, sizeof(query),
            "SELECT max_capacity, hourly_rate, name, status FROM facilities WHERE facility_id = '%s'", fasi_id);

        if (mysql_query(conn, query) != 0) {
            cout << "Facility lookup failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            row = mysql_fetch_row(res);
            max_capacity = atoi(row[0]);
            hourly_rate = atof(row[1]);
            facility_name = row[2];
            string facility_status = row[3];
            mysql_free_result(res);

            if (facility_status == "Closed" || facility_status == "closed") {
                SetConsoleTextAttribute(hConsole, 4);  // Red
                cout << "\nThis facility is currently CLOSED and cannot be booked.\n";
                SetConsoleTextAttribute(hConsole, 7);  // Reset
                cin.ignore();
                cin.get();
                customermenu(); // return to customer menu
                return;
            }
            break;
        }

        SetConsoleTextAttribute(hConsole, 4);
        cout << "       Invalid Facility ID. Please try again.\n";
        cout << "+=========================================================+\n";

        SetConsoleTextAttribute(hConsole, 7);
        if (res) mysql_free_result(res);
    }

    while (true) {
        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> date;

        // 1. Format validation using regex
        if (!regex_match(date, regex("^\\d{4}-\\d{2}-\\d{2}$"))) {
            SetConsoleTextAttribute(hConsole, 4); // Red
            cout << "Invalid format. Please use YYYY-MM-DD.\n";
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        // 2. Valid calendar date check using get_time
        struct tm tm = {};
        istringstream ss(date);
        ss >> get_time(&tm, "%Y-%m-%d");

        if (ss.fail()) {
            SetConsoleTextAttribute(hConsole, 4); // Red
            cout << "Invalid date. Please enter a valid calendar date.\n";
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        break; // Valid date
    }

    string entry_time, exit_time;
    while (true) {
        cout << "Enter Entry Time (HH:MM:SS): ";
        cin >> entry_time;
        if (!isValidTimeFormat(entry_time)) {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Invalid format. Please use HH:MM:SS (24-hour format).\n";
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        cout << "Enter Exit Time (HH:MM:SS): ";
        cin >> exit_time;
        if (!isValidTimeFormat(exit_time)) {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Invalid format. Please use HH:MM:SS (24-hour format).\n";
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        istringstream ss1(entry_time), ss2(exit_time);
        struct tm t1 = {}, t2 = {};
        t1.tm_year = t2.tm_year = 100;
        t1.tm_mon = t2.tm_mon = 0;
        t1.tm_mday = t2.tm_mday = 1;
        ss1 >> get_time(&t1, "%H:%M:%S");
        ss2 >> get_time(&t2, "%H:%M:%S");

        time_t start = mktime(&t1), end = mktime(&t2);
        if (difftime(end, start) <= 0) {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Exit time must be after entry time.\n";
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        total_hours = difftime(end, start) / 3600.0;
        break;
    }

    //  INSERT THIS BLOCK HERE
    int remaining_capacity = 0;

    // Get facility info
    // Calculate total bookings for the selected facility, date, and time slot
    
    // later, just use them — no re-declaration
 

    snprintf(query, sizeof(query),
        "SELECT IFNULL(SUM(pax), 0) FROM booking WHERE fasi_id = '%s' AND date = '%s' "
        "AND ((entry_time < '%s' AND exit_time > '%s') OR "
        "     (entry_time >= '%s' AND entry_time < '%s'))",
        fasi_id, date, exit_time.c_str(), entry_time.c_str(), entry_time.c_str(), exit_time.c_str());

    if (mysql_query(conn, query) != 0) {
        cerr << "Failed to calculate slot usage: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return;
    }

    res = mysql_store_result(conn);
    if ((row = mysql_fetch_row(res))) {
        current_total_pax = atoi(row[0]);
        remaining_capacity = max_capacity - current_total_pax;
    }
    mysql_free_result(res);

    // Show available slots for selected time
    cout << "\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +---------------------------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "  |            Available Capacity for Selected Time Slot               |\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +---------------------------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "  | Facility       : " << setw(52) << left << facility_name << "|\n";
    cout << "  | Date           : " << setw(52) << left << date << "|\n";
    cout << "  | Time Slot      : " << setw(52) << left << (entry_time + " to " + exit_time) << "|\n";
    cout << "  | Max Capacity   : " << setw(52) << left << max_capacity << "|\n";
    cout << "  | Already Booked : " << setw(52) << left << current_total_pax << "|\n";
    cout << "  | Remaining Slots: " << setw(52) << left << remaining_capacity << "|\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +---------------------------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);

    while (true) {
        cout << "Enter number of people (pax): ";
        cin >> pax;


        if (cin.fail() || pax <= 0) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);
            cout << "INVALID PAX. Please enter a positive number.\n";
            cout << "----------------------------------------------\n\n";
            SetConsoleTextAttribute(hConsole, 7);
            cin.clear(); // clear error flags
            cin.ignore(1000, '\n'); // discard invalid input
            continue;
        }

        // Check remaining capacity
        snprintf(query, sizeof(query),
            "SELECT IFNULL(SUM(pax), 0) FROM booking WHERE fasi_id = '%s' AND date = '%s'",
            fasi_id, date);

        if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            current_total_pax = atoi(row[0]);
            mysql_free_result(res);

            int remaining_capacity = max_capacity - current_total_pax;

            if (pax > remaining_capacity) {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 4);
                cout << "NOT ENOUGH CAPACITY. Only " << remaining_capacity << " spots left.\n";
                cout << "----------------------------------------------\n\n";
                SetConsoleTextAttribute(hConsole, 7);
                continue; // let user try again
            }

            break; // valid pax
        }
        else {
            cout << "Failed to check bookings: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }
    }

    snprintf(query, sizeof(query),
        "SELECT IFNULL(SUM(pax), 0) FROM booking WHERE fasi_id = '%s' AND date = '%s'",
        fasi_id, date);

    if (mysql_query(conn, query) == 0) {
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        current_total_pax = atoi(row[0]);
        mysql_free_result(res);

        int remaining_capacity = max_capacity - current_total_pax;

        if (pax > remaining_capacity) {
            cout << "Not enough capacity. Only " << remaining_capacity << " spots left.\n";
            mysql_close(conn);
            makeBooking(conn);
            return;
        }
    }
    else {
        cout << "Failed to check bookings: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return;
    }

    time_t start = 0, end = 0; // Safe initialization

    while (true) {
        cout << "Enter Entry Time (HH:MM:SS): ";
        cin >> entry_time;

        istringstream ss1(entry_time);
        struct tm t1 = {};
        t1.tm_year = 100;  // 2000 (year - 1900)
        t1.tm_mon = 0;     // January
        t1.tm_mday = 1;
        ss1 >> get_time(&t1, "%H:%M:%S");

        if (ss1.fail()) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);
            cout << "INVALID ENTRY TIME FORMAT.\n";
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        cout << "Enter Exit Time (HH:MM:SS): ";
        cin >> exit_time;

        istringstream ss2(exit_time);
        struct tm t2 = {};
        t2.tm_year = 100;  // 2000
        t2.tm_mon = 0;     // January
        t2.tm_mday = 1;
        ss2 >> get_time(&t2, "%H:%M:%S");

        if (ss2.fail()) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);
            cout << "INVALID EXIT TIME FORMAT.\n";
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        start = mktime(&t1);
        end = mktime(&t2);

        if (difftime(end, start) <= 0) {
            cout << "Exit time must be after entry time.\n";
            continue;
        }

        total_hours = difftime(end, start) / 3600.0;
        break;
    }

    total_hours = difftime(end, start) / 3600.0;
    int cost = total_hours * hourly_rate;
    string text;
    if (status > 0) {
        total_cost = cost * 0.85;
        text = "Resident Discount 15% is applied";
    }
    else {
        total_cost = cost;
        text = "";
    }

    int total_seconds = static_cast<int>(difftime(end, start));
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    total_hours = total_seconds / 3600.0; // Still needed for cost

    ostringstream formattedDuration;
    formattedDuration << hours << " hour(s) " << minutes << " minute(s)";

    cout << "\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +---------------------------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "  |                           Booking Summary                           |\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +---------------------------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "  | Facility       : " << setw(52) << left << facility_name << "|\n";
    cout << "  | Date           : " << setw(52) << left << date << "|\n";
    cout << "  | Time           : " << setw(52) << left << (entry_time + " to " + exit_time) << "|\n";
    cout << "  | Duration       : " << setw(52) << left << formattedDuration.str() << "|\n";
    cout << "  | Rate/hour      : RM" << setw(50) << left << hourly_rate << "|\n";
    cout << "  | Total Cost     : RM" << setw(50) << left << total_cost << "|\n";
    cout << "  | " << setw(67) << left << text << "|\n";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  +---------------------------------------------------------------------+\n";
    SetConsoleTextAttribute(hConsole, 7);

    // Final INSERT query
    snprintf(query, sizeof(query),
        "INSERT INTO booking (fasi_id, Name, date, cust_id, pax, entry_time, exit_time, duration, amount) "
        "VALUES ('%s', '%s', '%s', '%s', %d, '%s', '%s', '%.2f', '%.2f')",
        fasi_id, customerName, date, customerID, pax, entry_time.c_str(), exit_time.c_str(),
        total_hours, total_cost);

    if (mysql_query(conn, query) != 0) {
        cerr << "Insert failed: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Booking successful and stored in the database." << endl;
        //customermenu();
    }

    mysql_close(conn);
    cin.ignore();
    cin.get();
    customermenu();
}

// Function to view booking history
void viewBookingHistory(MYSQL* conn) {
    system("cls");
    stringstream ss;
    ss << "SELECT b.book_id, b.name, f.name, b.date, b.pax, b.entry_time, b.exit_time "
        << "FROM booking b "
        << "JOIN facilities f ON b.fasi_id = f.facility_id "
        << "WHERE b.cust_id = " << SessionCustomerID << " ORDER BY b.date DESC";

    string query = ss.str();

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n Booking History:\n";
        cout << "-------------------------------------------------------------------------------\n";
        cout << left << setw(10) << "BookingID"
            << setw(15) << "Name"
            << setw(15) << "Facility"
            << setw(12) << "Date"
            << setw(6) << "Pax"
            << setw(10) << "Entry"
            << setw(10) << "Exit" << endl;
        cout << "-------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << left << setw(10) << row[0]  // Booking ID
                << setw(15) << row[1]          // Customer Name
                << setw(15) << row[2]          // Facility Name
                << setw(12) << row[3]          // Date
                << setw(6) << row[4]          // Pax
                << setw(10) << row[5]          // Entry Time
                << setw(10) << row[6] << endl; // Exit Time
        }

        mysql_free_result(res);

        string input;
        int selected_id;

        cout << "\nEnter a Booking ID to view receipt (0 to return): ";
        getline(cin, input);

        try {
            selected_id = stoi(input);  // Convert input to integer

            if (selected_id > 0) {
                viewBookingReceipt(conn, selected_id);
            }
            else {
                customermenu(); // Go back to main customer menu
            }
        }
        catch (invalid_argument&) {
            cout << "Invalid input! Please enter a valid number.\n";
            cin.get();
            viewBookingHistory(conn); // Retry
        }

    }
    else {
        cerr << " Failed to retrieve booking history: " << mysql_error(conn) << endl;
    }

    cout << "\nPress Enter to return to menu...";
    cin.ignore();
    cin.get();
    customermenu();
}

void cancelbooking()
{
    system("cls");
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate;

    // Connect to DB
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "restoranaliff", 3306, NULL, 0);

    if (!conn) {
        cout << "Database connection failed.\n";
        return;
    }


    stringstream ss;
    ss << "SELECT * FROM booking WHERE cust_id = " << SessionCustomerID << " ORDER BY date DESC";
    string query = ss.str();
    const char* q = query.c_str();

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14); // Yellow
        cout << "\nBooking History:\n";

        // Set color for top border line
        SetConsoleTextAttribute(hConsole, 11); // Cyan
        cout << "-------------------------------------------------------------------------------------------------------------\n";

        // Center the word "Name" in 20 characters
        string nameHeader = "Name";
        int nameWidth = 20;
        int leftPad = (nameWidth - nameHeader.length()) / 2;
        int rightPad = nameWidth - nameHeader.length() - leftPad;
        string centeredName = string(leftPad, ' ') + nameHeader + string(rightPad, ' ');

        // Set color for headers
        SetConsoleTextAttribute(hConsole, 14); // Yellow
        cout << left
            << setw(5) << "ID"
            << setw(12) << "Facility ID"
            << centeredName
            << setw(13) << "Customer ID"
            << setw(12) << "Date"
            << setw(6) << "Pax"
            << setw(10) << "Entry"
            << setw(10) << "Exit"
            << setw(10) << "Duration"
            << setw(10) << "Amount" << endl;

        // Set color for bottom border line
        SetConsoleTextAttribute(hConsole, 11); // Cyan
        cout << "-------------------------------------------------------------------------------------------------------------\n";

        // Reset to default color
        SetConsoleTextAttribute(hConsole, 7);

        while ((row = mysql_fetch_row(res))) {
            string name = row[2];
            int nameWidth = 20;
            int leftPad = (nameWidth - name.length()) / 2;
            int rightPad = nameWidth - name.length() - leftPad;

            cout << left
                << setw(5) << row[0]
                << setw(12) << row[1]
                << string(leftPad, ' ') << name << string(rightPad, ' ')
                << setw(13 - nameWidth) << ""  // adjust spacing to line up next column
                << setw(13) << row[3]
                << setw(12) << row[4]
                << setw(6) << row[5]
                << setw(10) << row[6]
                << setw(10) << row[7]
                << setw(10) << row[8]
                << "RM " << row[9] << endl;
        }

        mysql_free_result(res);
    }
    else {
        cerr << "Failed to retrieve booking history: " << mysql_error(conn) << endl;
    }



    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice;

    // Title
    SetConsoleTextAttribute(hConsole, 14); // Yellow
    cout << "\n";

    // Box Top
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << "  +===========================================+\n";

    // Title Inside Box
    SetConsoleTextAttribute(hConsole, 14); // Yellow
    cout << "  |             << Your Booking >>            |\n";

    // Box Divider
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << "  +-------------------------------------------+\n";

    // Options
    SetConsoleTextAttribute(hConsole, 7); // Default color (white/gray)
    cout << "  | 1. Cancel Booking                         |\n";
    cout << "  | 2. Back                                   |\n";

    // Box Bottom
    SetConsoleTextAttribute(hConsole, 11); // Cyan
    cout << "  +===========================================+\n";

    // Prompt
    SetConsoleTextAttribute(hConsole, 7); // Default
    cout << "  Enter your choice: ";
    cin >> choice;


    if (choice == 1) {
        // DELETE
        string book_id;
        cout << "Enter Booking ID to Cancel: ";
        cin >> book_id;

        char confirm;
        cout << "Are you sure you want to Cancel booking ID " << book_id << "? (y/n): ";
        cin >> confirm;

        if (tolower(confirm) == 'y') {
            string query = "DELETE FROM booking WHERE book_id = '" + book_id + "'";
            qstate = mysql_query(conn, query.c_str());

            if (!qstate && mysql_affected_rows(conn) > 0)
                cout << "Booking Cancel successfully.\n";
            else
                cout << "Cancelation failed: " << mysql_error(conn) << endl;
        }
        else {
            cout << "Cancelation cancelled.\n";
        }
    }
    else if (choice == 2) {
        mysql_close(conn);
        //customermenu();
        return;
    }

    mysql_close(conn);
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
    customermenu();
}


void viewBookingReceipt(MYSQL* conn, int booking_id) {
    system("cls");

    stringstream ss;
    ss << "SELECT b.name, f.name, b.date, b.entry_time, b.exit_time, b.duration, b.amount  "
        << "FROM booking b "
        << "JOIN facilities f ON b.fasi_id = f.facility_id "
        << "WHERE b.book_id = " << booking_id;

    string query = ss.str();

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 2);

        cout << R"(

                               _____   ______  _____  ______  _____  _____  _______ 
                              |  __ \ |  ____|/ ____||  ____||_   _||  __ \|__   __|
                              | |__) || |__  | |     | |__     | |  | |__) |  | |   
                              |  _  / |  __| | |     |  __|    | |  |  ___/   | |   
                              | | \ \ | |____| |____ | |____  _| |_ | |       | |   
                              |_|  \_\|______|\_____||______||_____||_|       |_|  
                                                                                
)";
        SetConsoleTextAttribute(hConsole, 7);

        if (row) {
            SetConsoleTextAttribute(hConsole, 2);
            cout << "\n                           ======================================\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << "                                  Booking Receipt\n";
            SetConsoleTextAttribute(hConsole, 2);
            cout << "                            ======================================\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << "                            Name         : " << row[0] << "\n";
            cout << "                            Facility     : " << row[1] << "\n";
            cout << "                            Date         : " << row[2] << "\n";
            cout << "                            Entry Time   : " << row[3] << "\n";
            cout << "                            Exit Time    : " << row[4] << "\n";
            cout << "                            Duration     : " << fixed << setprecision(2) << atof(row[5]) << " hours\n";
            cout << "                            Amount Paid  : RM " << fixed << setprecision(2) << atof(row[6]) << "\n";
            SetConsoleTextAttribute(hConsole, 2);
            cout << "                            ======================================\n";
            SetConsoleTextAttribute(hConsole, 7);
            cout << "                            Thank you for your booking!\n";
            SetConsoleTextAttribute(hConsole, 2);
            cout << "                            ======================================\n";
            SetConsoleTextAttribute(hConsole, 7);
        }
        else {
            cout << " No booking found with ID " << booking_id << ".\n";
        }

        mysql_free_result(res);
    }
    else {
        cerr << " Error retrieving receipt: " << mysql_error(conn) << endl;
    }

    cout << "\nPress Enter to go back...";
    cin.ignore();
    cin.get();
    viewBookingHistory(conn);  // Optional: return to booking list
}





