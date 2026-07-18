#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <windows.h>

using namespace std;

//================ COLOR =================

void color(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//================ STUDENT =================

struct Student
{
    int roll;
    string name;
    string fatherName;
    string course;
    string address;
    string phone;
    int marks;
    char grade;
};

vector<Student> students;

//================ BORDER =================

void line()
{
    color(11);
    cout << "+======================================================================+\n";
}

void title()
{
    system("cls");

    color(13);

    line();
    cout << "|                         CAMPUSCORE                                  |\n";
    cout << "|                  Student Management System                          |\n";
    line();

    color(14);
}

//================ LOGIN =================

bool login()
{
    string user, pass;

    title();

    color(10);

    cout << "\n";
    cout << "================ LOGIN PANEL ================\n\n";

    cout << "Username : ";
    cin >> user;

    cout << "Password : ";
    cin >> pass;

    if(user == "admin" && pass == "1234"){
        color(10);
        cout << "\nLogin Successful...\n";
        Sleep(1200);
        return true;
    }

    color(12);
    cout << "\nWrong Username or Password!\n";
    Sleep(1500);

    return false;
}

//================ MENU =================

void menu()
{
    title();

    color(11);

    line();

    cout << "|                          MAIN MENU                                 |\n";

    line();

    color(14);

    cout << "|  1. Add Student                                   |\n";
    cout << "|  2. Delete Student                                |\n";
    cout << "|  3. Update Student                                |\n";
    cout << "|  4. Search By Roll                                |\n";
    cout << "|  5. Search By Name                                |\n";
    cout << "|  6. Search By Course                              |\n";
    cout << "|  7. Search By Grade                               |\n";
    cout << "|  8. Search By Address                             |\n";
    cout << "|  9. Search By Phone Number                        |\n";
    cout << "| 10. View All Students                             |\n";
    cout << "| 11. View All Student Details                      |\n";
    cout << "| 12. View All Student Addresses                    |\n";
    cout << "| 13. View All Phone Numbers                        |\n";
    cout << "| 14. View All Marks (Out of 100)                   |\n";
    cout << "| 15. View Course-wise Students                     |\n";
    //cout << "| 16. Sort By Name                                  |\n";
   // cout << "| 17. Sort By Marks                                 |\n";
    cout << "| 16. Topper Student                                |\n";
    cout << "| 17. Average Marks                                 |\n";
    cout << "| 18. Highest & Lowest Marks                        |\n";
    //cout << "| 21. Pass Percentage                               |\n";
    cout << "| 19. Student Count By Course                       |\n";
    cout << "| 20. Generate Report                               |\n";
    //cout << "| 24. Export Report                                 |\n";
  //  cout << "| 25. Backup Data                                   |\n";
   // cout << "| 26. Restore Data                                  |\n";
    cout << "| 21. Save Data                                     |\n";
    cout << "| 22. Exit                                          |\n";

    line();

    color(10);
}
//================ GRADE =================

char calculateGrade(int marks)
{
    if(marks >= 90) return 'A';
    else if(marks >= 75) return 'B';
    else if(marks >= 60) return 'C';
    else if(marks >= 40) return 'D';
    else return 'F';
}

//================ SELECT COURSE =================

string selectCourse()
{
    int c;

    color(11);

    cout << "\n---------------------------------\n";
    cout << " Select Course :\n";
    cout << "---------------------------------\n";
    cout << " 1. B.Tech CSE\n";
    cout << " 2. MBA\n";
    cout << " 3. AIML\n";
    cout << " 4. Cyber Security\n";
    cout << " 5. MCA\n";
    cout << "---------------------------------\n";

    color(14);

    while(true)
    {
        cout << "Enter Choice (1-5) : ";
        cin >> c;

        switch(c)
        {
            case 1: return "B.Tech CSE";
            case 2: return "MBA";
            case 3: return "AIML";
            case 4: return "Cyber Security";
            case 5: return "MCA";
            default:
                color(12);
                cout << "Invalid Choice! Try Again.\n";
                color(14);
        }
    }
}

//================ ADD STUDENT =================

void addStudent()
{
    Student s;

    title();

    color(10);
    line();
    cout<<"              ADD NEW STUDENT\n";
    line();

    cout<<"Enter Roll Number : ";
    cin>>s.roll;

    // Duplicate Roll Check
    for(auto st : students)
    {
        if(st.roll == s.roll)
        {
            color(12);
            cout<<"\nRoll Number Already Exists!\n";
            system("pause");
            return;
        }
    }

    cin.ignore();

    cout<<"Enter Name : ";
    getline(cin,s.name);

    cout<<"Enter Father Name : ";
    getline(cin,s.fatherName);

    s.course = selectCourse();
    cin.ignore();

    cout<<"Enter Address : ";
    getline(cin,s.address);

    cout<<"Enter Phone Number : ";
    getline(cin,s.phone);

    cout<<"Enter Marks (0-100) : ";
    cin>>s.marks;

    s.grade = calculateGrade(s.marks);

    students.push_back(s);

    color(10);
    cout<<"\nStudent Added Successfully...\n";

    system("pause");
}

//================ DELETE STUDENT =================

void deleteStudent()
{
    title();

    int roll;
    bool found=false;

    color(12);

    line();
    cout<<"              DELETE STUDENT\n";
    line();

    cout<<"Enter Roll Number : ";
    cin>>roll;

    for(int i=0;i<students.size();i++)
    {
        if(students[i].roll==roll)
        {
            students.erase(students.begin()+i);

            found=true;

            color(10);
            cout<<"\nStudent Deleted Successfully.\n";
            break;
        }
    }

    if(!found)
    {
        color(12);
        cout<<"\nStudent Not Found!\n";
    }

    system("pause");
}

//================ UPDATE STUDENT =================

void updateStudent()
{
    title();

    int roll;
    bool found=false;

    color(14);

    line();
    cout<<"              UPDATE STUDENT\n";
    line();

    cout<<"Enter Roll Number : ";
    cin>>roll;

    cin.ignore();

    for(auto &s : students)
    {
        if(s.roll==roll)
        {
            cout<<"Enter New Name : ";
            getline(cin,s.name);

            cout<<"Enter Father Name : ";
            getline(cin,s.fatherName);

            s.course = selectCourse();
            cin.ignore();

            cout<<"Enter Address : ";
            getline(cin,s.address);

            cout<<"Enter Phone Number : ";
            getline(cin,s.phone);

            cout<<"Enter Marks : ";
            cin>>s.marks;

            s.grade=calculateGrade(s.marks);

            found=true;

            color(10);
            cout<<"\nStudent Updated Successfully.\n";

            break;
        }
    }

    if(!found)
    {
        color(12);
        cout<<"\nStudent Not Found!\n";
    }

    system("pause");
}

//================ SEARCH BY ROLL =================

void searchByRoll()
{
    title();

    int roll;
    bool found=false;

    color(11);

    line();
    cout<<"            SEARCH BY ROLL\n";
    line();

    cout<<"Enter Roll Number : ";
    cin>>roll;

    for(auto s : students)
    {
        if(s.roll==roll)
        {
            color(10);

            line();

            cout<<"Roll Number : "<<s.roll<<endl;
            cout<<"Name        : "<<s.name<<endl;
            cout<<"Father Name : "<<s.fatherName<<endl;
            cout<<"Course      : "<<s.course<<endl;
            cout<<"Address     : "<<s.address<<endl;
            cout<<"Phone       : "<<s.phone<<endl;
            cout<<"Marks       : "<<s.marks<<endl;
            cout<<"Grade       : "<<s.grade<<endl;

            line();

            found=true;
            break;
        }
    }

    if(!found)
    {
        color(12);
        cout<<"\nStudent Not Found!\n";
    }

    system("pause");
}

//================ SEARCH BY NAME =================

void searchByName()
{
    title();

    cin.ignore();

    string name;
    bool found=false;

    color(11);

    line();
    cout<<"            SEARCH BY NAME\n";
    line();

    cout<<"Enter Student Name : ";
    getline(cin,name);

    for(auto s : students)
    {
        if(s.name==name)
        {
            color(10);

            line();

            cout<<"Roll Number : "<<s.roll<<endl;
            cout<<"Name        : "<<s.name<<endl;
            cout<<"Course      : "<<s.course<<endl;
            cout<<"Marks       : "<<s.marks<<endl;
            cout<<"Grade       : "<<s.grade<<endl;

            line();

            found=true;
        }
    }

    if(!found)
    {
        color(12);
        cout<<"\nStudent Not Found!\n";
    }

    system("pause");
}
//================ SEARCH BY COURSE =================

void searchByCourse()
{
    title();

    string course;
    bool found = false;

    color(11);
    line();
    cout << "               SEARCH BY COURSE\n";
    line();

    course = selectCourse();

    for(auto s : students)
    {
        if(s.course == course)
        {
            color(10);

            cout << "\nRoll  : " << s.roll;
            cout << "\nName  : " << s.name;
            cout << "\nMarks : " << s.marks;
            cout << "\nGrade : " << s.grade;
            cout << "\n-------------------------------------\n";

            found = true;
        }
    }

    if(!found)
    {
        color(12);
        cout << "\nCourse Not Found!\n";
    }

    system("pause");
}

//================ SEARCH BY GRADE =================

void searchByGrade()
{
    title();

    char grade;
    bool found = false;

    line();
    color(11);
    cout << "               SEARCH BY GRADE\n";
    line();

    cout << "Enter Grade (A/B/C/D/F) : ";
    cin >> grade;

    for(auto s : students)
    {
        if(s.grade == grade)
        {
            color(10);

            cout << "\nRoll  : " << s.roll;
            cout << "\nName  : " << s.name;
            cout << "\nCourse: " << s.course;
            cout << "\nMarks : " << s.marks;
            cout << "\n-------------------------------------\n";

            found = true;
        }
    }

    if(!found)
    {
        color(12);
        cout << "\nNo Student Found!\n";
    }

    system("pause");
}

//================ SEARCH BY ADDRESS =================

void searchByAddress()
{
    title();

    string address;
    bool found = false;

    cin.ignore();

    line();
    color(11);
    cout << "             SEARCH BY ADDRESS\n";
    line();

    cout << "Enter Address : ";
    getline(cin, address);

    for(auto s : students)
    {
        if(s.address == address)
        {
            color(10);

            cout << "\nRoll : " << s.roll;
            cout << "\nName : " << s.name;
            cout << "\nPhone: " << s.phone;
            cout << "\n-------------------------------------\n";

            found = true;
        }
    }

    if(!found)
    {
        color(12);
        cout << "\nAddress Not Found!\n";
    }

    system("pause");
}

//================ SEARCH BY PHONE =================

void searchByPhone()
{
    title();

    string phone;
    bool found = false;

    cin.ignore();

    line();
    color(11);
    cout << "           SEARCH BY PHONE NUMBER\n";
    line();

    cout << "Enter Phone Number : ";
    getline(cin, phone);

    for(auto s : students)
    {
        if(s.phone == phone)
        {
            color(10);

            cout << "\nRoll : " << s.roll;
            cout << "\nName : " << s.name;
            cout << "\nCourse : " << s.course;
            cout << "\nMarks : " << s.marks;
            cout << "\n-------------------------------------\n";

            found = true;
        }
    }

    if(!found)
    {
        color(12);
        cout << "\nPhone Number Not Found!\n";
    }

    system("pause");
}

//================ VIEW ALL STUDENTS =================

void viewAllStudents()
{
    title();

    if(students.empty())
    {
        color(12);
        cout << "\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    line();

    color(14);

    cout << left
         << setw(8) << "Roll"
         << setw(25) << "Name"
         << setw(20) << "Course"
         << setw(10) << "Marks"
         << setw(8) << "Grade"
         << endl;

    line();

    color(10);

    for(auto s : students)
    {
        cout << left
             << setw(8) << s.roll
             << setw(25) << s.name
             << setw(20) << s.course
             << setw(10) << s.marks
             << setw(8) << s.grade
             << endl;
    }

    line();

    system("pause");
}

//================ VIEW ALL DETAILS =================

void viewAllDetails()
{
    title();

    if(students.empty())
    {
        color(12);
        cout << "\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    for(auto s : students)
    {
        line();

        color(10);

        cout << "Roll Number : " << s.roll << endl;
        cout << "Name        : " << s.name << endl;
        cout << "Father Name : " << s.fatherName << endl;
        cout << "Course      : " << s.course << endl;
        cout << "Address     : " << s.address << endl;
        cout << "Phone       : " << s.phone << endl;
        cout << "Marks       : " << s.marks << endl;
        cout << "Grade       : " << s.grade << endl;
    }

    line();

    system("pause");
}
//================ VIEW ALL ADDRESSES =================

void viewAllAddresses()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    line();
    color(14);

    cout<<left<<setw(8)<<"Roll"<<setw(25)<<"Name"<<setw(35)<<"Address"<<endl;

    line();

    color(10);

    for(auto s:students)
    {
        cout<<left<<setw(8)<<s.roll
            <<setw(25)<<s.name
            <<setw(35)<<s.address<<endl;
    }

    line();

    system("pause");
}

//================ VIEW ALL PHONE NUMBERS =================

void viewAllPhones()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    line();

    color(14);

    cout<<left<<setw(8)<<"Roll"
        <<setw(25)<<"Name"
        <<setw(20)<<"Phone"<<endl;

    line();

    color(10);

    for(auto s:students)
    {
        cout<<left<<setw(8)<<s.roll
            <<setw(25)<<s.name
            <<setw(20)<<s.phone<<endl;
    }

    line();

    system("pause");
}

//================ VIEW ALL MARKS =================

void viewAllMarks()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    line();

    color(14);

    cout<<left
        <<setw(8)<<"Roll"
        <<setw(25)<<"Name"
        <<setw(10)<<"Marks"
        <<setw(8)<<"Grade"<<endl;

    line();

    color(10);

    for(auto s:students)
    {
        cout<<left
            <<setw(8)<<s.roll
            <<setw(25)<<s.name
            <<setw(10)<<s.marks
            <<setw(8)<<s.grade<<endl;
    }

    line();

    system("pause");
}

//================ COURSE WISE STUDENTS =================

void viewCourseWiseStudents()
{
    title();

    string course;

    line();

    color(11);

    course = selectCourse();

    bool found=false;

    line();

    for(auto s:students)
    {
        if(s.course==course)
        {
            found=true;

            color(10);

            cout<<"Roll  : "<<s.roll<<endl;
            cout<<"Name  : "<<s.name<<endl;
            cout<<"Marks : "<<s.marks<<endl;

            line();
        }
    }

    if(!found)
    {
        color(12);
        cout<<"\nNo Student Found.\n";
    }

    system("pause");
}

//================ SORT BY NAME =================

void sortByName()
{
    sort(students.begin(),students.end(),

    [](Student a,Student b)
    {
        return a.name<b.name;
    });

    color(10);

    cout<<"\nStudents Sorted By Name Successfully.\n";

    system("pause");
}

//================ SORT BY MARKS =================

void sortByMarks()
{
    sort(students.begin(),students.end(),

    [](Student a,Student b)
    {
        return a.marks>b.marks;
    });

    color(10);

    cout<<"\nStudents Sorted By Marks Successfully.\n";

    system("pause");
}
//================ TOPPER STUDENT =================

void topperStudent()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    Student topper = students[0];

    for(auto s : students)
    {
        if(s.marks > topper.marks)
            topper = s;
    }

    line();
    color(10);

    cout<<"TOPPER STUDENT\n";

    line();

    cout<<"Roll Number : "<<topper.roll<<endl;
    cout<<"Name        : "<<topper.name<<endl;
    cout<<"Course      : "<<topper.course<<endl;
    cout<<"Marks       : "<<topper.marks<<endl;
    cout<<"Grade       : "<<topper.grade<<endl;

    line();

    system("pause");
}

//================ AVERAGE MARKS =================

void averageMarks()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    int sum = 0;

    for(auto s : students)
        sum += s.marks;

    double avg = (double)sum / students.size();

    line();

    color(10);

    cout<<"Average Marks : "<<avg<<endl;

    line();

    system("pause");
}

//================ HIGHEST & LOWEST =================

void highestLowestMarks()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    int highest = students[0].marks;
    int lowest = students[0].marks;

    for(auto s : students)
    {
        if(s.marks > highest)
            highest = s.marks;

        if(s.marks < lowest)
            lowest = s.marks;
    }

    line();

    color(10);

    cout<<"Highest Marks : "<<highest<<endl;
    cout<<"Lowest Marks  : "<<lowest<<endl;

    line();

    system("pause");
}

//================ PASS PERCENTAGE =================

void passPercentage()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    int pass = 0;

    for(auto s : students)
    {
        if(s.marks >= 40)
            pass++;
    }

    double per = (double)pass * 100 / students.size();

    line();

    color(10);

    cout<<"Pass Percentage : "<<per<<" %"<<endl;

    line();

    system("pause");
}

//================ STUDENT COUNT BY COURSE =================

void studentCountByCourse()
{
    title();

    string course;

    line();

    color(11);

    course = selectCourse();

    int count = 0;

    for(auto s : students)
    {
        if(s.course == course)
            count++;
    }

    line();

    color(10);

    cout<<"Total Students in "<<course<<" : "<<count<<endl;

    line();

    system("pause");
}
//================ GENERATE REPORT =================

void generateReport()
{
    title();

    if(students.empty())
    {
        color(12);
        cout<<"\nNo Student Record Found!\n";
        system("pause");
        return;
    }

    ofstream file("Student_Report.txt");

    file<<"========== CAMPUSCORE REPORT ==========\n\n";

    for(auto s : students)
    {
        file<<"Roll      : "<<s.roll<<endl;
        file<<"Name      : "<<s.name<<endl;
        file<<"Father    : "<<s.fatherName<<endl;
        file<<"Course    : "<<s.course<<endl;
        file<<"Address   : "<<s.address<<endl;
        file<<"Phone     : "<<s.phone<<endl;
        file<<"Marks     : "<<s.marks<<endl;
        file<<"Grade     : "<<s.grade<<endl;
        file<<"----------------------------------------\n";
    }

    file.close();

    color(10);
    cout<<"\nReport Generated Successfully.\n";

    system("pause");
}

//================ EXPORT REPORT =================

void exportReport()
{
    generateReport();
}

//================ SAVE DATA =================

void saveData()
{
    ofstream file("students.txt");

    for(auto s : students)
    {
        file<<s.roll<<endl;
        file<<s.name<<endl;
        file<<s.fatherName<<endl;
        file<<s.course<<endl;
        file<<s.address<<endl;
        file<<s.phone<<endl;
        file<<s.marks<<endl;
        file<<s.grade<<endl;
    }

    file.close();
}

//================ LOAD DATA =================

void loadData()
{
    students.clear();

    ifstream file("students.txt");

    if(!file)
        return;

    Student s;

    while(file>>s.roll)
    {
        file.ignore();

        getline(file,s.name);
        getline(file,s.fatherName);
        getline(file,s.course);
        getline(file,s.address);
        getline(file,s.phone);

        file>>s.marks;
        file>>s.grade;
        file.ignore();

        students.push_back(s);
    }

    file.close();
}

//================ BACKUP =================

void backupData()
{
    saveData();

    ifstream src("students.txt", ios::binary);
    ofstream dst("backup.txt", ios::binary);

    dst << src.rdbuf();

    src.close();
    dst.close();

    color(10);
    cout<<"\nBackup Created Successfully.\n";

    system("pause");
}

//================ RESTORE =================

void restoreData()
{
    ifstream src("backup.txt", ios::binary);
    ofstream dst("students.txt", ios::binary);

    dst << src.rdbuf();

    src.close();
    dst.close();

    loadData();

    color(10);
    cout<<"\nBackup Restored Successfully.\n";

    system("pause");
}
//====================== MAIN FUNCTION ======================

int main()
{
    while(!login());

    loadData();

    int choice;

    do
    {
        menu();

        cout << "\nEnter Your Choice : ";
        cin >> choice;

        switch(choice)
        {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: updateStudent(); break;
            case 4: searchByRoll(); break;
            case 5: searchByName(); break;
            case 6: searchByCourse(); break;
            case 7: searchByGrade(); break;
            case 8: searchByAddress(); break;
            case 9: searchByPhone(); break;
            case 10: viewAllStudents(); break;
            case 11: viewAllDetails(); break;
            case 12: viewAllAddresses(); break;
            case 13: viewAllPhones(); break;
            case 14: viewAllMarks(); break;
            case 15: viewCourseWiseStudents(); break;
            //case 16: sortByName(); break;
            //case 17: sortByMarks(); break;
            case 16: topperStudent(); break;
            case 17: averageMarks(); break;
            case 18: highestLowestMarks(); break;
           // case 21: passPercentage(); break;
            case 19: studentCountByCourse(); break;
            case 20: generateReport(); break;
            //case 24: exportReport(); break;   
            //case 25: backupData(); break;
           // case 26: restoreData(); break;
            case 21:
                saveData();
                color(10);
                cout << "\nData Saved Successfully.\n";
                system("pause");
                break;

            case 22:
                saveData();
                title();
                color(10);

                line();
                cout << "|            THANK YOU FOR USING CAMPUSCORE            |\n";
                cout << "|          Student Management System Closed            |\n";
                line();

                Sleep(2000);
                return 0;


            default:
                color(12);
                cout << "\nInvalid Choice! Please Try Again.\n";
                system("pause");
        }

    } while(true);

    return 0;
}