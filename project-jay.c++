#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <ctime>
#include <cctype>
#include <conio.h>   // for hiding password ( _getch() )
#include <windows.h> // for console colors + Sleep()

using namespace std;

// ==========================================
// Color Codes (Windows Console)
// ==========================================
#define COLOR_RED     12
#define COLOR_GREEN   10
#define COLOR_YELLOW  14
#define COLOR_CYAN    11
#define COLOR_WHITE   15
#define COLOR_MAGENTA 13
#define COLOR_BLUE    9
#define STYLE_HEADER  0x1F // bright white text on blue background

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int color)
{
    SetConsoleTextAttribute(hConsole, color);
}

void resetColor()
{
    SetConsoleTextAttribute(hConsole, COLOR_WHITE);
}

void clearScreen()
{
    system("cls");
}

string toLowerStr(const string &s)
{
    string out = s;
    transform(out.begin(), out.end(), out.begin(), [](unsigned char c)
              { return tolower(c); });
    return out;
}

// Truncates a string to maxLen chars (adds ".." if cut) so table columns
// never overflow and shift the box borders out of alignment.
string truncateStr(const string &s, int maxLen)
{
    if ((int)s.size() <= maxLen)
        return s;
    if (maxLen <= 2)
        return s.substr(0, maxLen);
    return s.substr(0, maxLen - 2) + "..";
}

// ==========================================
// Box Drawing Characters (Plain ASCII)
// Using plain ASCII (+, -, |, =) instead of CP437 or Unicode box-drawing
// glyphs. Those depend on the console codepage/font supporting extra
// characters, which can silently fail and make borders look broken or
// missing depending on the terminal. ASCII always renders correctly,
// everywhere, with zero encoding dependency.
// ==========================================
const string TL = "+", TR = "+", BL = "+", BR = "+", HZ = "=", VT = "|"; // double line (banners/menus)
const string ML = "+", MR = "+";                                        // double line mid junctions

const string tTL = "+", tTR = "+", tBL = "+", tBR = "+", tHZ = "-", tVT = "|"; // single line (tables/cards)
const string tML = "+", tMR = "+", tMT = "+", tMB = "+";                       // single line junctions

// Repeats a box character `count` times.
string rep(const string &ch, int count)
{
    string out;
    out.reserve(ch.size() * count);
    for (int i = 0; i < count; i++)
        out += ch;
    return out;
}

void loadingAnimation(const string &msg)
{
    setColor(COLOR_YELLOW);
    cout << "\n" << msg;
    for (int i = 0; i < 3; i++)
    {
        Sleep(150);
        cout << ".";
        cout.flush();
    }
    Sleep(150);
    cout << "\n";
    resetColor();
}

// Prints a centered double-line box banner
void printBanner(const string &title, int width = 45)
{
    setColor(COLOR_BLUE);
    cout << TL << rep(HZ, width) << TR << "\n";
    int pad = (width - (int)title.size()) / 2;
    cout << VT << string(pad, ' ') << title << string(width - pad - (int)title.size(), ' ') << VT << "\n";
    cout << BL << rep(HZ, width) << BR << "\n";
    resetColor();
}

// Prints a double-line box with a title row + separator + content lines
void printMenuBox(const string &title, const vector<string> &lines, int width = 45)
{
    setColor(COLOR_BLUE);
    cout << TL << rep(HZ, width) << TR << "\n";
    int pad = (width - (int)title.size()) / 2;
    cout << VT << string(pad, ' ') << title << string(width - pad - (int)title.size(), ' ') << VT << "\n";
    cout << ML << rep(HZ, width) << MR << "\n";
    resetColor();

    for (const auto &line : lines)
    {
        setColor(COLOR_BLUE);
        cout << VT;
        setColor(COLOR_WHITE);
        cout << " " << left << setw(width - 1) << line;
        setColor(COLOR_BLUE);
        cout << VT << "\n";
    }
    resetColor();

    setColor(COLOR_BLUE);
    cout << BL << rep(HZ, width) << BR << "\n";
    resetColor();
}

// Prints a bordered "card" for a single record (used for search results)
void printCard(const string &title, const vector<pair<string, string>> &fields, int width = 40)
{
    setColor(COLOR_BLUE);
    cout << "\n" << tTL << rep(tHZ, width) << tTR << "\n";
    int pad = (width - (int)title.size()) / 2;
    cout << tVT;
    setColor(COLOR_GREEN);
    cout << string(pad, ' ') << title << string(width - pad - (int)title.size(), ' ');
    setColor(COLOR_BLUE);
    cout << tVT << "\n";
    cout << tML << rep(tHZ, width) << tMR << "\n";
    resetColor();

    for (const auto &f : fields)
    {
        setColor(COLOR_BLUE);
        cout << tVT << " ";
        setColor(COLOR_YELLOW);
        cout << left << setw(12) << f.first;
        resetColor();
        cout << ": " << left << setw(width - 17) << f.second;
        setColor(COLOR_BLUE);
        cout << tVT << "\n";
        resetColor();
    }
    setColor(COLOR_BLUE);
    cout << tBL << rep(tHZ, width) << tBR << "\n";
    resetColor();
}

// Generic small bordered stat box (single label:value line, big colored value)
void printStatBox(const string &title, const string &value, int valueColor, int width = 41)
{
    setColor(COLOR_BLUE);
    cout << "\n" << TL << rep(HZ, width) << TR << "\n";
    int pad = (width - (int)title.size()) / 2;
    cout << VT << string(pad, ' ') << title << string(width - pad - (int)title.size(), ' ') << VT << "\n";
    cout << ML << rep(HZ, width) << MR << "\n";
    resetColor();

    setColor(valueColor);
    int vpad = (width - (int)value.size()) / 2;
    cout << VT << string(vpad, ' ') << value << string(width - vpad - (int)value.size(), ' ');
    setColor(COLOR_BLUE);
    cout << VT << "\n";

    cout << BL << rep(HZ, width) << BR << "\n";
    resetColor();
}

struct Student
{
    int roll;
    string name;
    int age;
    string course;
    float marks;
};

vector<Student> students;

// ==========================================
// 1. file handling functions
// ==========================================

void saveToFile()
{
    ofstream fout("students.txt");
    if (!fout)
    {
        setColor(COLOR_RED);
        cout << "\nError opening file for saving!\n";
        resetColor();
        return;
    }
    for (const auto &s : students)
    {
        fout << s.roll << "\n"
             << s.name << "\n"
             << s.age << "\n"
             << s.course << "\n"
             << s.marks << "\n";
    }
    fout.close();
    loadingAnimation("Saving data");
    setColor(COLOR_GREEN);
    cout << "Data Saved Successfully to students.txt.\n";
    resetColor();
}

void loadFromFile()
{
    ifstream fin("students.txt");
    if (!fin)
    {
        return;
    }
    Student s;
    while (fin >> s.roll)
    {
        fin.ignore();
        getline(fin, s.name);
        fin >> s.age;
        fin.ignore();
        getline(fin, s.course);
        fin >> s.marks;
        students.push_back(s);
    }
    fin.close();
}

// ==========================================
// 2. Login and Management Functions
// ==========================================

void welcome()
{
    clearScreen();
    cout << "\n";
    printBanner("CAMPUSCORE MANAGEMENT SYSTEM");
}

bool login()
{
    string user, pass = "";
    char ch;

    setColor(COLOR_YELLOW);
    cout << "\n  Username : ";
    resetColor();
    cin >> user;

    setColor(COLOR_YELLOW);
    cout << "  Password : ";
    resetColor();

    while (true)
    {
        ch = _getch();

        if (ch == 13)
        {
            break;
        }
        else if (ch == 8)
        {
            if (!pass.empty())
            {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            pass.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;

    loadingAnimation("Authenticating");

    if (user == "admin" && pass == "1234")
    {
        setColor(COLOR_GREEN);
        cout << "Login Successful\n";
        resetColor();
        Sleep(500);
        return true;
    }
    setColor(COLOR_RED);
    cout << "Invalid Login\n";
    resetColor();
    return false;
}

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
    else
        return "Fail";
}

// Helper to print grade with color, returns to normal after
void printGradeColored(const string &grade)
{
    if (grade == "Fail")
        setColor(COLOR_RED);
    else if (grade == "A+" || grade == "A")
        setColor(COLOR_GREEN);
    else
        setColor(COLOR_YELLOW);
    cout << grade;
    resetColor();
}

bool rollExists(int roll)
{
    for (const auto &s : students)
    {
        if (s.roll == roll)
            return true;
    }
    return false;
}

// ==========================================
// Course selection menu (fixed list of offered courses)
// ==========================================
const vector<string> COURSE_LIST = {
    "B.Tech CSE",
    "BCA",
    "B.Tech AI",
    "B.Tech Cyber Security",
    "MBA"
};

// Displays a numbered course menu and returns the chosen course name.
// Loops until a valid choice (1..COURSE_LIST.size()) is entered.
string selectCourse()
{
    vector<string> lines;
    for (size_t i = 0; i < COURSE_LIST.size(); i++)
    {
        lines.push_back(" " + to_string(i + 1) + ". " + COURSE_LIST[i]);
    }

    while (true)
    {
        printMenuBox("Select Course", lines);

        setColor(COLOR_YELLOW);
        cout << "\n  Enter Choice : ";
        resetColor();

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            setColor(COLOR_RED);
            cout << "\nInvalid Input. Please Enter A Number.\n";
            resetColor();
            continue;
        }

        if (choice >= 1 && choice <= (int)COURSE_LIST.size())
        {
            return COURSE_LIST[choice - 1];
        }

        setColor(COLOR_RED);
        cout << "\nInvalid Choice. Please Select A Valid Course.\n";
        resetColor();
    }
}

// ==========================================
// Reusable bordered table (used by View All / Search By Course / Search By Grade / Topper)
// ==========================================
void printStudentTable(const vector<Student> &list)
{
    int wRoll = 8, wName = 18, wAge = 6, wCourse = 16, wMarks = 8, wGrade = 7;

    setColor(COLOR_BLUE);
    cout << "\n" << tTL << rep(tHZ, wRoll) << tMT << rep(tHZ, wName) << tMT
         << rep(tHZ, wAge) << tMT << rep(tHZ, wCourse) << tMT
         << rep(tHZ, wMarks) << tMT << rep(tHZ, wGrade) << tTR << "\n";
    resetColor();

    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wRoll) << " Roll";
    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wName) << " Name";
    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wAge) << " Age";
    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wCourse) << " Course";
    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wMarks) << " Marks";
    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wGrade) << " Grade";
    resetColor();
    setColor(COLOR_BLUE);
    cout << tVT << "\n";

    resetColor();
    setColor(COLOR_BLUE);
    cout << tML << rep(tHZ, wRoll) << tMT << rep(tHZ, wName) << tMT
         << rep(tHZ, wAge) << tMT << rep(tHZ, wCourse) << tMT
         << rep(tHZ, wMarks) << tMT << rep(tHZ, wGrade) << tMR << "\n";
    resetColor();

    for (const auto &s : list)
    {
        setColor(COLOR_BLUE);
        cout << tVT;
        resetColor();
        cout << left << setw(wRoll) << (" " + to_string(s.roll));
        setColor(COLOR_BLUE);
        cout << tVT;
        resetColor();
        cout << left << setw(wName) << (" " + truncateStr(s.name, wName - 1));
        setColor(COLOR_BLUE);
        cout << tVT;
        resetColor();
        cout << left << setw(wAge) << (" " + to_string(s.age));
        setColor(COLOR_BLUE);
        cout << tVT;
        resetColor();
        cout << left << setw(wCourse) << (" " + truncateStr(s.course, wCourse - 1));
        setColor(COLOR_BLUE);
        cout << tVT;
        resetColor();
        cout << left << setw(wMarks) << (" " + to_string((int)s.marks));
        setColor(COLOR_BLUE);
        cout << tVT << " ";
        string grade = getGrade(s.marks);
        printGradeColored(grade);
        setColor(COLOR_BLUE);
        // wGrade(7) = 1 leading space + grade text + remaining padding
        cout << string(max(0, wGrade - 1 - (int)grade.size()), ' ') << tVT << "\n";
        resetColor();
    }

    setColor(COLOR_BLUE);
    cout << tBL << rep(tHZ, wRoll) << tMB << rep(tHZ, wName) << tMB
         << rep(tHZ, wAge) << tMB << rep(tHZ, wCourse) << tMB
         << rep(tHZ, wMarks) << tMB << rep(tHZ, wGrade) << tBR << "\n";
    resetColor();
}

void addStudent()
{
    Student s;
    setColor(COLOR_CYAN);
    cout << "\nEnter Roll Number : ";
    resetColor();
    cin >> s.roll;

    if (rollExists(s.roll))
    {
        setColor(COLOR_RED);
        cout << "\nRoll Number Already Exists.\n";
        resetColor();
        return;
    }

    cin.ignore();
    setColor(COLOR_CYAN);
    cout << "Enter Name : ";
    resetColor();
    getline(cin, s.name);

    setColor(COLOR_CYAN);
    cout << "Enter Age : ";
    resetColor();
    cin >> s.age;

    s.course = selectCourse();

    setColor(COLOR_CYAN);
    cout << "\nEnter Marks : ";
    resetColor();
    cin >> s.marks;

    students.push_back(s);
    saveToFile();
    setColor(COLOR_GREEN);
    cout << "Student Added Successfully.\n";
    resetColor();
}

void viewStudents()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }
    printStudentTable(students);
}

void searchByRoll()
{
    int roll;
    setColor(COLOR_CYAN);
    cout << "\nEnter Roll Number : ";
    resetColor();
    cin >> roll;

    for (const auto &s : students)
    {
        if (s.roll == roll)
        {
            printCard("STUDENT FOUND", {
                {"Roll", to_string(s.roll)},
                {"Name", s.name},
                {"Age", to_string(s.age)},
                {"Course", s.course},
                {"Marks", to_string((int)s.marks)},
                {"Grade", getGrade(s.marks)}
            });
            return;
        }
    }
    setColor(COLOR_RED);
    cout << "\nStudent Not Found.\n";
    resetColor();
}

void searchByName()
{
    cin.ignore();
    string name;
    setColor(COLOR_CYAN);
    cout << "\nEnter Name : ";
    resetColor();
    getline(cin, name);

    bool found = false;
    for (const auto &s : students)
    {
        if (s.name == name)
        {
            found = true;
            printCard("STUDENT FOUND", {
                {"Roll", to_string(s.roll)},
                {"Name", s.name},
                {"Age", to_string(s.age)},
                {"Course", s.course},
                {"Marks", to_string((int)s.marks)},
                {"Grade", getGrade(s.marks)}
            });
        }
    }
    if (!found)
    {
        setColor(COLOR_RED);
        cout << "\nStudent Not Found.\n";
        resetColor();
    }
}

void searchByCourse()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }

    string course = selectCourse();

    vector<Student> result;
    for (const auto &s : students)
    {
        if (toLowerStr(s.course) == toLowerStr(course))
            result.push_back(s);
    }

    if (result.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found In This Course.\n";
        resetColor();
        return;
    }

    setColor(COLOR_GREEN);
    cout << "\n" << (int)result.size() << " Student(s) Found In \"" << course << "\"\n";
    resetColor();
    printStudentTable(result);
}

void searchByGrade()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }
    string grade;
    setColor(COLOR_CYAN);
    cout << "\nEnter Grade (A+ / A / B / C / D / Fail) : ";
    resetColor();
    cin >> grade;

    vector<Student> result;
    for (const auto &s : students)
    {
        if (toLowerStr(getGrade(s.marks)) == toLowerStr(grade))
            result.push_back(s);
    }

    if (result.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found With This Grade.\n";
        resetColor();
        return;
    }

    setColor(COLOR_GREEN);
    cout << "\n" << (int)result.size() << " Student(s) Found With Grade \"" << grade << "\"\n";
    resetColor();
    printStudentTable(result);
}

void topperStudent()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }

    float maxMarks = students[0].marks;
    for (const auto &s : students)
        if (s.marks > maxMarks)
            maxMarks = s.marks;

    vector<Student> toppers;
    for (const auto &s : students)
        if (s.marks == maxMarks)
            toppers.push_back(s);

    if (toppers.size() == 1)
    {
        const auto &s = toppers[0];
        printCard("TOPPER STUDENT", {
            {"Roll", to_string(s.roll)},
            {"Name", s.name},
            {"Age", to_string(s.age)},
            {"Course", s.course},
            {"Marks", to_string((int)s.marks)},
            {"Grade", getGrade(s.marks)}
        });
    }
    else
    {
        setColor(COLOR_GREEN);
        cout << "\n" << (int)toppers.size() << " Students Tied For Topper (Marks: " << (int)maxMarks << ")\n";
        resetColor();
        printStudentTable(toppers);
    }
}

void passPercentage()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }

    int passCount = 0;
    for (const auto &s : students)
        if (getGrade(s.marks) != "Fail")
            passCount++;

    float pct = (float)passCount / students.size() * 100;
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f%%", pct);

    int color = (pct >= 50) ? COLOR_GREEN : COLOR_RED;
    printStatBox("PASS PERCENTAGE", string(buf) + "  (" + to_string(passCount) + "/" + to_string(students.size()) + " passed)", color);
}

void studentCountByCourse()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }

    map<string, int> counts;
    for (const auto &s : students)
        counts[s.course]++;

    int wCourse = 26, wCount = 10;

    setColor(COLOR_BLUE);
    cout << "\n" << tTL << rep(tHZ, wCourse) << tMT << rep(tHZ, wCount) << tTR << "\n";
    resetColor();

    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wCourse) << " Course";
    setColor(COLOR_BLUE);
    cout << tVT;
    setColor(COLOR_YELLOW);
    cout << left << setw(wCount) << " Count";
    resetColor();
    setColor(COLOR_BLUE);
    cout << tVT << "\n";
    cout << tML << rep(tHZ, wCourse) << tMT << rep(tHZ, wCount) << tMR << "\n";
    resetColor();

    for (const auto &c : counts)
    {
        setColor(COLOR_BLUE);
        cout << tVT;
        resetColor();
        cout << left << setw(wCourse) << (" " + truncateStr(c.first, wCourse - 1));
        setColor(COLOR_BLUE);
        cout << tVT;
        resetColor();
        cout << left << setw(wCount) << (" " + to_string(c.second));
        setColor(COLOR_BLUE);
        cout << tVT << "\n";
        resetColor();
    }

    setColor(COLOR_BLUE);
    cout << tBL << rep(tHZ, wCourse) << tMB << rep(tHZ, wCount) << tBR << "\n";
    resetColor();
}

void deleteStudent()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }

    int roll;
    setColor(COLOR_CYAN);
    cout << "\nEnter Roll Number To Delete : ";
    resetColor();
    cin >> roll;

    for (auto it = students.begin(); it != students.end(); it++)
    {
        if (it->roll == roll)
        {
            students.erase(it);
            saveToFile();
            setColor(COLOR_GREEN);
            cout << "Student Deleted Successfully.\n";
            resetColor();
            return;
        }
    }
    setColor(COLOR_RED);
    cout << "\nStudent Not Found.\n";
    resetColor();
}

void updateStudent()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo Student Found.\n";
        resetColor();
        return;
    }
    int roll;
    setColor(COLOR_CYAN);
    cout << "\nEnter Roll Number to Update: ";
    resetColor();
    cin >> roll;

    for (auto &s : students)
    {
        if (s.roll == roll)
        {
            setColor(COLOR_GREEN);
            cout << "\nStudent Found! Enter New Details:\n";
            resetColor();
            cin.ignore();
            cout << "Enter New Name: ";
            getline(cin, s.name);
            cout << "Enter New Age: ";
            cin >> s.age;
            s.course = selectCourse();
            cout << "\nEnter New Marks: ";
            cin >> s.marks;
            saveToFile();
            setColor(COLOR_GREEN);
            cout << "Student Updated Successfully.\n";
            resetColor();
            return;
        }
    }
    setColor(COLOR_RED);
    cout << "\nStudent Not Found.\n";
    resetColor();
}

// ==========================================
// 3. Sorting and Report Functions
// ==========================================

void sortByName()
{
    sort(students.begin(), students.end(), [](const Student &a, const Student &b)
         { return a.name < b.name; });
}

void sortByMarks()
{
    sort(students.begin(), students.end(), [](const Student &a, const Student &b)
         { return a.marks > b.marks; });
}

void generateReport()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo data available to generate report.\n";
        resetColor();
        return;
    }
    float totalMarks = 0;
    float highest = students[0].marks;
    float lowest = students[0].marks;
    int passCount = 0;

    for (const auto &s : students)
    {
        totalMarks += s.marks;
        if (s.marks > highest)
            highest = s.marks;
        if (s.marks < lowest)
            lowest = s.marks;
        if (getGrade(s.marks) != "Fail")
            passCount++;
    }

    char avgBuf[16];
    snprintf(avgBuf, sizeof(avgBuf), "%.2f", (totalMarks / students.size()));

    setColor(COLOR_BLUE);
    cout << "\n" << TL << rep(HZ, 41) << TR << "\n";
    string title = "PERFORMANCE REPORT";
    int pad = (41 - (int)title.size()) / 2;
    cout << VT << string(pad, ' ') << title << string(41 - pad - (int)title.size(), ' ') << VT << "\n";
    cout << ML << rep(HZ, 41) << MR << "\n";
    resetColor();

    auto printReportLine = [&](const string &label, const string &value, int color)
    {
        setColor(COLOR_WHITE);
        cout << VT << " " << left << setw(21) << label;
        setColor(color);
        cout << left << setw(19) << value;
        setColor(COLOR_WHITE);
        cout << VT << "\n";
        resetColor();
    };

    printReportLine("Total Students", to_string(students.size()), COLOR_WHITE);
    printReportLine("Average Marks", avgBuf, COLOR_WHITE);
    printReportLine("Highest Marks", to_string((int)highest), COLOR_GREEN);
    printReportLine("Lowest Marks", to_string((int)lowest), COLOR_RED);
    printReportLine("Passed Students", to_string(passCount), COLOR_GREEN);
    printReportLine("Failed Students", to_string(students.size() - passCount), COLOR_RED);

    setColor(COLOR_BLUE);
    cout << BL << rep(HZ, 41) << BR << "\n";
    resetColor();
}

void exportReport()
{
    if (students.empty())
    {
        setColor(COLOR_RED);
        cout << "\nNo data available to export.\n";
        resetColor();
        return;
    }

    ofstream fout("report_export.txt");
    if (!fout)
    {
        setColor(COLOR_RED);
        cout << "\nError opening file for export!\n";
        resetColor();
        return;
    }

    float totalMarks = 0, highest = students[0].marks, lowest = students[0].marks;
    int passCount = 0;
    map<string, int> courseCounts;

    for (const auto &s : students)
    {
        totalMarks += s.marks;
        if (s.marks > highest)
            highest = s.marks;
        if (s.marks < lowest)
            lowest = s.marks;
        if (getGrade(s.marks) != "Fail")
            passCount++;
        courseCounts[s.course]++;
    }

    time_t now = time(0);
    string dt = ctime(&now); // includes trailing newline

    fout << "====================================================\n";
    fout << "           CAMPUSCORE PERFORMANCE REPORT\n";
    fout << "====================================================\n";
    fout << "Generated On : " << dt;
    fout << "----------------------------------------------------\n";
    fout << "Total Students      : " << students.size() << "\n";
    fout << "Average Marks       : " << fixed << setprecision(2) << (totalMarks / students.size()) << "\n";
    fout << "Highest Marks       : " << highest << "\n";
    fout << "Lowest Marks        : " << lowest << "\n";
    fout << "Passed Students     : " << passCount << "\n";
    fout << "Failed Students     : " << students.size() - passCount << "\n";
    fout << "Pass Percentage     : " << fixed << setprecision(2) << ((float)passCount / students.size() * 100) << "%\n";
    fout << "----------------------------------------------------\n";
    fout << "Student Count By Course:\n";
    for (const auto &c : courseCounts)
        fout << "  - " << c.first << " : " << c.second << "\n";
    fout << "----------------------------------------------------\n";
    fout << "Full Student List:\n\n";
    fout << left << setw(8) << "Roll" << setw(20) << "Name" << setw(6) << "Age"
         << setw(18) << "Course" << setw(8) << "Marks" << setw(6) << "Grade" << "\n";
    fout << string(66, '-') << "\n";
    for (const auto &s : students)
    {
        fout << left << setw(8) << s.roll << setw(20) << s.name << setw(6) << s.age
             << setw(18) << s.course << setw(8) << s.marks << setw(6) << getGrade(s.marks) << "\n";
    }
    fout << "====================================================\n";
    fout.close();

    loadingAnimation("Exporting report");
    setColor(COLOR_GREEN);
    cout << "Report Exported Successfully to report_export.txt\n";
    resetColor();
}

// ==========================================
// 4. main function
// ==========================================

int main()
{
    welcome();

    if (!login())
    {
        cout << "\nPress any key to exit...";
        _getch();
        return 0;
    }

    loadFromFile();
    Sleep(400);

    int choice;

    while (true)
    {
        clearScreen();
        printMenuBox("CAMPUSCORE MAIN MENU", {
            " 1. Add Student",
            " 2. Delete Student",
            " 3. Update Student",
            " 4. Search By Roll",
            " 5. Search By Name",
            " 6. Search By Course",
            " 7. Search By Grade",
            " 8. View All Students",
            " 9. Sort By Name",
            "10. Sort By Marks",
            "11. Topper Student",
            "12. Pass Percentage",
            "13. Student Count by Course",
            "14. Generate Report",
            "15. Export Report",
            "16. Save Data",
            "17. Exit"
        });

        setColor(COLOR_YELLOW);
        cout << "\n  Enter Choice : ";
        resetColor();
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
            searchByCourse();
            break;
        case 7:
            searchByGrade();
            break;
        case 8:
            viewStudents();
            break;
        case 9:
            sortByName();
            setColor(COLOR_GREEN);
            cout << "\nSorted By Name Successfully.\n";
            resetColor();
            break;
        case 10:
            sortByMarks();
            setColor(COLOR_GREEN);
            cout << "\nSorted By Marks Successfully.\n";
            resetColor();
            break;
        case 11:
            topperStudent();
            break;
        case 12:
            passPercentage();
            break;
        case 13:
            studentCountByCourse();
            break;
        case 14:
            generateReport();
            break;
        case 15:
            exportReport();
            break;
        case 16:
            saveToFile();
            break;
        case 17:
        {
            char ch;
            setColor(COLOR_YELLOW);
            cout << "\nDo You Want To Save Before Exit? (Y/N) : ";
            resetColor();
            cin >> ch;
            if (ch == 'Y' || ch == 'y')
            {
                saveToFile();
            }
            clearScreen();
            printBanner("THANK YOU FOR USING CAMPUSCORE");
            return 0;
        }
        default:
            setColor(COLOR_RED);
            cout << "\nInvalid Choice.\n";
            resetColor();
        }

        setColor(COLOR_MAGENTA);
        cout << "\nPress any key to continue...";
        resetColor();
        _getch();
    }
    return 0;
}

