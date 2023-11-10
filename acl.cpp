#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

enum class SecurityLevel
{
    Unclassified,
    Confidential,
    Secret,
    TopSecret
};

map<string, SecurityLevel> userSecurity;
map<string, SecurityLevel> fileSecurity;
map<int, SecurityLevel> securityLevels = {{1, SecurityLevel::Unclassified},
                                          {2, SecurityLevel::Confidential},
                                          {3, SecurityLevel::Secret},
                                          {4, SecurityLevel::TopSecret}};

void createUser(const string &userName, SecurityLevel level)
{
    userSecurity[userName] = level;
}

void createFile(const string &fileName, SecurityLevel level)
{
    ofstream file(fileName);
    if (file.is_open())
    {
        file << "This is a secure file named " << fileName << "\n";
        file.close();
    }
    else
    {
        cerr << "Unable to create file " << fileName << endl;
    }
    fileSecurity[fileName] = level;
}

void printInstructions()
{
    cout << YELLOW << "\n1. Create security groups\n"
         << "2. Create users\n"
         << "3. List security information\n"
         << "4. Check access permission\n"
         << "5. Create resources (files)\n"
         << "6. Remove user's security level\n"
         << "7. Exit\n"
         << "Select an option: " << RESET;
}
void createSecurityGroups()
{
    int level;
    cout << CYAN << "Enter number of security level (1-Unclassified, 2-Confidential, 3-Secret, 4-TopSecret): " << RESET;
    cin >> level;
    if (level < 1 || level > 4)
    {
        cout << RED << "Invalid level. Please enter a number between 1 and 4.\n"
             << RESET;
        return;
    }
    string groupName;
    cout << CYAN << "Enter group name: " << RESET;
    cin >> groupName;
    securityLevels[level] = SecurityLevel(level - 1);
    cout << "Security group '" << groupName << "' created with level " << level + 1 << ".\n";
}

void createUsers()
{
    string userName;
    int groupNumber;
    cout << CYAN << "Enter user name: " << RESET;
    cin >> userName;
    cout << CYAN << "Enter number for security group (1-Unclassified, 2-Confidential, 3-Secret, 4-TopSecret): " << RESET;
    cin >> groupNumber;

    if (groupNumber < 1 || groupNumber > 4)
    {
        cout << RED << "Invalid security group number. Please enter a number between 1 and 4.\n"
             << RESET;
        return;
    }

    SecurityLevel level = securityLevels[groupNumber];
    userSecurity[userName] = level;
    cout << GREEN << "User '" << userName << "' created and added to security level " << groupNumber << ".\n"
         << RESET;
}

void listSecurityInformation()
{
    cout << MAGENTA << "\nUsers and their Security Levels:\n"
         << RESET;
    for (const auto &user : userSecurity)
    {
        cout << MAGENTA << user.first << " - Level " << static_cast<int>(user.second) + 1 << RESET << endl;
    }

    cout << MAGENTA << "\nFiles and their Security Levels:\n"
         << RESET;
    for (const auto &file : fileSecurity)
    {
        cout << MAGENTA << file.first << " - Level " << static_cast<int>(file.second) + 1 << RESET << endl;
    }
}

bool canRead(const string &userName, const string &fileName)
{
    SecurityLevel userLevel = userSecurity[userName];
    SecurityLevel fileLevel = fileSecurity[fileName];
    return userLevel >= fileLevel;
}

bool canWrite(const string &userName, const string &fileName)
{
    SecurityLevel userLevel = userSecurity.at(userName);
    SecurityLevel fileLevel = fileSecurity.at(fileName);
    return userLevel == fileLevel;
}

void checkAccessPermission()
{
    string userName, fileName, operation;
    cout << "Enter user name: ";
    cin >> userName;
    cout << "Enter file name to check access: ";
    cin >> fileName;
    cout << "Enter operation (read/write): ";
    cin >> operation;

    try
    {
        bool hasAccess = false;
        if (operation == "read")
        {
            hasAccess = canRead(userName, fileName);
        }
        else if (operation == "write")
        {
            hasAccess = canWrite(userName, fileName);
        }
        else
        {
            cout << RED << "Invalid operation. Please enter 'read' or 'write'." << RESET << endl;
            return;
        }

        cout << "Can " << userName << " " << operation << " " << fileName << "? ";
        if (hasAccess)
        {
            cout << GREEN << "OK" << RESET << endl;
        }
        else
        {
            cout << RED << "CHYBA OPRAVNENIA" << RESET << endl;
        }
    }
    catch (const std::out_of_range &e)
    {
        cout << RED << "Error: " << e.what() << RESET << endl;
    }
}

void createResources()
{
    string fileName;
    int level;
    cout << CYAN << "Enter file name to create: " << RESET;
    cin >> fileName;
    cout << CYAN << "Enter security level for file (1-4): " << RESET;
    cin >> level;
    createFile(fileName, securityLevels[level]);
    cout << "File '" << fileName << "' created with security level " << level << ".\n";
}

void removeUserSecurityLevel(const string &userName)
{
    auto it = userSecurity.find(userName);
    if (it != userSecurity.end())
    {
        userSecurity.erase(it);
        cout << GREEN << "Security level has been removed from user '" << userName << RESET << "'.\n";
    }
    else
    {
        cout << GREEN << "User '" << userName << "' does not exist.\n"
             << RESET;
    }
}

int main()
{
    int choice;
    do
    {
        printInstructions();
        cin >> choice;
        switch (choice)
        {
        case 1:
            createSecurityGroups();
            break;
        case 2:
            createUsers();
            break;
        case 3:
            listSecurityInformation();
            break;
        case 4:
            checkAccessPermission();
            break;
        case 5:
            createResources();
            break;
        case 6:
        {
            string userName;
            cout << "Enter the username to remove security level: ";
            cin >> userName;
            removeUserSecurityLevel(userName);
            break;
        }
        case 7:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please select a valid option.\n";
        }
    } while (choice != 7);

    return 0;
}
