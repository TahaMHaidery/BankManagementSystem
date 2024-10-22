#include <iostream>
#include <list>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include <sstream> //lib

using namespace std;

struct Transaction
{
    string date;
    int accountNo;
    int amount;
    string type;
};

class Account
{
protected:
    static int CountAccountID;
    static int CountAccountNo;

    int AccountID;
    int AccountNo;
    string AccountName;
    string AccountType;
    string AccountStatus;
    string DOB;
    int Balance;

public:
    Account(string Name, string dob, string Type)
    {
        AccountID = CountAccountID++;
        AccountNo = CountAccountNo++;
        AccountName = Name;
        AccountType = Type;
        AccountStatus = "Active";
        DOB = dob;
        Balance = 0;

        cout << "\nNew Created Account No.: " << AccountNo << endl;
    }

    virtual void DisplayData()
    {
        cout << left << setw(12) << AccountID
             << setw(20) << AccountNo
             << setw(20) << AccountName
             << setw(15) << DOB
             << setw(10) << AccountType
             << setw(10) << AccountStatus
             << setw(10) << Balance;
    }

    int getAccountNo()
    {
        return AccountNo;
    }

    void setAccountStatus(string newStatus)
    {
        AccountStatus = newStatus;
        cout << "Account status changed to: " << AccountStatus << endl;
    }

    void saveAccount(ofstream &outFile)
    {
        outFile << AccountID << ","
                << AccountNo << ","
                << AccountName << ","
                << DOB << ","
                << AccountType << ","
                << AccountStatus << ","
                << Balance << endl;
    }

    friend Account operator+(Account acc, int amount)
    {
        acc.Balance += amount;
        cout << "\nDeposited: " << amount << ". New balance: " << acc.Balance << endl;
        return acc;
    }

    friend Account operator-(Account acc, int amount)
    {
        if (acc.Balance < amount)
        {
            cout << "\nInsufficient balance to withdraw " << amount << ". Available balance: " << acc.Balance << endl;
        }
        else
        {
            acc.Balance -= amount;
            cout << "\nWithdrew: " << amount << ". New balance: " << acc.Balance << endl;
        }
        return acc;
    }

    virtual ~Account() {}
};

class SalaryAccount : public Account
{
private:
    int MonthlySalary;
    double InterestRate;

public:
    SalaryAccount(string Name, string dob, int Salary, double Rate)
        : Account(Name, dob, "Salary"), MonthlySalary(Salary), InterestRate(Rate) {}

    void DisplayData()
    {
        Account::DisplayData();
        cout << setw(15) << MonthlySalary
             << setw(15) << InterestRate
             << endl;
    }

    void saveAccount(ofstream &outFile)
    {
        Account::saveAccount(outFile);
        outFile << MonthlySalary << ","
                << InterestRate << endl;
    }
};

class CurrentAccount : public Account
{
private:
    int TransactionLimit, MonthlyCharge;

public:
    CurrentAccount(string Name, string dob, int limit, int charge)
        : Account(Name, dob, "Current"), TransactionLimit(limit), MonthlyCharge(charge) {}

    void DisplayData()
    {
        Account::DisplayData();
        cout << setw(15) << TransactionLimit
             << setw(15) << MonthlyCharge
             << endl;
    }

    void saveAccount(ofstream &outFile)
    {
        Account::saveAccount(outFile);
        outFile << TransactionLimit << ","
                << MonthlyCharge << endl;
    }
};

class SavingsAccount : public Account
{
private:
    int MinimumBalance;
    double InterestRate;

public:
    SavingsAccount(string Name, string dob, int Balance, double Rate)
        : Account(Name, dob, "Savings"), MinimumBalance(Balance), InterestRate(Rate) {}

    void DisplayData()
    {
        Account::DisplayData();
        cout << setw(15) << MinimumBalance
             << setw(15) << InterestRate
             << endl;
    }

    void saveAccount(ofstream &outFile)
    {
        Account::saveAccount(outFile);
        outFile << MinimumBalance << ","
                << InterestRate << endl;
    }
};

int Account::CountAccountID = 1;
int Account::CountAccountNo = 10001;

template <typename AccountType, typename... Args>
AccountType *createAccount(Args... args)
{
    return new AccountType(args...);
}

template <typename AccountType>
void displayAccounts(list<Account *> accounts)
{
    bool found = false;
    for (Account *acc : accounts)
    {
        if (AccountType *typedAcc = dynamic_cast<AccountType *>(acc))
        {
            typedAcc->DisplayData();
            found = true;
        }
    }
    if (!found)
    {
        cout << "No accounts of this type available.\n";
    }
}

void printSalaryHeader()
{
    cout << "\n--- Salary Accounts ---\n";
    cout << left << setw(12) << "Account ID"
         << setw(20) << "Account No."
         << setw(20) << "Account Name"
         << setw(15) << "Date Of Birth"
         << setw(10) << "Type"
         << setw(10) << "Status"
         << setw(10) << "Balance"
         << setw(15) << "Monthly Salary"
         << setw(15) << "Interest Rate"
         << endl;
    cout << string(135, '-') << endl;
}

void printCurrentHeader()
{
    cout << "\n--- Current Accounts ---\n";
    cout << left << setw(12) << "Account ID"
         << setw(20) << "Account No."
         << setw(20) << "Account Name"
         << setw(15) << "Date Of Birth"
         << setw(10) << "Type"
         << setw(10) << "Status"
         << setw(10) << "Balance"
         << setw(15) << "Transaction Limit "
         << setw(15) << "Monthly Charge"
         << endl;
    cout << string(135, '-') << endl;
}

void printSavingsHeader()
{
    cout << "\n--- Savings Accounts ---\n";
    cout << left << setw(12) << "Account ID"
         << setw(20) << "Account No."
         << setw(20) << "Account Name"
         << setw(15) << "Date Of Birth"
         << setw(10) << "Type"
         << setw(10) << "Status"
         << setw(10) << "Balance"
         << setw(15) << "Min. Balance"
         << setw(15) << "Interest Rate"
         << endl;
    cout << string(135, '-') << endl;
}

int calculateAge(string dob)
{
    int day, month, year;
    sscanf(dob.c_str(), "%d-%d-%d", &day, &month, &year);

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    int age = currentYear - year;

    if (currentMonth < month || (currentMonth == month && currentDay < day))
    {
        age--;
    }

    return age;
}

void deleteAccount(list<Account *> &accounts, int accountNo)
{
    bool accountFound = false;

    for (auto it = accounts.begin(); it != accounts.end();)
    {
        if ((*it)->getAccountNo() == accountNo)
        {
            cout << "Deleting account with Account No: " << (*it)->getAccountNo() << endl;
            delete *it;
            it = accounts.erase(it);
            accountFound = true;
            break;
        }
        else
        {
            ++it;
        }
    }

    if (!accountFound)
    {
        cout << "No account found with Account No: " << accountNo << endl;
    }
}

void searchAccount(list<Account *> &accounts, int accountNo)
{
    list<Account *>::iterator it = accounts.begin();
    bool accountFound = false;

    while (it != accounts.end())
    {
        if ((*it)->getAccountNo() == accountNo)
        {
            if (dynamic_cast<SalaryAccount *>(*it))
            {
                printSalaryHeader();
            }
            else if (dynamic_cast<CurrentAccount *>(*it))
            {
                printCurrentHeader();
            }
            else if (dynamic_cast<SavingsAccount *>(*it))
            {
                printSavingsHeader();
            }

            (*it)->DisplayData();

            accountFound = true;
            break;
        }
        else
        {
            ++it;
        }
    }

    if (!accountFound)
    {
        cout << "No account found with Account No: " << accountNo << endl;
    }
}

void changeAccountStatus(list<Account *> &accounts, int accountNo)
{
    list<Account *>::iterator it = accounts.begin();
    bool accountFound = false;

    while (it != accounts.end())
    {
        if ((*it)->getAccountNo() == accountNo)
        {
            accountFound = true;

            string newStatus;
            cout << "Enter new account status: ";
            cin >> newStatus;
            (*it)->setAccountStatus(newStatus);

            break;
        }
        else
        {
            ++it;
        }
    }

    if (!accountFound)
    {
        cout << "No account found with Account No: " << accountNo << endl;
    }
}

void saveAllAccounts(list<Account *> &accounts)
{
    ofstream file("accounts.txt");

    if (file.is_open())
    {
        for (Account *acc : accounts)
        {
            acc->saveAccount(file);
        }
        file.close();
        cout << "All accounts saved to file successfully.\n";
    }
    else
    {
        cout << "Error saving accounts to file.\n";
    }
}

void loadAccountsFromFile(list<Account *> &accounts)
{
    ifstream file("accounts.txt");

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            int id, no, balance;
            string name, dob, type, status;

            getline(ss, line, ',');
            id = stoi(line);
            getline(ss, line, ',');
            no = stoi(line);
            getline(ss, name, ',');
            getline(ss, dob, ',');
            getline(ss, type, ',');
            getline(ss, status, ',');
            getline(ss, line, ',');
            balance = stoi(line);

            if (type == "Salary")
            {
                int salary;
                double rate;
                ss >> salary;
                ss.ignore();
                ss >> rate;

                accounts.push_back(new SalaryAccount(name, dob, salary, rate));
            }
            else if (type == "Current")
            {
                int limit, charge;
                ss >> limit;
                ss.ignore();
                ss >> charge;

                accounts.push_back(new CurrentAccount(name, dob, limit, charge));
            }
            else if (type == "Savings")
            {
                int minBalance;
                double rate;
                ss >> minBalance;
                ss.ignore();
                ss >> rate;

                accounts.push_back(new SavingsAccount(name, dob, minBalance, rate));
            }
        }
        file.close();
    }
    else
    {
        cout << "Error loading accounts from file.\n";
    }
}

void logTransaction(int accountNo, int amount, string type)
{
    string fileName = to_string(accountNo) + "_statement.txt";
    ofstream outFile(fileName, ios::app);

    static bool headerPrinted = false;
    if (!headerPrinted)
    {
        outFile << left << setw(20) << "Date"
                << setw(15) << "Account No."
                << setw(15) << "Amount"
                << setw(15) << "Type" << endl;
        outFile << string(65, '-') << endl;
        headerPrinted = true;
    }

    if (outFile.is_open())
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        char date[20];
        strftime(date, sizeof(date), "%Y-%m-%d %H:%M", ltm);

        outFile << left << setw(20) << date
                << setw(15) << accountNo
                << setw(15) << amount
                << setw(15) << type << endl;
        outFile.close();
    }
    else
    {
        cout << "Error opening transaction file." << endl;
    }
}

void withdrawMoneyAccount(list<Account *> &accounts, int accountNo, int amount)
{
    for (Account *acc : accounts)
    {
        if (acc->getAccountNo() == accountNo)
        {
            *acc = *acc - amount;
            logTransaction(accountNo, amount, "Withdrawal");
            return;
        }
    }
    cout << "No account found with Account No: " << accountNo << endl;
}

void depositMoneyAccount(list<Account *> &accounts, int accountNo, int amount)
{
    for (Account *acc : accounts)
    {
        if (acc->getAccountNo() == accountNo)
        {
            *acc = *acc + amount;
            logTransaction(accountNo, amount, "Deposit");
            return;
        }
    }
    cout << "No account found with Account No: " << accountNo << endl;
}

void printStatement(int accountNo)
{
    string fileName = to_string(accountNo) + "_statement.txt";
    ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        cout << "Error opening transaction file." << endl;
        return;
    }

    cout << "\n--- Statement for Account No: " << accountNo << " ---" << endl;

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string date, accNoStr, amountStr, type;
        getline(ss, date, ',');
        getline(ss, accNoStr, ',');
        getline(ss, amountStr, ',');
        getline(ss, type);

        cout << left << setw(20) << date
             << setw(15) << accNoStr
             << setw(15) << amountStr
             << setw(15) << type << endl;
    }

    inFile.close();
}

int main()
{
    list<Account *> accounts;
    loadAccountsFromFile(accounts);
    int flag = 0, opt, acctype;
    int minAge = 18;

    cout << "-----------------Welcome To Bank Management System-----------------" << endl;

    while (flag == 0)
    {
        cout << "\nSelect Below Options: " << endl;
        cout << "1. Create Account" << endl;
        cout << "2. All Accounts List Type-wise" << endl;
        cout << "3. Delete Account" << endl;
        cout << "4. Search Account" << endl;
        cout << "5. Withdraw Money" << endl;
        cout << "6. Deposit Money" << endl;
        cout << "7. Account Statement" << endl;
        cout << "8. Change Account Status" << endl;
        cout << "9. Exit \n"
             << endl;

        cout << "Option: ";
        cin >> opt;

        switch (opt)
        {
        case 1:
            cout << "\nSelect Below Options: " << endl;
            cout << "1. Salary Account" << endl;
            cout << "2. Current Account" << endl;
            cout << "3. Savings Account" << endl;
            cout << "\nAccount Type: ";
            cin >> acctype;

            switch (acctype)
            {
            case 1:
            {
                string name, dob;
                int salary;
                double rate;

                cout << "Enter Name: ";
                cin >> ws;
                getline(cin, name);
                cout << "Enter DOB (DD-MM-YYYY): ";
                getline(cin, dob);

                try
                {
                    int age = calculateAge(dob);
                    if (age < minAge)
                    {
                        throw age;
                    }

                    cout << "Enter Salary: ";
                    cin >> salary;
                    cout << "Enter Interest Rate: ";
                    cin >> rate;

                    accounts.push_back(createAccount<SalaryAccount>(name, dob, salary, rate));
                }
                catch (int age)
                {
                    cout << "Error: User must be at least 18 years old to create an account." << endl;
                }
                break;
            }
            case 2:
            {
                string name, dob;
                int limit, charge;

                cout << "Enter Name: ";
                cin >> ws;
                getline(cin, name);
                cout << "Enter DOB (DD-MM-YYYY): ";
                getline(cin, dob);

                try
                {
                    int age = calculateAge(dob);
                    if (age < minAge)
                    {
                        throw age;
                    }

                    cout << "Enter Transaction Limit: ";
                    cin >> limit;
                    cout << "Enter Monthly Charge: ";
                    cin >> charge;

                    accounts.push_back(createAccount<CurrentAccount>(name, dob, limit, charge));
                }
                catch (int age)
                {
                    cout << "Error: User must be at least 18 years old to create an account." << endl;
                }
                break;
            }
            case 3:
            {
                string name, dob;
                int minBalance;
                double rate;

                cout << "Enter Name: ";
                cin >> ws;
                getline(cin, name);
                cout << "Enter DOB (DD-MM-YYYY): ";
                getline(cin, dob);

                try
                {
                    int age = calculateAge(dob);
                    if (age < minAge)
                    {
                        throw age;
                    }

                    cout << "Enter Minimum Balance: ";
                    cin >> minBalance;
                    cout << "Enter Interest Rate: ";
                    cin >> rate;

                    accounts.push_back(createAccount<SavingsAccount>(name, dob, minBalance, rate));
                }
                catch (int age)
                {
                    cout << "Error: User must be at least 18 years old to create an account." << endl;
                }
                break;
            }
            }

            break;

        case 2:
            cout << "\nSelect Account Type to Display: " << endl;
            cout << "1. Salary Account" << endl;
            cout << "2. Current Account" << endl;
            cout << "3. Savings Account" << endl;
            cout << "\nAccount Type: ";
            cin >> acctype;

            switch (acctype)
            {
            case 1:
                printSalaryHeader();
                displayAccounts<SalaryAccount>(accounts);
                break;
            case 2:
                printCurrentHeader();
                displayAccounts<CurrentAccount>(accounts);
                break;
            case 3:
                printSavingsHeader();
                displayAccounts<SavingsAccount>(accounts);
                break;
            }

            break;

        case 3:
        {
            int accountNo;
            cout << "Enter the Account No. to delete: ";
            cin >> accountNo;
            deleteAccount(accounts, accountNo);
            break;
        }

        case 4:
        {
            int accountNo, amount;
            cout << "Enter the Account No. to search: ";
            cin >> accountNo;
            searchAccount(accounts, accountNo);
            break;
        }

        case 5:
        {
            int accountNo, amount;
            cout << "Enter Account No.: ";
            cin >> accountNo;
            cout << "Enter Amount: ";
            cin >> amount;
            withdrawMoneyAccount(accounts, accountNo, amount);
            break;
        }

        case 6:

            int accountNo, amount;
            cout << "Enter Account No.: ";
            cin >> accountNo;
            cout << "Enter Amount: ";
            cin >> amount;
            depositMoneyAccount(accounts, accountNo, amount);
            break;

        case 7:
        {
            int accountNo;
            cout << "Enter Account No. for statement: ";
            cin >> accountNo;
            printStatement(accountNo);
            break;
        }

        case 8:
        {
            int accountNo;
            cout << "Enter the Account No. to change status: ";
            cin >> accountNo;
            changeAccountStatus(accounts, accountNo);
            break;
        }

        case 9:
            flag = 1;
            saveAllAccounts(accounts);
            break;

        default:
            cout << "Invalid Option! Try Again." << endl;
        }
    }

    return 0;
}
