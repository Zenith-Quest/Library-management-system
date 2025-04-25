//Library Management System

#include<iostream>
#include<ctime>
#include<cmath>
#include<sstream>
#include<fstream>
#include<string>

using namespace std;

string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << 1 + ltm->tm_mon << "-"
       << ltm->tm_mday << " "
       << 1 + ltm->tm_hour << ":"
       << 1 + ltm->tm_min << ":"
       << 1 + ltm->tm_sec;
    return ss.str();
}

class Student {
    private:
        string password;
    public:
        string username;
        string name;
        string email;

        string getpassword() { return password; }
        void setpassword(string pass) { password = pass; }

        void logfile(){
            ofstream loff("log.txt", ios::app);
            loff << "Student " << username << " logged in at " << getCurrentDateTime() << endl;
        }

        void issuebook(){
            string title;
            cout << "Enter book title to issue: ";
            cin >> title;

            ifstream infile("books.txt");
            ofstream outfile("temp.txt");
            string line;
            bool found = false;

            while (getline(infile, line)) {
                stringstream ss(line);
                string t, a, s;
                int n;
                getline(ss, t, ',');
                getline(ss, a, ',');
                getline(ss, s, ',');
                ss >> n;

                if (t == title && s == "available" && n > 0 && !found) {
                    found = true;
                    n--;
                    if (n == 0) s = "not available";
                    outfile << t << "," << a << "," << s << "," << n << endl;
                    ofstream issued("issuedbooks.txt", ios::app);
                    issued << t << "," << username << "," << getCurrentDateTime() << endl;
                    issued.close();
                } else {
                    outfile << t << "," << a << "," << s << "," << n << endl;
                }
            }
            infile.close();
            outfile.close();
            remove("books.txt");
            rename("temp.txt", "books.txt");

            if (found) cout << "Book issued successfully!" << endl;
            else cout << "Book not available or doesn't exist!" << endl;
        }

        void returnbook(){
            string title;
            cout << "Enter book title to return: ";
            cin >> title;

            ifstream infile("issuedbooks.txt");
            ofstream temp("tempissued.txt");
            string line;
            bool found = false;

            while (getline(infile, line)) {
                if (line.find(title) != string::npos && line.find(username) != string::npos && !found) {
                    found = true;
                } else {
                    temp << line << endl;
                }
            }
            infile.close();
            temp.close();
            remove("issuedbooks.txt");
            rename("tempissued.txt", "issuedbooks.txt");

            ifstream books("books.txt");
            ofstream tempbooks("tempbooks.txt");

            while (getline(books, line)) {
                stringstream ss(line);
                string t, a, s;
                int n;
                getline(ss, t, ',');
                getline(ss, a, ',');
                getline(ss, s, ',');
                ss >> n;

                if (t == title) {
                    n++;
                    s = "available";
                }
                tempbooks << t << "," << a << "," << s << "," << n << endl;
            }
            books.close();
            tempbooks.close();
            remove("books.txt");
            rename("tempbooks.txt", "books.txt");

            if (found) cout << "Book returned successfully!" << endl;
            else cout << "No such issued book found!" << endl;
        }
};

class Staff {
    private:
        string password;
    public:
        string username;
        string name;
        string email;

        string getpassword() { return password; }
        void setpassword(string pass) { password = pass; }

        void logfile(){
            ofstream loff("log.txt", ios::app);
            loff << "Staff " << username << " logged in at " << getCurrentDateTime() << endl;
        }
};

class Admin {
    private:
        string password;
    public:
        string getpassword(){ return password; }
};

class Book {
    public:
        string title;
        string author;
        string status;
        string issued_to;
        string issued_date;
        int num;

        void setstatus(string stat){ status = stat; }
};

void viewbooks(){
    ifstream file("books.txt");
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
}

void addbook(){
    Book b;
    cout << "Enter book title: "; cin >> b.title;
    cout << "Enter author: "; cin >> b.author;
    cout << "Enter status (available/not available): "; cin >> b.status;
    cout << "Enter number of copies: "; cin >> b.num;
    ofstream file("books.txt", ios::app);
    file << b.title << "," << b.author << "," << b.status << "," << b.num << endl;
    cout << "Book added successfully!" << endl;
}

void removebook(){
    string title;
    cout << "Enter book title to remove: ";
    cin >> title;

    ifstream file("books.txt");
    ofstream temp("temp.txt");
    string line;
    bool found = false;
    while (getline(file, line)) {
        if (line.find(title) == string::npos) {
            temp << line << endl;
        } else {
            found = true;
        }
    }
    file.close();
    temp.close();
    remove("books.txt");
    rename("temp.txt", "books.txt");
    if (found) cout << "Book removed successfully!" << endl;
    else cout << "Book not found!" << endl;
}

void viewlogfile(){
    ifstream file("log.txt");
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
}

void studentMenu(Student &s) {
    int choice;
    cout << "1. View Books\n2. Issue Book\n3. Return Book\nEnter your choice: ";
    cin >> choice;
    switch(choice){
        case 1: viewbooks(); break;
        case 2: s.issuebook(); break;
        case 3: s.returnbook(); break;
        default: cout << "Invalid choice!" << endl;
    }
}

void staffMenu() {
    int choice;
    cout << "1. Add Book\n2. Remove Book\n3. View Issued Books\nEnter your choice: ";
    cin >> choice;
    switch(choice){
        case 1: addbook(); break;
        case 2: removebook(); break;
        case 3: {
            ifstream file("issuedbooks.txt");
            string line;
            while (getline(file, line)) cout << line << endl;
            break;
        }
        default: cout << "Invalid choice!" << endl;
    }
}

void adminMenu() {
    int choice;
    cout << "1. View All Users\n2. View Log File\nEnter your choice: ";
    cin >> choice;
    switch(choice){
        case 1: {
            string line;
            cout << "--- Students ---\n";
            ifstream sfile("students.txt"); while (getline(sfile, line)) cout << line << endl;
            cout << "--- Staff ---\n";
            ifstream stfile("staff.txt"); while (getline(stfile, line)) cout << line << endl;
            break;
        }
        case 2: viewlogfile(); break;
        default: cout << "Invalid choice!" << endl;
    }
}

void sregister();
void slogin();
void stlogin();

void schoice(){
    int choice;
    cout << "1. Login\n2. Register\nEnter your choice: ";
    cin >> choice;
    switch(choice){
        case 1: slogin(); break;
        case 2: sregister(); break;
        default: cout << "Invalid choice!" << endl;
    }
}

void slogin(){
    Student s;
    string pass;
    cout << "Enter username: "; cin >> s.username;
    cout << "Enter password: "; cin >> pass;
    s.setpassword(pass);
    ifstream file("students.txt");
    string line;
    bool found = false;
    while (getline(file, line)) {
        if (line.find(s.username) != string::npos && line.find(s.getpassword()) != string::npos) {
            found = true; break;
        }
    }
    if (found) {
        cout << "Login successful!\n";
        s.logfile();
        studentMenu(s);
    } else cout << "Invalid username or password!\n";
}

void sregister(){
    Student s;
    string pass;
    cout << "Enter username: "; cin >> s.username;
    cout << "Enter password: "; cin >> pass;
    s.setpassword(pass);
    cout << "Enter name: "; cin >> s.name;
    cout << "Enter email: "; cin >> s.email;
    ofstream file("students.txt", ios::app);
    file << s.username << "," << s.getpassword() << "," << s.name << "," << s.email << endl;
    cout << "Registration successful! You can now login.\n";
    slogin();
}

void stlogin(){
    Staff s;
    string pass;
    cout << "Enter username: "; cin >> s.username;
    cout << "Enter password: "; cin >> pass;
    s.setpassword(pass);
    ifstream file("staff.txt");
    string line;
    bool found = false;
    while (getline(file, line)) {
        if (line.find(s.username) != string::npos && line.find(s.getpassword()) != string::npos) {
            found = true; break;
        }
    }
    if (found) {
        cout << "Login successful!\n";
        s.logfile();
        staffMenu();
    } else cout << "Invalid username or password!\n";
}

int main(){
    int choice;
    cout << "1. Student\n2. Staff\nEnter your choice: ";
    cin >> choice;
    switch(choice){
        case 1: schoice(); break;
        case 2: stlogin(); break;
        case 6969: adminMenu(); break;
        default: cout << "Invalid choice!" << endl;
    }
    return 0;
}