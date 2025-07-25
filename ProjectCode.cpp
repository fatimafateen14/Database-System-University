#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <windows.h>
#include <cctype>
#include <stdexcept>

using namespace std;

const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

//---------------------------------------------functions for exceptional handling--------------------------------
bool Alphabet(const string& str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

bool Numeric(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to validate that the input is exactly 6 digits and not null
bool isValidStudentID(const string& id) {
    // Check if the string is not empty and exactly 6 characters long
    if (id.empty() || id.length() != 6) return false;

    // Check if all characters are digits
    for (char c : id) {
        if (!isdigit(c)) return false; // If any character is not a digit, return false
    }
    return true; // Valid if it passes both checks
}

void allocateName(string& name) {
    int length = 0;
    string input;

    // Ask user for the length
    while (true) {
        cout << YELLOW << "\nHow many characters space you want? " << RESET;
        cin >> input;

        // Check if the input is a valid positive integer
        bool valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            length = stoi(input); // Convert to integer
            if (length > 0) {
                break; // Valid length
            }
        }

        cout << RED << "\nError: Please enter a positive number.\n" << RESET;
        cin.clear(); // Clear the input flag
        cin.ignore(10000, '\n'); // Discard invalid input
    }

    cin.ignore(); // Clear leftover characters in the input buffer

    while (true) {
        string temp; // Temporary string to handle input
        cout << "\nEnter (up to " << length << " characters): ";
        getline(cin, temp); // Read input into temp string

        if (Alphabet(temp) && temp.length() <= length) {
            name = temp; // Store valid input in the provided string reference
            break; // Exit the loop
        }
        else {
            if (!Alphabet(temp)) {
                cout << RED << "\n\nInvalid! Name must not contain numeric values." << RESET << endl;
            }
            else {
                cout << RED << "\nError: Name exceeds the allowed length. Try again.\n" << RESET;
            }
        }
    }

    // Display the entered name
    cout << "\t\t\t\tYou entered: " << name << endl;
}

// Example XOR encryption/decryption function
string xorEncryptDecrypt(const string& input, char key) {
    string output = input;
    for (size_t i = 0; i < input.size(); i++) {
        output[i] = input[i] ^ key;
    }
    return output;
}

// Function to change console text color (Windows only)
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Function to display a decorative heading
void displayHeading(const string& heading) {
    setColor(14); // Yellow
    cout << "\n=======================================================\n";
    cout << "               " << heading << "\n";
    cout << "=======================================================\n";
    setColor(7); // Reset to white
}

// Function to print a separator
void printSeparator() {
    setColor(11); // Cyan
    cout << "-------------------------------------------------------\n";
    setColor(7); // Reset to white
}

// Function to validate if input is non-empty
bool isNonEmpty(const string& str) {
    return !str.empty();
}

// Function to validate if input is a valid double
bool isValidDouble(const string& str) {
    try {
        stod(str);
        return true;
    }
    catch (...) {
        return false;
    }
}

// Function to validate if input contains only digits
bool isDigitsOnly(const string& str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

// Function to validate if input is a valid integer
bool isValidInteger(const string& str) {
    return isDigitsOnly(str) && !str.empty();
}

// Function to validate if a contact number is exactly 11 digits
bool isValidContactNumber(const string& number) {
    return isDigitsOnly(number) && number.length() == 11;
}

// Function to validate GPA
bool validateGPA(double gpa) {
    return gpa >= 0.0 && gpa <= 4.0; // GPA must be between 0.0 and 4.0
}

// -----------------------------------------------User authentication-----------------------------------------------
bool login(const string& username, const string& password, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file.\n";
        return false;
    }

    string storedUsername, storedPassword;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        ss >> storedUsername >> storedPassword;
        if (storedUsername == username && storedPassword == password) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// Struct for Rows representing keys
struct Row {
    string id; // Primary Key
    string name;
    string foreignKey; // Foreign Key reference

    Row() : id("NULL"), name("NULL"), foreignKey("NULL") {}
    Row(string id, string name, string foreignKey) : id(id), name(name), foreignKey(foreignKey) {}
};

// --------------------------------------------read data from a text file-------------------------------
vector<Row> readDataFromFile(const string& filename) {
    vector<Row> rows;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return rows;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, foreignKey;

        // Assuming the format is: id,name,foreignKey
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, foreignKey); // Read the foreign key

        // Check if all fields are non-empty
        if (isNonEmpty(id) && isNonEmpty(name)) {
            rows.emplace_back(Row{ id, name, foreignKey }); // Include foreign key
        }
    }

    file.close();
    return rows;
}
//--------------------------------------------------------------joins------------------------------------------
// Inner Join
vector<pair<Row, Row>> innerJoin(const vector<Row>& table1, const vector<Row>& table2) {
    vector<pair<Row, Row>> result;
    unordered_map<string, Row> table2Map;

    for (const auto& row : table2) {
        table2Map[row.id] = row; // Assuming row.id is the primary key in table2
    }

    for (const auto& row1 : table1) {
        if (table2Map.find(row1.foreignKey) != table2Map.end()) {
            result.emplace_back(row1, table2Map[row1.foreignKey]);
        }
    }

    return result;
}
// Left Join
vector<pair<Row, Row>> leftJoin(const vector<Row>& table1, const vector<Row>& table2) {
    vector<pair<Row, Row>> result;
    unordered_map<string, Row> table2Map;

    for (const auto& row : table2) {
        table2Map[row.id] = row;
    }

    for (const auto& row1 : table1) {
        if (table2Map.find(row1.foreignKey) != table2Map.end()) {
            result.emplace_back(row1, table2Map[row1.foreignKey]);
        }
        else {
            result.emplace_back(row1, Row{ "NULL", "NULL", "NULL" }); // NULL for no match
        }
    }

    return result;
}

// Right Join
vector<pair<Row, Row>> rightJoin(const vector<Row>& table1, const vector<Row>& table2) {
    vector<pair<Row, Row>> result;
    unordered_map<string, Row> table1Map;

    for (const auto& row : table1) {
        table1Map[row.id] = row;
    }

    for (const auto& row2 : table2) {
        if (table1Map.find(row2.foreignKey) != table1Map.end()) {
            result.emplace_back(table1Map[row2.foreignKey], row2);
        }
        else {
            result.emplace_back(Row{ "NULL", "NULL", "NULL" }, row2); // NULL for no match
        }
    }

    return result;
}

// Full Outer Join
vector<pair<Row, Row>> fullOuterJoin(const vector<Row>& table1, const vector<Row>& table2) {
    vector<pair<Row, Row>> result;
    unordered_map<string, Row> table1Map;
    unordered_map<string, Row> table2Map;

    for (const auto& row : table1) {
        table1Map[row.id] = row;
    }
    for (const auto& row : table2) {
        table2Map[row.id] = row;
    }

    for (const auto& row1 : table1) {
        if (table2Map.find(row1.foreignKey) != table2Map.end()) {
            result.emplace_back(row1, table2Map[row1.foreignKey]);
        }
        else {
            result.emplace_back(row1, Row{ "NULL", "NULL", "NULL" });
        }
    }

    for (const auto& row2 : table2) {
        if (table1Map.find(row2.foreignKey) == table1Map.end()) {
            result.emplace_back(Row{ "NULL", "NULL", "NULL" }, row2);
        }
    }

    return result;
}

// Function to display join results
void displayJoinResult(const vector<pair<Row, Row>>& result) {
    for (const auto& pair : result) {
        cout << CYAN << "Table 1: " << RESET << pair.first.name << " | "
            << CYAN << "Table 2: " << RESET << pair.second.name << endl;
    }
}

void joinMenu() {
    vector<Row> students = readDataFromFile("students.txt");
    vector<Row> faculty = readDataFromFile("faculty.txt");
    vector<Row> gpa = readDataFromFile("gpa.txt");
    vector<Row> courses = readDataFromFile("courses.txt");

    int joinChoice;
    setColor(11); // Cyan
    cout << "\nSelect a join type:\n";
    cout << "1. Inner Join\n";
    cout << "2. Left Join\n";
    cout << "3. Right Join\n";
    cout << "4. Full Outer Join\n";
    setColor(7); // Reset to white
    cout << "Enter your choice: ";
    cin >> joinChoice;

    int tableChoice1, tableChoice2;
    setColor(11); // Cyan
    cout << "\nSelect tables to join:\n";
    cout << "1. Students\n";
    cout << "2. Faculty\n";
    cout << "3. GPA\n";
    cout << "4. Course\n";
    setColor(7); // Reset to white
    cout << "Enter the table number for Table 1: ";
    cin >> tableChoice1;
    cout << "Enter the table number for Table 2: ";
    cin >> tableChoice2;

    vector<Row> table1, table2;

    switch (tableChoice1) {
    case 1: table1 = students; break;
    case 2: table1 = faculty; break;
    case 3: table1 = gpa; break;
    case 4: table1 = courses; break;
    default:
        setColor(12); // Red
        cout << "Invalid table choice" << RESET << endl;
        return;
    }
    switch (tableChoice2) {
    case 1: table2 = students; break;
    case 2: table2 = faculty; break;
    case 3: table2 = gpa; break;
    case 4: table2 = courses; break;
    default:
        setColor(12); // Red
        cout << "Invalid table choice" << RESET << endl;
        return;
    }

    vector<pair<Row, Row>> result;

    switch (joinChoice) {
    case 1:
        result = innerJoin(table1, table2);
        break;
    case 2:
        result = leftJoin(table1, table2);
        break;
    case 3:
        result = rightJoin(table1, table2);
        break;
    case 4:
        result = fullOuterJoin(table1, table2);
        break;
    default:
        setColor(12); // Red
        cout << "Invalid join choice" << RESET << endl;
        return;
    }

    setColor(10); // Green
    cout << "\nJoin Result:\n";
    setColor(7); // Reset to white
    displayJoinResult(result);
}

void showMainMenu() {
    displayHeading("Academic Management System");
    setColor(11); // Cyan
    cout << "1. Manage Students\n";
    cout << "2. Manage Faculty\n";
    cout << "3. Manage Courses\n";
    cout << "4. Manage GPA Records\n";
    cout << "5. Perform a Join\n";
    cout << "6. Exit\n";
    setColor(7); // Reset to white
    cout << "Enter your choice: ";
}

//--------------------------------------------------- Student class ----------------------------------------------
class Student {
public:
    string id, name, father_name, address, contact_no, program, course_id;
    double cgpa;

    bool operator<(const Student& other) const {
        return id < other.id;
    }
};

class BSTNode {
public:
    Student student;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Student& s) : student(s), left(nullptr), right(nullptr) {}
};

class StudentManager {
private:
    BSTNode* root;
    const string filename = "students.txt";

    void insert(BSTNode*& node, const Student& student) {
        if (!node) {
            node = new BSTNode(student);
        }
        else if (student < node->student) {
            insert(node->left, student);
        }
        else if (node->student < student) {
            insert(node->right, student);
        }
        else {
            setColor(12); // Red
            cout << "Duplicate student record! Cannot add.\n";
            setColor(7); // Reset to white
        }
    }

    void display(BSTNode* node) const {
        if (node) {
            display(node->left);
            printSeparator();
            setColor(10); // Green
            cout << "ID: " << node->student.id << "\n";
            cout << "Name: " << node->student.name << "\n";
            cout << "Father's Name: " << node->student.father_name << "\n";
            cout << "Address: " << node->student.address << "\n";
            cout << "Contact No: " << node->student.contact_no << "\n";
            cout << "Program: " << node->student.program << "\n";
            cout << "Enrolled course: " << node->student.course_id << "\n";
            cout << fixed << setprecision(2) << "CGPA: " << node->student.cgpa << "\n";
            setColor(7); // Reset to white
            printSeparator();
            display(node->right);
        }
    }

    BSTNode* search(BSTNode* node, const string& id) const {
        if (!node || node->student.id == id) {
            return node;
        }
        if (id < node->student.id) {
            return search(node->left, id);
        }
        return search(node->right, id);
    }

    BSTNode* remove(BSTNode*& node, const string& id) {
        if (!node) return nullptr;

        if (id < node->student.id) {
            node->left = remove(node->left, id);
        }
        else if (node->student.id < id) {
            node->right = remove(node->right, id);
        }
        else {
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                BSTNode* temp = node->right;
                while (temp && temp->left) {
                    temp = temp->left;
                }
                node->student = temp->student;
                node->right = remove(node->right, temp->student.id);
            }
        }
        return node;
    }

    void saveToFile(BSTNode* node) const {
        // Open the file in append mode instead of truncating it
        ofstream file(filename, ios::trunc); // Append to file
        if (file.is_open()) {
            saveToFileRec(node, file);
            file.close();
        }
    }

    void saveToFileRec(BSTNode* node, ofstream& file) const {
        if (node) {
            saveToFileRec(node->left, file);
            string data = node->student.id + "," +
                node->student.name + "," +
                node->student.father_name + "," +
                node->student.address + "," +
                node->student.contact_no + "," +
                node->student.program + "," +
                node->student.course_id + "," +
                to_string(node->student.cgpa);
            file << xorEncryptDecrypt(data, 'K') << endl; // Encrypt before saving
            saveToFileRec(node->right, file);
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                line = xorEncryptDecrypt(line, 'K'); // Decrypt the line
                stringstream ss(line);
                Student student;
                string cgpaString;

                getline(ss, student.id, ',');
                getline(ss, student.name, ',');
                getline(ss, student.father_name, ',');
                getline(ss, student.address, ',');
                getline(ss, student.contact_no, ',');
                getline(ss, student.program, ',');
                getline(ss, student.course_id, ',');
                getline(ss, cgpaString);

                // Validating CGPA before storing
                if (isValidDouble(cgpaString)) {
                    student.cgpa = stod(cgpaString);
                    if (validateGPA(student.cgpa)) {
                        insert(student);
                    }
                    else {
                        cerr << "Warning: Invalid CGPA for student ID " << student.id << ". Must be between 0.0 and 4.0. Skipping record.\n";
                    }
                }
                else {
                    cerr << "Warning: Invalid CGPA for student ID " << student.id << ". Skipping record.\n";
                }
            }
            file.close();
        }
    }

public:
    StudentManager() : root(nullptr) {
        loadFromFile();
    }

    void insert(const Student& student) {
        insert(root, student);
        saveToFile(root); // Save data to file after insertion
    }

    void display() const {
        display(root);
    }

    BSTNode* search(const string& id) const {
        return search(root, id);
    }

    void update(const Student& student) {
        remove(student.id);
        insert(student);
    }

    void remove(const string& id) {
        root = remove(root, id);
        saveToFile(root); // Save data to file after deletion
    }

    ~StudentManager() {
        // Ideally implement a destructor to free memory
    }
};

// -------------------------------------------Faculty class with a BST structure
class Faculty {
public:
    string id, name, department, contact_no, position;
    double salary;

    bool operator<(const Faculty& other) const {
        return id < other.id;
    }
};

class FacultyNode {
public:
    Faculty faculty;
    FacultyNode* left;
    FacultyNode* right;

    FacultyNode(const Faculty& f) : faculty(f), left(nullptr), right(nullptr) {}
};

class FacultyManager {
private:
    FacultyNode* root;
    const string filename = "faculty.txt";

    void insert(FacultyNode*& node, const Faculty& faculty) {
        if (!node) {
            node = new FacultyNode(faculty);
        }
        else if (faculty < node->faculty) {
            insert(node->left, faculty);
        }
        else if (node->faculty < faculty) {
            insert(node->right, faculty);
        }
        else {
            setColor(12); // Red
            cout << "Duplicate faculty record! Cannot add.\n";
            setColor(7); // Reset to white
        }
    }

    void display(FacultyNode* node) const {
        if (node) {
            display(node->left);
            printSeparator();
            setColor(10); // Green
            cout << "ID: " << node->faculty.id << "\n";
            cout << "Name: " << node->faculty.name << "\n";
            cout << "Department: " << node->faculty.department << "\n";
            cout << "Contact No: " << node->faculty.contact_no << "\n";
            cout << "Position: " << node->faculty.position << "\n";
            cout << fixed << setprecision(2) << "Salary: " << node->faculty.salary << "\n";
            setColor(7); // Reset to white
            printSeparator();
            display(node->right);
        }
    }

    FacultyNode* search(FacultyNode* node, const string& id) const {
        if (!node || node->faculty.id == id) {
            return node;
        }
        if (id < node->faculty.id) {
            return search(node->left, id);
        }
        return search(node->right, id);
    }

    FacultyNode* remove(FacultyNode*& node, const string& id) {
        if (!node) return nullptr;

        if (id < node->faculty.id) {
            node->left = remove(node->left, id);
        }
        else if (node->faculty.id < id) {
            node->right = remove(node->right, id);
        }
        else {
            if (!node->left) {
                FacultyNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                FacultyNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                FacultyNode* temp = node->right;
                while (temp && temp->left) {
                    temp = temp->left;
                }
                node->faculty = temp->faculty;
                node->right = remove(node->right, temp->faculty.id);
            }
        }
        return node;
    }

    void saveToFile(FacultyNode* node) const {
        // Open the file in append mode instead of truncating it
        ofstream file(filename, ios::trunc); 
        if (file.is_open()) {
            saveToFileRec(node, file);
            file.close();
        }
    }

    void saveToFileRec(FacultyNode* node, ofstream& file) const {
        if (node) {
            saveToFileRec(node->left, file);
            std::string data = node->faculty.id + "," +
                node->faculty.name + "," +
                node->faculty.department + "," +
                node->faculty.contact_no + "," +
                node->faculty.position + "," +
                to_string(node->faculty.salary);
            file << xorEncryptDecrypt(data, 'K') << endl; // Encrypt before saving
            saveToFileRec(node->right, file);
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                line = xorEncryptDecrypt(line, 'K'); // Decrypt the line
                stringstream ss(line);
                Faculty faculty;
                string salaryString;

                getline(ss, faculty.id, ',');
                getline(ss, faculty.name, ',');
                getline(ss, faculty.department, ',');
                getline(ss, faculty.contact_no, ',');
                getline(ss, faculty.position, ',');
                getline(ss, salaryString);

                // Validating Salary before storing
                if (isValidDouble(salaryString)) {
                    faculty.salary = stod(salaryString);
                    insert(faculty);
                }
                else {
                    cerr << "Warning: Invalid salary for faculty ID " << faculty.id << ". Skipping record.\n";
                }
            }
            file.close();
        }
    }

public:
    FacultyManager() : root(nullptr) {
        loadFromFile();
    }

    void insert(const Faculty& faculty) {
        insert(root, faculty);
        saveToFile(root); // Save data to file after insertion
    }

    void display() const {
        display(root);
    }

    FacultyNode* search(const string& id) const {
        return search(root, id);
    }

    void update(const Faculty& faculty) {
        remove(faculty.id);
        insert(faculty);
    }

    void remove(const string& id) {
        root = remove(root, id);
        saveToFile(root); // Save data to file after deletion
    }

    ~FacultyManager() {
    }
};

// Course class with a BST structure
class Course {
public:
    string code, name, department, faculty_id;
    int credits;

    bool operator<(const Course& other) const {
        return code < other.code;
    }
};

class CourseNode {
public:
    Course course;
    CourseNode* left;
    CourseNode* right;

    CourseNode(const Course& c) : course(c), left(nullptr), right(nullptr) {}
};

class CourseManager {
private:
    CourseNode* root;
    const string filename = "courses.txt";

    void insert(CourseNode*& node, const Course& course) {
        if (!node) {
            node = new CourseNode(course);
        }
        else if (course < node->course) {
            insert(node->left, course);
        }
        else if (node->course < course) {
            insert(node->right, course);
        }
        else {
            setColor(12); // Red
            cout << "Duplicate course record! Cannot add.\n";
            setColor(7); // Reset to white
        }
    }

    void display(CourseNode* node) const {
        if (node) {
            display(node->left);
            printSeparator();
            setColor(10); // Green
            cout << "Course Code: " << node->course.code << "\n";
            cout << "Course Name: " << node->course.name << "\n";
            cout << "Department: " << node->course.department << "\n";
            cout << "Faculty ID: " << node->course.faculty_id << "\n";
            cout << "Credits: " << node->course.credits << "\n";
            setColor(7); // Reset to white
            printSeparator();
            display(node->right);
        }
    }

    CourseNode* search(CourseNode* node, const string& code) const {
        if (!node || node->course.code == code) {
            return node;
        }
        if (code < node->course.code) {
            return search(node->left, code);
        }
        return search(node->right, code);
    }

    CourseNode* remove(CourseNode*& node, const string& code) {
        if (!node) return nullptr;

        if (code < node->course.code) {
            node->left = remove(node->left, code);
        }
        else if (node->course.code < code) {
            node->right = remove(node->right, code);
        }
        else {
            if (!node->left) {
                CourseNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                CourseNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                CourseNode* temp = node->right;
                while (temp && temp->left) {
                    temp = temp->left;
                }
                node->course = temp->course;
                node->right = remove(node->right, temp->course.code);
            }
        }
        return node;
    }

    void saveToFile(CourseNode* node) const {
        // Open the file in append mode instead of truncating it
        ofstream file(filename, ios::trunc);
        if (file.is_open()) {
            saveToFileRec(node, file);
            file.close();
        }
    }

    void saveToFileRec(CourseNode* node, ofstream& file) const {
        if (node) {
            saveToFileRec(node->left, file);
            string data = node->course.code + "," +
                node->course.name + "," +
                node->course.department + "," +
                node->course.faculty_id + "," +
                to_string(node->course.credits);
            file << xorEncryptDecrypt(data, 'K') << endl; // Encrypt before saving
            saveToFileRec(node->right, file);
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                line = xorEncryptDecrypt(line, 'K'); // Decrypt the line
                stringstream ss(line);
                Course course;
                string creditsString;

                getline(ss, course.code, ',');
                getline(ss, course.name, ',');
                getline(ss, course.department, ',');
                getline(ss, course.faculty_id, ',');
                getline(ss, creditsString);

                // Validating Credits before storing
                if (isValidInteger(creditsString)) {
                    course.credits = stoi(creditsString);
                    insert(course);
                }
                else {
                    cerr << "Warning: Invalid credits for course code " << course.code << ". Skipping record.\n";
                }
            }
            file.close();
        }
    }

public:
    CourseManager() : root(nullptr) {
        loadFromFile();
    }

    void insert(const Course& course) {
        insert(root, course);
        saveToFile(root); // Save data to file after insertion
    }

    void display() const {
        display(root);
    }

    CourseNode* search(const string& code) const {
        return search(root, code);
    }

    void update(const Course& course) {
        remove(course.code);
        insert(course);
    }

    void remove(const string& code) {
        root = remove(root, code);
        saveToFile(root); // Save data to file after deletion
    }

    ~CourseManager() {

    }
};

// GPARecord class with a BST structure
class GPARecord {
public:
    string studentId;
    double gpa;

    bool operator<(const GPARecord& other) const {
        return studentId < other.studentId;
    }
};

class GPARecordNode {
public:
    GPARecord record;
    GPARecordNode* left;
    GPARecordNode* right;

    GPARecordNode(const GPARecord& r) : record(r), left(nullptr), right(nullptr) {}
};

class GPARecordManager {
private:
    GPARecordNode* root;
    const string filename = "gpa.txt";

    void insert(GPARecordNode*& node, const GPARecord& record) {
        if (!node) {
            node = new GPARecordNode(record);
        }
        else if (record < node->record) {
            insert(node->left, record);
        }
        else if (node->record < record) {
            insert(node->right, record);
        }
        else {
            setColor(12); // Red
            cout << "Duplicate GPA record! Cannot add.\n";
            setColor(7); // Reset to white
        }
    }

    void display(GPARecordNode* node) const {
        if (node) {
            display(node->left);
            printSeparator();
            setColor(10); // Green
            cout << "Student ID: " << node->record.studentId << "\n";
            cout << fixed << setprecision(2) << "GPA: " << node->record.gpa << "\n";
            setColor(7); // Reset to white
            printSeparator();
            display(node->right);
        }
    }

    GPARecordNode* search(GPARecordNode* node, const string& studentId) const {
        if (!node || node->record.studentId == studentId) {
            return node;
        }
        if (studentId < node->record.studentId) {
            return search(node->left, studentId);
        }
        return search(node->right, studentId);
    }

    GPARecordNode* remove(GPARecordNode*& node, const string& studentId) {
        if (!node) return nullptr;

        if (studentId < node->record.studentId) {
            node->left = remove(node->left, studentId);
        }
        else if (node->record.studentId < studentId) {
            node->right = remove(node->right, studentId);
        }
        else {
            if (!node->left) {
                GPARecordNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                GPARecordNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                GPARecordNode* temp = node->right;
                while (temp && temp->left) {
                    temp = temp->left;
                }
                node->record = temp->record;
                node->right = remove(node->right, temp->record.studentId);
            }
        }
        return node;
    }

    void saveToFile(GPARecordNode* node) const {
        // Open the file in append mode instead of truncating it
        ofstream file(filename, ios::trunc); 
        if (file.is_open()) {
            saveToFileRec(node, file);
            file.close();
        }
    }

    void saveToFileRec(GPARecordNode* node, ofstream& file) const {
        if (node) {
            saveToFileRec(node->left, file);
            file << xorEncryptDecrypt(node->record.studentId + "," + to_string(node->record.gpa), 'K') << endl; // Encrypt before saving
            saveToFileRec(node->right, file);
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                line = xorEncryptDecrypt(line, 'K'); // Decrypt the line
                stringstream ss(line);
                GPARecord record;

                getline(ss, record.studentId, ',');
                string gpaString;
                getline(ss, gpaString);

                // Validating GPA before storing
                if (isValidDouble(gpaString)) {
                    record.gpa = stod(gpaString);
                    if (validateGPA(record.gpa)) {
                        insert(record);
                    }
                    else {
                        cerr << "Warning: Invalid GPA for student ID " << record.studentId << ". Must be between 0.0 and 4.0. Skipping record.\n";
                    }
                }
                else {
                    cerr << "Warning: Invalid GPA for student ID " << record.studentId << ". Skipping record.\n";
                }
            }
            file.close();
        }
    }

public:
    GPARecordManager() : root(nullptr) {
        loadFromFile();
    }

    void insert(const GPARecord& record) {
        insert(root, record);
        saveToFile(root); // Save data to file after insertion
    }

    void display() const {
        display(root);
    }

    GPARecordNode* search(const string& studentId) const {
        return search(root, studentId);
    }

    void update(const GPARecord& record) {
        remove(record.studentId);
        insert(record);
    }

    void remove(const string& studentId) {
        root = remove(root, studentId);
        saveToFile(root); // Save data to file after deletion
    }

    ~GPARecordManager() {}
};

// Main Function
int main() {
    StudentManager studentManager;
    FacultyManager facultyManager;
    CourseManager courseManager;
    GPARecordManager gpaRecordManager;

    cout << "\n\n\n\t\t\t\t\t" << GREEN << "Welcome TO AU Database System" << RESET << endl;
    cout << "\t\t\t\t==============================================" << endl;
    cout << "\n\t\t\t\t\t\t" << YELLOW << "LOGIN TO SYSTEM" << RESET << endl;

    string username, password;
    cout << "\n\t\t\t\t" << CYAN << "Enter username: " << RESET;
    cin >> username;
    cout << "\t\t\t\t" << CYAN << "Enter password: " << RESET;
    cin >> password;

    const string filename = "loginID.txt"; // File containing username-password pairs
    if (login(username, password, filename)) {
        cout << GREEN << "\n\t\t\t\tLogin successful! Proceeding...\n" << RESET;
        cout << "\n\t\t\t";

        // Progress Bar Animation
        for (int j = 0; j < 30; j++) {
            Sleep(200);
            cout << YELLOW << (char)219 << RESET;
        }

        int choice;
        do {
            showMainMenu();
            cin >> choice;

            while (!isValidInteger(to_string(choice))) {
                setColor(12); // Red
                cout << "Invalid choice! Please enter a valid number: ";
                cin >> choice;
                setColor(7); // Reset to white
            }

            switch (choice) {
            case 1: {
                int studentChoice;
                do {
                    displayHeading("Manage Students");
                    setColor(11); // Cyan
                    cout << "1. Add Student\n";
                    cout << "2. Search Student\n";
                    cout << "3. Print All Students\n";
                    cout << "4. Delete Student\n";
                    cout << "5. Update Student\n";
                    cout << "6. Back to Main Menu\n";
                    setColor(7); // Reset to white
                    cout << "Enter your choice: ";
                    cin >> studentChoice;

                    switch (studentChoice) {
                    case 1: { //--------------insert student-------------------------
                        Student student;

                        cout << "Enter Student ID (6 digits only): ";
                        cin >> student.id;
                        // Validate the input
                        while (!isValidStudentID(student.id)) {
                            cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                            cin >> student.id; // Read input again
                        }

                        cout << "Enter Name (no numbers): ";
                        allocateName(student.name);

                        cout << "Enter Father's Name (no numbers): ";
                        allocateName(student.father_name);

                        cout << "Enter Address: ";
                        getline(cin, student.address);

                        cout << "Enter Contact No (11 digits): ";
                        cin >> student.contact_no;
                        while (!isValidContactNumber(student.contact_no)) {
                            setColor(12);
                            cout << "Contact number must be exactly 11 digits, only numbers allowed. Please enter again: ";
                            cin >> student.contact_no;
                        }

                        cout << "Enter Program: ";
                        allocateName(student.program);

                        cout << "Enter Course ID: ";
                        cin >> student.course_id;
                        // Validate the input
                        while (!isValidStudentID(student.course_id)) {
                            cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                            cin >> student.course_id; // Read input again
                        }

                        // CGPA Input with exception handling
                        string cgpaInput;
                        cout << "Enter CGPA (0.0 to 4.0): ";
                        cin >> cgpaInput;

                        // Loop until a valid CGPA is entered
                        while (true) {
                            try {
                                student.cgpa = stod(cgpaInput);  // Convert CGPA to double
                                if (!validateGPA(student.cgpa)) {
                                    throw invalid_argument("GPA must be between 0.0 and 4.0");
                                }
                                break; // Valid input, exit loop
                            }
                            catch (const invalid_argument&) {
                                setColor(12);
                                cout << "Invalid CGPA! Please enter a valid number between 0.0 and 4.0: ";
                                cin >> cgpaInput; // Attempt to read again
                            }
                            catch (const out_of_range&) {
                                setColor(12);
                                cout << "CGPA out of range! Please enter a valid number: ";
                                cin >> cgpaInput; // Attempt to read again
                            }
                        }

                        // Insert student.
                        studentManager.insert(student);
                        break;
                    }
                    case 2: {
                        string id;
                        cout << "Enter Student ID to search: ";
                        cin >> id;
                        BSTNode* result = studentManager.search(id);
                        if (result) {
                            setColor(10); // Green
                            cout << "Student record found:\n";
                            setColor(7); // Reset to white
                            printSeparator();
                            setColor(10); // Green
                            cout << "ID: " << result->student.id << "\n";
                            cout << "Name: " << result->student.name << "\n";
                            cout << "Father's Name: " << result->student.father_name << "\n";
                            cout << "Address: " << result->student.address << "\n";
                            cout << "Contact No: " << result->student.contact_no << "\n";
                            cout << "Program: " << result->student.program << "\n";
                            cout << "Enrolled Course: " << result->student.course_id << "\n";
                            cout << "CGPA: " << result->student.cgpa << "\n";
                            setColor(7); // Reset to white
                            printSeparator();
                        }
                        else {
                            setColor(12); // Red
                            cout << "Student record not found!\n";
                            setColor(7); // Reset to white
                        }
                        break;
                    }
                    case 3:
                        studentManager.display();
                        break;
                    case 4: {
                        string id;
                        cout << "Enter Student ID to delete: ";
                        cin >> id;
                        studentManager.remove(id);
                        break;
                    }
                    case 5: { // Update Student
                        string id;
                        cout << "Enter Student ID to update: ";
                        cin >> id;

                        // Search for the student record
                        BSTNode* result = studentManager.search(id);
                        if (result) {
                            Student student = result->student; // Store the current data in a temporary variable

                            cout << "Current Data:\n";
                            cout << "1. ID: " << student.id << "\n";
                            cout << "2. Name: " << student.name << "\n";
                            cout << "3. Father's Name: " << student.father_name << "\n";
                            cout << "4. Address: " << student.address << "\n";
                            cout << "5. Contact No: " << student.contact_no << "\n";
                            cout << "6. Program: " << student.program << "\n";
                            cout << "7. Enrolled Course: " << student.course_id << "\n";
                            cout << "8. CGPA: " << student.cgpa << "\n";

                            int updateChoice;
                            cout << "Select the field to update (1-8): ";
                            cin >> updateChoice;

                            switch (updateChoice) {
                            case 1: // Update ID
                                cout << "Enter new Student ID (digits only): ";
                                cin >> student.id; // Read input as a string
                                // Validate the input
                                while (!isValidStudentID(student.id)) {
                                    cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                                    cin >> student.id; // Read input again
                                }
                                break;
                            case 2: // Update Name
                                cout << "Enter new Name (no numbers): ";
                                allocateName(student.name);
                                break;
                            case 3: // Update Father's Name
                                cout << "Enter new Father's Name (no numbers): ";
                                allocateName(student.father_name);
                                break;
                            case 4: // Update Address
                                cout << "Enter new Address: ";
                                getline(cin, student.address);
                                break;
                            case 5: // Update Contact No
                                cout << "Enter new Contact No: ";
                                cin >> student.contact_no;
                                while (!isValidContactNumber(student.contact_no)) {
                                    cout << RED << "Contact number must be exactly 11 digits, only numbers allowed. Please enter again: " << RESET;
                                    cin >> student.contact_no;
                                }
                                break;
                            case 6: // Update Program
                                cout << "Enter new Program: ";
                                allocateName(student.program);
                                break;
                            case 7:
                                cout << "Enter new Course ID: ";
                                cin >> student.course_id; // Read input as a string
                                // Validate the input
                                while (!isValidStudentID(student.course_id)) {
                                    cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                                    cin >> student.course_id; // Read input again
                                }
                            case 8: // Update CGPA
                                while (true) {
                                    string cgpaInput;
                                    cout << "Enter new CGPA (0.0 to 4.0): ";
                                    cin >> cgpaInput;
                                    try {
                                        student.cgpa = stod(cgpaInput);
                                        if (!validateGPA(student.cgpa)) {
                                            throw invalid_argument("GPA must be between 0.0 and 4.0");
                                        }
                                        break; // Valid input, exit loop
                                    }
                                    catch (const std::invalid_argument&) {

                                        cout << RED << "Invalid CGPA! Please enter a valid number between 0.0 and 4.0: " << RESET;
                                    }
                                }
                                break;
                            default:
                                cout << "Invalid choice!\n";
                                continue;
                            }

                            // Update the student record
                            studentManager.update(student); // Use update function
                            cout << "Student record updated successfully.\n";
                        }
                        else {
                            cout << "Student record not found!\n";
                        }
                        break;
                    }
                    case 6:
                        cout << "Returning to main menu...\n";
                        break;
                    default:
                        setColor(12); // Red
                        cout << "Invalid choice! Please try again.\n";
                        setColor(7); // Reset to white
                    }
                } while (studentChoice != 6);
                break;
            }
            case 2: {
                int facultyChoice;
                do {
                    displayHeading("Manage Faculty");
                    setColor(11); // Cyan
                    cout << "1. Add Faculty\n";
                    cout << "2. Delete Faculty\n";
                    cout << "3. Search Faculty\n";
                    cout << "4. Print All Faculty\n";
                    cout << "5. Update Faculty\n"; // Added update option
                    cout << "6. Back to Main Menu\n";
                    setColor(7); // Reset to white
                    cout << "Enter your choice: ";
                    cin >> facultyChoice;

                    switch (facultyChoice) {
                    case 1: {
                        Faculty faculty;

                        // Get Faculty ID (digits only)
                        cout << "Enter Faculty ID (digits only): ";
                        cin >> faculty.id; // Read input as a string
                        // Validate the input
                        while (!isValidStudentID(faculty.id)) {
                            cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                            cin >> faculty.id; // Read input again
                        }

                        // Get Name
                        cout << "Enter Name (no numbers): ";
                        allocateName(faculty.name);

                        // Get Department
                        cout << "Enter Department: ";
                        allocateName(faculty.department);

                        // Get position
                        cout << "Enter Position: ";
                        allocateName(faculty.position);

                        // Handle Salary input
                        string salaryInput;
                        while (true) {
                            cout << "Enter Salary: ";
                            cin >> salaryInput;
                            try {
                                faculty.salary = stod(salaryInput); // Convert salary to double
                                break; // Exit loop on valid input
                            }
                            catch (const invalid_argument&) {

                                cout << RED << "Invalid Salary! Please enter a valid number: " << RESET;
                            }
                            catch (const out_of_range&) {

                                cout << RED << "Salary out of range! Please enter a valid number: " << RESET;
                            }
                        }

                        // Validate contact number
                        cout << "Enter Contact No (11 digits): ";
                        cin >> faculty.contact_no;
                        while (!isValidContactNumber(faculty.contact_no)) {
                            setColor(12);
                            cout << "Contact number must be exactly 11 digits, only numbers allowed. Please enter again: ";
                            cin >> faculty.contact_no;
                        }

                        // Insert faculty
                        facultyManager.insert(faculty);
                        break;
                    }
                    case 2: {
                        string id;
                        cout << "Enter Faculty ID to delete: ";
                        cin >> id;
                        facultyManager.remove(id);
                        break;
                    }
                    case 3: {
                        string id;
                        cout << "Enter Faculty ID to search: ";
                        cin >> id;
                        FacultyNode* result = facultyManager.search(id);
                        if (result) {
                            setColor(10); // Green
                            cout << "Faculty record found:\n";
                            setColor(7); // Reset to white
                            printSeparator();
                            setColor(10); // Green
                            cout << "ID: " << result->faculty.id << "\n";
                            cout << "Name: " << result->faculty.name << "\n";
                            cout << "Department: " << result->faculty.department << "\n";
                            cout << "Contact No: " << result->faculty.contact_no << "\n";
                            cout << "Position: " << result->faculty.position << "\n";
                            cout << "Salary: " << result->faculty.salary << "\n";
                            setColor(7); // Reset to white
                            printSeparator();
                        }
                        else {
                            setColor(12); // Red
                            cout << "Faculty record not found!\n";
                            setColor(7); // Reset to white
                        }
                        break;
                    }
                    case 4:
                        facultyManager.display();
                        break;
                    case 5: { // Update Faculty
                        string id;
                        cout << "Enter Faculty ID to update: ";
                        cin >> id;

                        // Search for the faculty record
                        FacultyNode* result = facultyManager.search(id);
                        if (result) {
                            Faculty faculty = result->faculty;

                            cout << "Current Data:\n";
                            cout << "1. ID: " << faculty.id << "\n";
                            cout << "2. Name: " << faculty.name << "\n";
                            cout << "3. Department: " << faculty.department << "\n";
                            cout << "4. Contact No: " << faculty.contact_no << "\n";
                            cout << "5. Position: " << faculty.position << "\n";
                            cout << "6. Salary: " << faculty.salary << "\n";

                            int updateChoice;
                            cout << "Select the field to update (1-6): ";
                            cin >> updateChoice;

                            switch (updateChoice) {
                            case 1: // Update ID
                                cout << "Enter new Faculty ID (6 digits only): ";
                                cin >> faculty.id; // Read input as a string
                                // Validate the input
                                while (!isValidStudentID(faculty.id)) {
                                    cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                                    cin >> faculty.id; // Read input again
                                }
                                break;
                            case 2: // Update Name
                                cout << "Enter new Name (no numbers): ";
                                allocateName(faculty.name);
                                break;
                            case 3: // Update Department
                                cout << "Enter new Department: ";
                                allocateName(faculty.department);
                                break;
                            case 4: // Update Contact No
                                cout << "Enter new Contact No: ";
                                cin >> faculty.contact_no;
                                while (!isValidContactNumber(faculty.contact_no)) {
                                    setColor(12);
                                    cout << "Contact number must be exactly 11 digits, only numbers allowed. Please enter again: ";
                                    cin >> faculty.contact_no;
                                }
                                break;
                            case 5: // Update Position
                                cout << "Enter new Position: ";
                                allocateName(faculty.position);
                                break;
                            case 6: // Update Salary
                                while (true) {
                                    string salaryInput;
                                    cout << "Enter new Salary: ";
                                    cin >> salaryInput;
                                    try {
                                        faculty.salary = stod(salaryInput);
                                        break;
                                    }
                                    catch (...) {
                                        setColor(12);
                                        cout << "Invalid Salary! Please enter a valid number: ";
                                    }
                                }
                                break;
                            default:
                                cout << "Invalid choice!\n";
                                continue;
                            }

                            // Update the faculty record
                            facultyManager.update(faculty); // Use update function
                            cout << "Faculty record updated successfully.\n";
                        }
                        else {
                            cout << "Faculty record not found!\n";
                        }
                        break;
                    }
                    case 6:
                        cout << "Returning to main menu...\n";
                        break;
                    default:
                        setColor(12); // Red
                        cout << "Invalid choice! Please try again.\n";
                        setColor(7); // Reset to white
                    }
                } while (facultyChoice != 6);
                break;
            }
            case 3: {
                int courseChoice;
                do {
                    displayHeading("Manage Courses");
                    setColor(11); // Cyan
                    cout << "1. Add Course\n";
                    cout << "2. Delete Course\n";
                    cout << "3. Search Course\n";
                    cout << "4. Print All Courses\n";
                    cout << "5. Update Course\n"; // Added update option
                    cout << "6. Back to Main Menu\n";
                    setColor(7); // Reset to white
                    cout << "Enter your choice: ";
                    cin >> courseChoice;

                    switch (courseChoice) {
                    case 1: {
                        Course course;

                        // Get Course Code (digits only)
                        cout << "Enter Course Code (6 digits only): ";
                        cin >> course.code; // Read input as a string
                        // Validate the input
                        while (!isValidStudentID(course.code)) {
                            cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                            cin >> course.code; // Read input again
                        }

                        // Get Course Name
                        cout << "Enter Course Name: ";
                        allocateName(course.name);

                        cout << "Enter Department: ";
                        allocateName(course.department);

                        cout << "Enter Faculty ID: ";
                        cin >> course.faculty_id; // Read input as a string
                        // Validate the input
                        while (!isValidStudentID(course.faculty_id)) {
                            cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                            cin >> course.faculty_id; // Read input again
                        }

                        // Get Credits
                        string creditsInput;
                        while (true) {
                            cout << "Enter Credits: ";
                            cin >> creditsInput;
                            if (isValidInteger(creditsInput)) {
                                course.credits = stoi(creditsInput);
                                break; // Exit loop on valid input
                            }
                            else {

                                cout << RED << "Invalid Credits! Please enter a valid number: " << RESET;
                            }
                        }

                        // Insert course
                        courseManager.insert(course);
                        break;
                    }

                    case 2: {
                        string code;
                        cout << "Enter Course Code to delete: ";
                        cin >> code;
                        courseManager.remove(code);
                        break;
                    }
                    case 3: {
                        string code;
                        cout << "Enter Course Code to search: ";
                        cin >> code;
                        CourseNode* result = courseManager.search(code);
                        if (result) {
                            setColor(10); // Green
                            cout << "Course record found:\n";
                            setColor(7); // Reset to white
                            printSeparator();
                            setColor(10); // Green
                            cout << "Course Code: " << result->course.code << "\n";
                            cout << "Course Name: " << result->course.name << "\n";
                            cout << "Department: " << result->course.department << "\n";
                            cout << "Faculty ID: " << result->course.faculty_id << "\n";
                            cout << "Credits: " << result->course.credits << "\n";
                            setColor(7); // Reset to white
                            printSeparator();
                        }
                        else {
                            setColor(12); // Red
                            cout << "Course record not found!\n";
                            setColor(7); // Reset to white
                        }
                        break;
                    }
                    case 4:
                        courseManager.display();
                        break;
                    case 5: { // Update Course
                        string code;
                        cout << "Enter Course Code to update: ";
                        cin >> code;

                        // Search for the course record
                        CourseNode* result = courseManager.search(code);
                        if (result) {
                            Course course = result->course;

                            cout << "Current Data:\n";
                            cout << "1. Course Code: " << course.code << "\n";
                            cout << "2. Course Name: " << course.name << "\n";
                            cout << "3. Department: " << course.department << "\n";
                            cout << "4. Faculty ID: " << course.faculty_id << "\n";
                            cout << "5. Credits: " << course.credits << "\n";

                            int updateChoice;
                            cout << "Select the field to update (1-5): ";
                            cin >> updateChoice;

                            switch (updateChoice) {
                            case 1: // Update Course Code
                                cout << "Enter new Course Code (digits only): ";
                                cin >> course.code; // Read input as a string
                                // Validate the input
                                while (!isValidStudentID(course.code)) {
                                    cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                                    cin >> course.code; // Read input again
                                }
                                break;
                            case 2: // Update Course Name
                                cout << "Enter new Course Name: ";
                                allocateName(course.name);
                                break;
                            case 3: // Update Department
                                cout << "Enter new Department: ";
                                allocateName(course.department);
                                break;
                            case 4: // Update Instructor
                                cout << "Enter new Faculty ID: ";
                                cin >> course.faculty_id; // Read input as a string
                                // Validate the input
                                while (!isValidStudentID(course.faculty_id)) {
                                    cout << RED << "ID must consist of exactly 6 digits (no spaces or characters) and cannot be empty. Please enter again: " << RESET;
                                    cin >> course.faculty_id; // Read input again
                                }
                                break;
                            case 5: // Update Credits
                                while (true) {
                                    string creditsInput;
                                    cout << "Enter new Credits: ";
                                    cin >> creditsInput;
                                    if (isValidInteger(creditsInput)) {
                                        course.credits = stoi(creditsInput);
                                        break; // Exit loop on valid input
                                    }
                                    else {
                                        setColor(12);
                                        cout << "Invalid Credits! Please enter a valid number: ";
                                    }
                                }
                                break;
                            default:
                                cout << "Invalid choice!\n";
                                continue;
                            }

                            // Update the course record
                            courseManager.update(course); // Use update function
                            cout << "Course record updated successfully.\n";
                        }
                        else {
                            cout << "Course record not found!\n";
                        }
                        break;
                    }
                    case 6:
                        cout << "Returning to main menu...\n";
                        break;
                    default:
                        setColor(12); // Red
                        cout << "Invalid choice! Please try again.\n";
                        setColor(7); // Reset to white
                    }
                } while (courseChoice != 6);
                break;
            }
            case 4: {
                int gpaChoice;
                do {
                    displayHeading("Manage GPA Records");
                    setColor(11); // Cyan
                    cout << "1. Add GPA Record\n";
                    cout << "2. Delete GPA Record\n";
                    cout << "3. Search GPA Record\n";
                    cout << "4. Print All GPA Records\n";
                    cout << "5. Update GPA Record\n"; // Added update option
                    cout << "6. Back to Main Menu\n";
                    setColor(7); // Reset to white
                    cout << "Enter your choice: ";
                    cin >> gpaChoice;

                    switch (gpaChoice) {
                    case 1: {
                        GPARecord record;

                        // Get Student ID (digits only)
                        cout << "Enter Student ID (digits only): ";
                        cin >> record.studentId;
                        cin.ignore();
                        while (!isValidInteger(record.studentId)) {
                            setColor(12);
                            cout << "Student ID must consist of digits only. Please enter again: ";
                            cin >> record.studentId;
                            cin.ignore();
                        }

                        // Handle GPA input
                        string gpaInput;
                        while (true) {
                            cout << "Enter GPA (0.0 to 4.0): ";
                            cin >> gpaInput;
                            if (isValidDouble(gpaInput)) {
                                record.gpa = stod(gpaInput); // Convert GPA to double
                                if (!validateGPA(record.gpa)) {
                                    setColor(12);
                                    cout << "Invalid GPA! It must be between 0.0 and 4.0. Please enter a valid number: ";
                                    continue;
                                }
                                break; // Exit loop on valid input
                            }
                            else {
                                setColor(12);
                                cout << "Invalid GPA! Please enter a valid number: ";
                            }
                        }

                        // Insert GPA record
                        gpaRecordManager.insert(record);
                        break;
                    }
                    case 2: {
                        string studentId;
                        cout << "Enter Student ID to delete: ";
                        cin >> studentId;
                        gpaRecordManager.remove(studentId);
                        break;
                    }
                    case 3: {
                        string studentId;
                        cout << "Enter Student ID to search: ";
                        cin >> studentId;
                        GPARecordNode* result = gpaRecordManager.search(studentId);
                        if (result) {
                            setColor(10); // Green
                            cout << "GPA record found:\n";
                            setColor(7); // Reset to white
                            printSeparator();
                            setColor(10); // Green
                            cout << "Student ID: " << result->record.studentId << "\n";
                            cout << fixed << setprecision(2) << "GPA: " << result->record.gpa << "\n";
                            setColor(7); // Reset to white
                            printSeparator();
                        }
                        else {
                            setColor(12); // Red
                            cout << "GPA record not found!\n";
                            setColor(7); // Reset to white
                        }
                        break;
                    }
                    case 4:
                        gpaRecordManager.display();
                        break;
                    case 5: { // Update GPA
                        string studentId;
                        cout << "Enter Student ID to update GPA: ";
                        cin >> studentId;

                        // Search for the GPA record
                        GPARecordNode* result = gpaRecordManager.search(studentId);
                        if (result) {
                            GPARecord record = result->record;

                            cout << "Current Data:\n";
                            cout << "1. Student ID: " << record.studentId << "\n";
                            cout << "2. GPA: " << record.gpa << "\n";

                            int updateChoice;
                            cout << "Select the field to update (1-2): ";
                            cin >> updateChoice;

                            switch (updateChoice) {
                            case 1: // Update Student ID
                                cout << "Enter new Student ID (digits only): ";
                                cin >> record.studentId;
                                while (!isValidInteger(record.studentId)) {
                                    setColor(12);
                                    cout << "Student ID must consist of digits only. Please enter again: ";
                                    cin >> record.studentId;
                                }
                                break;
                            case 2: // Update GPA
                                while (true) {
                                    string gpaInput;
                                    cout << "Enter new GPA (0.0 to 4.0): ";
                                    cin >> gpaInput;
                                    if (isValidDouble(gpaInput)) {
                                        record.gpa = stod(gpaInput); // Convert GPA to double
                                        if (!validateGPA(record.gpa)) {
                                            setColor(12);
                                            cout << "Invalid GPA! It must be between 0.0 and 4.0. Please enter a valid number: ";
                                            continue;
                                        }
                                        break; // Exit loop on valid input
                                    }
                                    else {
                                        setColor(12);
                                        cout << "Invalid GPA! Please enter a valid number: ";
                                    }
                                }
                                break;
                            default:
                                cout << "Invalid choice!\n";
                                continue;
                            }

                            // Update the GPA record
                            gpaRecordManager.update(record); // Use update function
                            cout << "GPA record updated successfully.\n";
                        }
                        else {
                            cout << "GPA record not found!\n";
                        }
                        break;
                    }
                    case 6:
                        cout << "Returning to main menu...\n";
                        break;
                    default:
                        setColor(12); // Red
                        cout << "Invalid choice! Please try again.\n";
                        setColor(7); // Reset to white
                    }
                } while (gpaChoice != 6);
                break;
            }
            case 5:
                joinMenu(); // Call joinMenu for join functionality
                break;
            case 6:
                displayHeading("Thank You for Using the System");
                Sleep(1000); // Pause before exit
                break;
            default:
                setColor(12); // Red
                cout << "Invalid choice! Please try again.\n";
                setColor(7); // Reset to white
            }
        } while (choice != 6);
    }
    else {
        cout << RED << "Login failed! Invalid username or password." << RESET << endl;
    }

    return 0;
}