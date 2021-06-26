#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>

#define MIN_BAL 500    
using namespace std;

class InsufficientFunds{};

class Account
{
    private:
        long a_num;
        string fname; 
        string lname;
        float balance;
        static long NextAccountNumber;

    public:
        Account(){}
        Account(string fname, string lname, float balance);
        long getAccNo() {return a_num;}
        string getFirstname() {return fname;}
        string getLastName() {return lname;}
        float getBalance() {return balance;}

        void Deposit(float amount);
        void Withdraw(float amount);
        static void setLastAccNumber(long a_num);
        static long getLastAccNumber();
        friend ofstream & operator<<(ofstream &ofs, Account &acc);
        friend ifstream & operator>>(ifstream &ifs, Account &acc);
        friend ostream & operator<<(ostream &os, Account &acc);

};

long Account::NextAccountNumber=0;

class Bank
{
    private:
        map<long, Account> accounts;

    public:
        Bank();
        Account OpenAccount(string fname,string lname,float balance);
        Account BalanceEnquiry(long a_num);
        Account Deposit(long a_num,float amount);
        Account Withdraw(long a_num,float amount);
        void CloseAccount(long a_num);
        void ShowAllAccounts();
        ~Bank();

};

int main(){

    Account acc;
    Bank b;

    string fname, lname; 
    long a_num;
    float balance, amount;
    int choice;

    cout << "***Banking System***\n\n";
    do
    {    
        cout << "Select one option from below\n\t";
        cout << "1 Open an account\n\t";
        cout << "2 Balance enquiry\n\t"; 
        cout << "3 Deposit\n\t"; 
        cout << "4 Withdrawl\n\t"; 
        cout << "5 Close an account\n\t"; 
        cout << "6 Show all accounts\n\t"; 
        cout << "7 Quit\n";
        cout << "Please enter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                cout << "Please Enter First Name: ";
                cin >> fname;
                cout << "Please Enter Last Name: ";
                cin >> lname;
                cout << "Please Enter Initial Balance";
                cin >> balance;
                acc = b.OpenAccount(fname, lname, balance);
                cout<<endl<<"Congratulations! Account is Created"<<endl;
                cout<<acc;
                break;
                
            case 2:
                cout << "Enter Account number: ";
                cin >> a_num;
                acc = b.BalanceEnquiry(a_num);
                cout<<endl<<"Your Account Details"<<endl;
                cout<<acc;
                break;

            case 3:
                cout << "Enter Account number: ";
                cin >> a_num;
                cout << "Enter balance to deposit: ";
                cin >> amount;
                acc=b.Deposit(a_num, amount);
                cout<<endl<<"Amount is Deposited"<<endl;
                cout<<acc; 
                break;
            
            case 4:
                cout << "Enter Account number: ";
                cin >> a_num;
                cout << "Enter balance to withdraw: ";
                cin >> amount;
                acc = b.Withdraw(a_num, amount);
                cout<<endl<<"Amount Withdrawn"<<endl;
                cout<<acc;
                break; 

            case 5:
                cout<<"Enter Account Number:";
                cin>>a_num;
                b.CloseAccount(a_num);
                cout<<endl<<"Account is Closed"<<endl;
                cout<<acc;
                break;

            case 6:
                b.ShowAllAccounts();
                break;

            case 7: break;

            default: 
                cout << "Invalid Choice";
                exit(0);

        }

    }while(choice!=7);
    return 0;
}


Account::Account(string fname, string lname, float balance)
{
    NextAccountNumber++;
    a_num=NextAccountNumber;
    fname=fname;
    lname=lname;
    this->balance=balance; 
}

void Account::Deposit(float amount)
{
    balance+=amount;
}

void Account::Withdraw(float amount)
{
    if(balance-amount < MIN_BAL)
        throw InsufficientFunds();
    balance-=amount;
}

void Account::setLastAccNumber(long account_num)
{
    NextAccountNumber=account_num;
}

long Account::getLastAccNumber()
{
    return NextAccountNumber;
}

ofstream & operator<<(ofstream &ofs,Account &acc)
{
    ofs<<acc.a_num<<endl;
    ofs<<acc.fname<<endl;
    ofs<<acc.lname<<endl;
    ofs<<acc.balance<<endl;
    return ofs;
}

ifstream & operator>>(ifstream &ifs,Account &acc)
{
    ifs>>acc.a_num;
    ifs>>acc.fname;
    ifs>>acc.lname;
    ifs>>acc.balance;
    return ifs;
}

ostream & operator<<(ostream &os,Account &acc)
{
    os<<"First Name:"<<acc.getFirstname()<<endl;
    os<<"Last Name:"<<acc.getLastName()<<endl;
    os<<"Account Number:"<<acc.getAccNo()<<endl;
    os<<"Balance:"<<acc.getBalance()<<endl;
    return os;
}


Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");

    if(!infile)
    {
        cout<<"Error in Opening! File Not Found!!"<<endl;
        return;
    }

    while(!infile.eof())
    {
        infile>>account;
        accounts.insert(pair<long,Account>(account.getAccNo(),account));
    }

    Account::setLastAccNumber(account.getAccNo());
    infile.close();
}

Account Bank::OpenAccount(string fname,string lname,float balance)
{
    ofstream outfile;
    Account account(fname,lname,balance);
    accounts.insert(pair<long,Account>(account.getAccNo(),account));
    
    outfile.open("Bank.data", ios::trunc);
    
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    return itr->second;
}

Account Bank::Deposit(long accountNumber,float amount)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::Withdraw(long accountNumber,float amount)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    cout<<"Account Deleted"<<itr->second;
    accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
    }
}

Bank::~Bank()
{ 
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
}

