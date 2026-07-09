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
void menuHeader();
void drawMenuFrame(int frame);

//                                                              Student Structure

struct Student
{
    int roll;
    string name;
    string email;
    string phone;
    string department;
    int semester;
    string course;
    float marks;
};

vector<Student> students;

// Console Color

void color(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Pause Screen

void pauseScreen()
{
    cout << "\n\nPress any key to continue...";
    _getch();
}

// Welcome Screen

void welcome()
{
    system("cls");

    color(11);

    cout << "==============================================================\n";
    cout << "              CAMPUSCORE MANAGEMENT SYSTEM\n";
    cout << "==============================================================\n";

    color(7);
}

// Login

bool login()
{
    string username;
    string password = "";
    char ch;
    while (true){
        welcome();

        cout << "\nUsername : ";
        cin >> username;

        cout << "Password : ";
        password.clear();

        while (true){
            ch = _getch();

            if (ch == 13)
                break;

            else if (ch == 8){
                if (!password.empty()){
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else{
                password.push_back(ch);
                cout << "*";
            }
        }

        if (username == "admin" && password == "1234"){
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

//                                                       Save File
void saveToFile()
{
    ofstream fout("students.txt");

    // Header
    fout << "Roll\tName\tEmail\tPhone\tDepartment\tSemester\tCourse\tMarks\n";

    for (auto &s : students)
    {
        fout << s.roll << '\t'
             << s.name << '\t'
             << s.email << '\t'
             << s.phone << '\t'
             << s.department << '\t'
             << s.semester << '\t'
             << s.course << '\t'
             << s.marks << '\n';
    }

    fout.close();
}

//                                                         Load File

void loadFromFile(){
    students.clear();

    ifstream fin("students.txt");

    if (!fin)
        return;

    Student s;

    while (fin >> s.roll){
        fin.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(fin, s.name);

        getline(fin, s.email);
        getline(fin, s.phone);
        getline(fin, s.department);

        fin >> s.semester;
        fin.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(fin, s.course);

        fin >> s.marks;
        fin.ignore(numeric_limits<streamsize>::max(), '\n');

        students.push_back(s);
    }

    fin.close();
}

//                                                     Duplicate Roll Check

bool rollExists(int roll){
    for (auto &s : students){
        if (s.roll == roll)
            return true;
    }

    return false;
}

//                                                           Grade

string getGrade(float marks){
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

//                                                     ADD STUDENT

void addStudent(){
    int choice;

    do {
        menuHeader();

        cout << "\n=========== ADD STUDENT ===========\n\n";

        Student s;

        cout << "Enter Roll Number : ";
        cin >> s.roll;

        if (rollExists(s.roll)) {
            cout << "\nRoll Number Already Exists!\n";
        }
        else{
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Name : ";
            getline(cin, s.name);

            cout << "Enter Email : ";
            getline(cin, s.email);

            cout << "Enter Phone Number : ";
            getline(cin, s.phone);

            cout << "Enter Department : ";
            getline(cin, s.department);

            cout << "Enter Semester : ";
            cin >> s.semester;
            while (s.semester <= 0 || s.semester > 10){
                cout << "Invalid Semester! Enter Semester : ";
                cin >> s.semester;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Course : ";
            getline(cin, s.course);

            cout << "Enter Marks : ";
            cin >> s.marks;
            while (s.marks < 0 || s.marks > 100){
                cout << "Invalid Marks! Enter Marks : ";
                cin >> s.marks;
            }

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

//                                                     DELETE STUDENT

void deleteStudent(){
    int choice;

    do{
        menuHeader();

        cout << "\n=========== DELETE STUDENT ===========\n\n";

        int roll;

        cout << "Enter Roll Number to Delete : ";
        cin >> roll;

        auto it = find_if(students.begin(), students.end(), [roll](const Student &s) {
            return s.roll == roll;
        });

        if (it != students.end()){
            cout << "\nStudent Record Found:\n";
            cout << "Name       : " << it->name << endl;
            cout << "Department : " << it->department << endl;
            cout << "Course     : " << it->course << endl;
            
            char confirm;
            cout << "\nAre you sure you want to delete this student record? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y'){
                students.erase(it);
                saveToFile();
                cout << "\nStudent Deleted Successfully.\n";
            }
            else{
                cout << "\nDeletion Cancelled.\n";
            }
        }
        else{
            cout << "\nStudent Record Not Found.\n";
        }

        cout << "\n---------------------------------\n";
        cout << "1. Delete Another Student\n";
        cout << "2. Back To Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter Choice : ";
        cin >> choice;

    } while (choice == 1);
}

//                                                     UPDATE STUDENT

void updateStudent(){
    int choice;

    do{
        menuHeader();

        cout << "\n=========== UPDATE STUDENT ===========\n\n";

        int roll;

        cout << "Enter Roll Number to Update : ";
        cin >> roll;

        bool found = false;

        for (auto &s : students){
            if (s.roll == roll){
                found = true;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\nEnter New Name : ";
                getline(cin, s.name);

                cout << "Enter New Email : ";
                getline(cin, s.email);

                cout << "Enter New Phone : ";
                getline(cin, s.phone);

                cout << "Enter New Department : ";
                getline(cin, s.department);

                cout << "Enter New Semester : ";
                cin >> s.semester;
                while (s.semester <= 0 || s.semester > 10){
                    cout << "Invalid Semester! Enter Semester : ";
                    cin >> s.semester;
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter New Course : ";
                getline(cin, s.course);

                cout << "Enter New Marks : ";
                cin >> s.marks;
                while (s.marks < 0 || s.marks > 100){
                    cout << "Invalid Marks! Enter Marks : ";
                    cin >> s.marks;
                }

                saveToFile();

                cout << "\nStudent Updated Successfully.\n";
                break;
            }
        }

        if (!found){
            cout << "\nStudent Record Not Found.\n";
        }

        cout << "\n---------------------------------\n";
        cout << "1. Update Another Student\n";
        cout << "2. Back To Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter Choice : ";
        cin >> choice;

    } while (choice == 1);
}

//                                                           SEARCH BY ROLL

void searchByRoll(){
    int choice;

    do{
        menuHeader();

        cout << "\n=========== SEARCH BY ROLL ===========\n\n";

        int roll;

        cout << "Enter Roll Number to Search : ";
        cin >> roll;

        bool found = false;

        for (auto &s : students) {
            if (s.roll == roll){
                found = true;

                cout << "\n----------------------------------\n";
                cout << "Roll Number : " << s.roll << endl;
                cout << "Name        : " << s.name << endl;
                cout << "Email       : " << s.email << endl;
                cout << "Phone       : " << s.phone << endl;
                cout << "Department  : " << s.department << endl;
                cout << "Semester    : " << s.semester << endl;
                cout << "Course      : " << s.course << endl;
                cout << "Marks       : " << s.marks << endl;
                cout << "Grade       : " << getGrade(s.marks) << endl;
                cout << "----------------------------------\n";
                break;
            }
        }

        if (!found)
            cout << "\nStudent Record Not Found.\n";

        cout << "\n---------------------------------\n";
        cout << "1. Search Another Student by Roll\n";
        cout << "2. Back To Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter Choice : ";
        cin >> choice;

    } while (choice == 1);
}

// SEARCH BY NAME

void searchByName(){
    int choice;

    do{
        menuHeader();

        cout << "\n=========== SEARCH BY NAME ===========\n\n";

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string name;

        cout << "Enter Name to Search : ";
        getline(cin, name);

        bool found = false;

        for (auto &s : students){
            if (s.name.find(name) != string::npos){
                found = true;

                cout << "\n----------------------------------\n";
                cout << "Roll Number : " << s.roll << endl;
                cout << "Name        : " << s.name << endl;
                cout << "Email       : " << s.email << endl;
                cout << "Phone       : " << s.phone << endl;
                cout << "Department  : " << s.department << endl;
                cout << "Semester    : " << s.semester << endl;
                cout << "Course      : " << s.course << endl;
                cout << "Marks       : " << s.marks << endl;
                cout << "Grade       : " << getGrade(s.marks) << endl;
            }
        }

        if (!found)
            cout << "\nNo Student Records Found matching \"" << name << "\".\n";

        cout << "\n---------------------------------\n";
        cout << "1. Search Another Student by Name\n";
        cout << "2. Back To Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter Choice : ";
        cin >> choice;

    } while (choice == 1);
}

//                                                     VIEW ALL STUDENTS

void viewStudents(){
    menuHeader();

    cout << "\n=================================== STUDENT RECORDS ===================================\n\n";

    if (students.empty()){
        cout << "No Student Records Available.";
        pauseScreen();
        return;
    }

    cout << left
         << setw(8)  << "Roll"
         << setw(20) << "Name"
         << setw(8)  << "Gender"
         << setw(15) << "Dept"
         << setw(6)  << "Sem"
         << setw(15) << "Course"
         << setw(8)  << "Marks"
         << setw(8)  << "Grade"
         << endl;

    cout << "---------------------------------------------------------------------------------------\n";

    cout << fixed << setprecision(2);

    for (auto &s : students){
        cout << left
             << setw(8)  << s.roll
             << setw(20) << s.name
             << setw(15) << s.department
             << setw(6)  << s.semester
             << setw(15) << s.course
             << setw(8)  << s.marks
             << setw(8)  << getGrade(s.marks)
             << endl;
    }

    pauseScreen();
}

//                                                     SORT BY NAME

void sortByName(){
    menuHeader();

    if (students.empty()){
        cout << "\nNo Student Records to Sort.";
        pauseScreen();
        return;
    }

    sort(students.begin(), students.end(), [](Student a, Student b){
             return a.name < b.name;
         });

    saveToFile();

    cout << "\nStudents Sorted By Name Successfully.";

    pauseScreen();
}

// SORT BY MARKS

void sortByMarks(){
    menuHeader();

    if (students.empty()){
        cout << "\nNo Student Records to Sort.";
        pauseScreen();
        return;
    }

    sort(students.begin(), students.end(), [](Student a, Student b){
             return a.marks > b.marks;
         });

    saveToFile();

    cout << "\nStudents Sorted By Marks Successfully.";

    pauseScreen();
}

// GENERATE REPORT

void generateReport(){
    menuHeader();

    if (students.empty()){
        cout << "\nNo Student Records Found.";
        pauseScreen();
        return;
    }

    float total = 0;
    float highest = students[0].marks;
    float lowest = students[0].marks;
    int pass = 0;

    for (auto &s : students){
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

char spinner[] = {'|', '/', '-', '\\'};

void drawMenuFrame(int frame){
    system("cls");

    color(11);

    cout << "\n\n";

    cout << "                 ╔══════════════════════════════════════════════╗\n";
    cout << "                 ║                                              ║\n";

    color(14);
    cout << "                 ║         CAMPUSCORE MANAGEMENT SYSTEM         ║\n";

    color(11);
    cout << "                 ║                                              ║\n";

    color(10);
    cout << "                 ║             Logged In As : ADMIN             ║\n";

    color(11);
    cout << "                 ╠══════════════════════════════════════════════╣\n";

    color(7);
    cout << "                 ║  1. Add Student                             ║\n";
    cout << "                 ║  2. Delete Student                          ║\n";
    cout << "                 ║  3. Update Student                          ║\n";
    cout << "                 ║  4. Search By Roll                          ║\n";
    cout << "                 ║  5. Search By Name                          ║\n";
    cout << "                 ║  6. View All Students                       ║\n";
    cout << "                 ║  7. Sort By Name                            ║\n";
    cout << "                 ║  8. Sort By Marks                           ║\n";
    cout << "                 ║  9. Generate Report                         ║\n";
    cout << "                 ║ 10. Save Data                               ║\n";
    cout << "                 ║ 11. Exit                                    ║\n";

    color(11);
    cout << "                 ╚══════════════════════════════════════════════╝\n";

    color(7);
}

void menuHeader(){
    static int frame = 0;
    drawMenuFrame(frame++);
}

// MAIN FUNCTION

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    login();          // Login only once
    loadFromFile();   // Load saved data

    while (true){
        menuHeader();
        int choice;
        cout << "\nEnter Choice : ";
        cin >> choice;
    
    switch (choice){
         case 1:  addStudent();    break;
         case 2:  deleteStudent(); break;
         case 3:  updateStudent(); break;
         case 4:  searchByRoll();  break;
         case 5:  searchByName();  break;
         case 6:  viewStudents();  break;
         case 7:  sortByName();    break;
         case 8:  sortByMarks();   break;
         case 9:  generateReport(); break;
         case 10: saveToFile();
                cout << "\nData Saved Successfully.";
                pauseScreen();
                break;

         case 11: return 0;
                  default:
                cout << "\nInvalid Choice!";
                _getch();
        }
    }

    return 0;
}
