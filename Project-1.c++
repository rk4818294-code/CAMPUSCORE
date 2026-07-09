#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <limits>
#include <conio.h>
#include <windows.h>

using namespace std;

//=============================
// Student Structure
//=============================

struct Student
{
    int roll;
    string name;
    int age;
    string course;
    float marks;
};

vector<Student> students;

//=============================
// Console Color
//=============================

void color(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//=============================
// Pause Screen
//=============================

void pauseScreen()
{
    cout << "\n\nPress any key to continue...";
    _getch();
}

//=============================
// Welcome Screen
//=============================

void welcome()
{
    system("cls");

    color(11);

    cout << "==============================================================\n";
    cout << "              CAMPUSCORE MANAGEMENT SYSTEM\n";
    cout << "==============================================================\n";

    color(7);
}

//=============================
// Main Menu Header
//=============================

void menuHeader()
{
    system("cls");

    color(11);

    cout << "==============================================================\n";
    cout << "              CAMPUSCORE MANAGEMENT SYSTEM\n";
    cout << "==============================================================\n";

    color(14);
    cout << "Logged In As : ADMIN\n";

    color(7);

    cout << "--------------------------------------------------------------\n";
    cout << "1.  Add Student\n";
    cout << "2.  Delete Student\n";
    cout << "3.  Update Student\n";
    cout << "4.  Search By Roll\n";
    cout << "5.  Search By Name\n";
    cout << "6.  View All Students\n";
    cout << "7.  Sort By Name\n";
    cout << "8.  Sort By Marks\n";
    cout << "9.  Generate Report\n";
    cout << "10. Save Data\n";
    cout << "11. Exit\n";
    cout << "--------------------------------------------------------------\n";
}

//=============================
// Login
//=============================

bool login()
{
    string username;
    string password = "";

    char ch;

    while (true)
    {
        welcome();

        cout << "\nUsername : ";
        cin >> username;

        cout << "Password : ";

        password.clear();

        while (true)
        {
            ch = _getch();

            if (ch == 13)
                break;

            else if (ch == 8)
            {
                if (!password.empty())
                {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                password.push_back(ch);
                cout << "*";
            }
        }

        if (username == "admin" && password == "1234")
        {
            color(10);
            cout << "\n\nLogin Successful...";
            color(7);
            Sleep(1000);
            return true;
        }

        color(12);
        cout << "\n\nInvalid Username or Password!";
        color(7);

        pauseScreen();
    }
}

//=============================
// Save File
//=============================

void saveToFile()
{
    ofstream fout("students.txt");

    for (auto &s : students)
    {
        fout << s.roll << endl;
        fout << s.name << endl;
        fout << s.age << endl;
        fout << s.course << endl;
        fout << s.marks << endl;
    }

    fout.close();
}

//=============================
// Load File
//=============================

void loadFromFile()
{
    students.clear();

    ifstream fin("students.txt");

    if (!fin)
        return;

    Student s;

    while (fin >> s.roll)
    {
        fin.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(fin, s.name);

        fin >> s.age;
        fin.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(fin, s.course);

        fin >> s.marks;

        students.push_back(s);
    }

    fin.close();
}

//=============================
// Duplicate Roll Check
//=============================

bool rollExists(int roll)
{
    for (auto &s : students)
    {
        if (s.roll == roll)
            return true;
    }

    return false;
}

//=============================
// Grade
//=============================

string getGrade(float marks)
{
    if (marks >= 90)
        return "A+";

    else if (marks >= 80)
        return "A";

    else if (marks >= 70)
        return "B";

    else if (marks >= 60)
        return "C";

    else if (marks >= 50)
        return "D";

    return "FAIL";
}
//==========================================================
// ADD STUDENT
//==========================================================

void addStudent()
{
    int choice;

    do
    {
        system("cls");
        menuHeader();

        cout << "\n=========== ADD STUDENT ===========\n\n";

        Student s;

        cout << "Enter Roll Number : ";
        cin >> s.roll;

        if (rollExists(s.roll))
        {
            cout << "\nRoll Number Already Exists!\n";
        }
        else
        {
            cin.ignore();

            cout << "Enter Name : ";
            getline(cin, s.name);

            cout << "Enter Age : ";
            cin >> s.age;

            cin.ignore();

            cout << "Enter Course : ";
            getline(cin, s.course);

            cout << "Enter Marks : ";
            cin >> s.marks;

            students.push_back(s);
            saveToFile();

            cout << "\nStudent Added Successfully.\n";
        }

        cout << "\n---------------------------------\n";
        cout << "1. Add Another Student\n";
        cout << "2. Back To Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter Choice : ";
        cin >> choice;

    } while (choice == 1);
}
//==========================================================
// DELETE STUDENT
//==========================================================

void deleteStudent()
{
    int choice;

    do
    {
        system("cls");
        menuHeader();

        cout << "\n=========== DELETE STUDENT ===========\n\n";

        int roll;

        cout << "Enter Roll Number : ";
        cin >> roll;

        // Delete code here

        cout << "\n---------------------------------\n";
        cout << "1. Delete Another Student\n";
        cout << "2. Back To Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter Choice : ";
        cin >> choice;

    } while (choice == 1);
}
//==========================================================
// UPDATE STUDENT
//==========================================================

void updateStudent()
{
    menuHeader();

    cout << "\n=========== UPDATE STUDENT ===========\n\n";

    int roll;

    cout << "Enter Roll Number : ";
    cin >> roll;

    for (auto &s : students)
    {
        if (s.roll == roll)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nEnter New Name : ";
            getline(cin, s.name);

            cout << "Enter New Age : ";
            cin >> s.age;

            while (s.age <= 0)
            {
                cout << "Invalid Age : ";
                cin >> s.age;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter New Course : ";
            getline(cin, s.course);

            cout << "Enter New Marks : ";
            cin >> s.marks;

            while (s.marks < 0 || s.marks > 100)
            {
                cout << "Invalid Marks : ";
                cin >> s.marks;
            }

            saveToFile();

            cout << "\nStudent Updated Successfully.";

            pauseScreen();
            return;
        }
    }

    cout << "\nStudent Not Found.";

    pauseScreen();
}

//==========================================================
// SEARCH BY ROLL
//==========================================================
void searchByRoll()
{
    int choice;

    do
    {
        system("cls");
        menuHeader();

        cout << "\n=========== SEARCH BY ROLL ===========\n\n";

        int roll;

        cout << "Enter Roll Number : ";
        cin >> roll;

        // Search code here

        cout << "\n---------------------------------\n";
        cout << "1. Search Again\n";
        cout << "2. Back To Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter Choice : ";
        cin >> choice;

    } while (choice == 1);
}
//==========================================================
// SEARCH BY NAME
//==========================================================

void searchByName()
{
    menuHeader();

    cout << "\n=========== SEARCH BY NAME ===========\n\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string name;

    cout << "Enter Name : ";
    getline(cin, name);

    bool found = false;

    for (auto &s : students)
    {
        if (s.name.find(name) != string::npos)
        {
            found = true;

            cout << "\n----------------------------------\n";

            cout << "Roll Number : " << s.roll << endl;
            cout << "Name        : " << s.name << endl;
            cout << "Age         : " << s.age << endl;
            cout << "Course      : " << s.course << endl;
            cout << "Marks       : " << s.marks << endl;
            cout << "Grade       : " << getGrade(s.marks) << endl;
        }
    }

    if (!found)
        cout << "\nStudent Not Found.";

    pauseScreen();
}

//==========================================================
// VIEW ALL STUDENTS
//==========================================================

void viewStudents()
{
    menuHeader();

    cout << "\n================== STUDENT RECORDS ==================\n\n";

    if (students.empty())
    {
        cout << "No Student Records Available.";

        pauseScreen();
        return;
    }

    cout << left
         << setw(8) << "Roll"
         << setw(25) << "Name"
         << setw(8) << "Age"
         << setw(20) << "Course"
         << setw(10) << "Marks"
         << setw(10) << "Grade"
         << endl;

    cout << "--------------------------------------------------------------------------\n";

    cout << fixed << setprecision(2);

    for (auto &s : students)
    {
        cout << left
             << setw(8) << s.roll
             << setw(25) << s.name
             << setw(8) << s.age
             << setw(20) << s.course
             << setw(10) << s.marks
             << setw(10) << getGrade(s.marks)
             << endl;
    }

    pauseScreen();
}
//==========================================================
// SORT BY NAME
//==========================================================

void sortByName()
{
    menuHeader();

    sort(students.begin(), students.end(),
         [](Student a, Student b)
         {
             return a.name < b.name;
         });

    saveToFile();

    cout << "\nStudents Sorted By Name Successfully.";

    pauseScreen();
}

//==========================================================
// SORT BY MARKS
//==========================================================

void sortByMarks()
{
    menuHeader();

    sort(students.begin(), students.end(),
         [](Student a, Student b)
         {
             return a.marks > b.marks;
         });

    saveToFile();

    cout << "\nStudents Sorted By Marks Successfully.";

    pauseScreen();
}

//==========================================================
// GENERATE REPORT
//==========================================================

void generateReport()
{
    menuHeader();

    if (students.empty())
    {
        cout << "\nNo Student Records Found.";
        pauseScreen();
        return;
    }

    float total = 0;
    float highest = students[0].marks;
    float lowest = students[0].marks;
    int pass = 0;

    for (auto &s : students)
    {
        total += s.marks;

        if (s.marks > highest)
            highest = s.marks;

        if (s.marks < lowest)
            lowest = s.marks;

        if (s.marks >= 50)
            pass++;
    }

    cout << "\n================ PERFORMANCE REPORT ================\n\n";

    cout << "Total Students   : " << students.size() << endl;
    cout << "Average Marks    : " << fixed << setprecision(2)
         << total / students.size() << endl;
    cout << "Highest Marks    : " << highest << endl;
    cout << "Lowest Marks     : " << lowest << endl;
    cout << "Passed Students  : " << pass << endl;
    cout << "Failed Students  : " << students.size() - pass << endl;

    pauseScreen();
}

//==========================================================
// MAIN FUNCTION
//==========================================================

int main()
{
    login();          // Login only once
    loadFromFile();   // Load saved data

while (true)
{
    menuHeader();

    int choice;

    cout << "\nEnter Choice : ";
    cin >> choice;

    switch (choice)
    {
        case 1:
            addStudent();
            break;

        case 2:
            deleteStudent();
            break;

        case 3:
            updateStudent();
            break;

        case 4:
            searchByRoll();
            break;

        case 5:
            searchByName();
            break;

        case 6:
            viewStudents();
            break;

        case 7:
            sortByName();
            break;

        case 8:
            sortByMarks();
            break;

        case 9:
            generateReport();
            break;

        case 10:
            saveToFile();
            break;

        case 11:
            return 0;

        default:
            cout << "\nInvalid Choice!";
            _getch();
    }
}

    return 0;
}
