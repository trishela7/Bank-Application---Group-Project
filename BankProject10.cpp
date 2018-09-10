/******************************************************************************
*** Bank Project, CS52                                                      ***
*** Group 5 - Robin Saacks, Shanyun Wen, Patricia Shatz                     ***
***                                                                         ***
*** Bank Application Program ==>>                                           ***
***   - Customers with bank accounts can be added                           ***
***   - One of 6 transaction types can be processed:                        ***
***               . Deposit                                                 ***
***               . Withdrawal                                              ***
***				  . Wire                                                    ***
***				  . Check                                                   ***
***				  . Transfer                                                ***
***				  . Balance Inquiry                                         ***
***                                                                         ***
*** Last modified: 5/16/2017                                                 ***
*******************************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
using namespace std;

const double MINIMUM_BALANCE = 100; 
const int IDlength = 5;

class BankAccount
{
public:

	BankAccount(string id, string firstName, string lastName, string city, string state, string type, double amount);
	void setDate(int yyyy, int mm, int dd);
	string addCustomer();
	void getCustomer(string customer_file, ifstream& infile, int& validCust);
	void addTransaction(string trantype, string transaction_file, ofstream& outfile, double balAmt);
	void getTransactions(string transaction_file, ifstream& infile, string tran[]);
	void getTransactionsSummary(string transaction_file, ifstream& infile);
	double getbalance(string transaction_file, ifstream& infile);

private:
	string id;
	string firstName;
	string lastName;
	string city;
	string state;
	string type; // transaction type
	double amount;
	int year;
	int month;
	int day;
};

//sort and display detail transactions retrieved by account id
void sort(string transaction[], int size);
int index_smallest(const string transaction[], int start_index, int size);
void swap_values(string& v1, string& v2);
void displayTranDet(const string transaction[], int size);

int main()
{
	int menuItem = 0 ;
	int today_day, today_month, today_year;
	int size; // number of transactions matching account id

	int validCustomerID;
	double bal;
	string customers_file_name = "Customers.csv";
	string transactions_file_name = "Transactions.csv";
	string customer;
	ifstream in_stream;
	ofstream out_stream;
	BankAccount account("", "", "", "", "", "", 0); // initialize bank account default variables
	
	typedef string* StrPtr;
	StrPtr tran; // dynamic variable to hold transactions read from a file

	//this if/else statement along with the do while statement below validate 
	//the date entered
	do 
	{// Today's date is captured at the start of the program so it can be recorded as the transaction date
		cout << " Enter today's date ==>\n";
		cout << "          Year:  "; cin >> today_year;
		cout << "          Month: "; cin >> today_month;
		cout << "          Day:   "; cin >> today_day;
		if((today_year > 2000 && today_year < 3000) && 
		(today_month >= 1 && today_month <= 12) && 
		(today_day >= 1 && today_day <= 31))
	
		{	
			account.setDate(today_year, today_month, today_day);
			cout << endl;  
		}		

		else
		{
			cout << "You entered an invalid date. Please try again! " << endl; 	
		}

	} while(!((today_year > 2000 && today_year < 3000) && 
		(today_month >= 1 && today_month <= 12) && 
		(today_day >= 1 && today_day <= 31)));


	while (menuItem != 9)
	{
		int c=0, t=0; // customer and transaction counters
		cout << "*******************************************\n" // display menu
			 << "***  Bank Application Program           ***\n"
			 << "*******************************************\n\n";
		cout << "    1. Add a customer\n"
			 << "    2. Balance inquiry - detail\n"
			 << "    3. Balance inquiry - summary\n"
			 << "    4. Check entry\n"
			 << "    5. Deposit\n"
			 << "    6. Transfer\n"
			 << "    7. Wire\n"
			 << "    8. Withdrawal\n"
			 << "    9. Exit\n\n"
			 << "    enter: ";
		cin >> menuItem; // enter menu selection

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');

		cout << endl << endl;

		switch(menuItem)
		{
			case 1: // 1. Add customer
				out_stream.open(customers_file_name, ios::app);
				if (out_stream.fail())
				{
					cout << "Failed to open file "
						 << customers_file_name;
					cin >> menuItem; // stop to read if message
					exit(1);
				}
				customer = account.addCustomer();
				// cout << "\nCustomer Record added: " << customer; // used for testing
				out_stream << customer;
				out_stream.close();
				break;
			case 2: // 2. Balance inquiry - detail
				account.getCustomer(customers_file_name, in_stream, validCustomerID);
				if (validCustomerID != 1)
					break;
				tran = new string[100];
				account.getTransactions(transactions_file_name, in_stream, tran);
				account.getTransactionsSummary(transactions_file_name, in_stream);
				getchar();
				cout << "\nPress any key to continue ...";
				getchar();
				cout << endl;
				delete [] tran; // delete dynamic array and free up memory
				break;
			case 3: // 3. Balance inquiry - summary
				account.getCustomer(customers_file_name, in_stream, validCustomerID);
				if (validCustomerID != 1)
					break;
				account.getTransactionsSummary(transactions_file_name, in_stream);
				getchar();
				cout << "\nPress any key to continue ..." << endl;
				getchar();
				cout << endl;
				break;
			case 4: // 4. Check entry
				account.getCustomer(customers_file_name, in_stream, validCustomerID);
				if (validCustomerID != 1)
					break;
				bal = account.getbalance(transactions_file_name, in_stream);
				account.addTransaction("Check", transactions_file_name, out_stream, bal);
				break;
			case 5: // 5. Deposit
				account.getCustomer(customers_file_name, in_stream, validCustomerID);
				if (validCustomerID != 1)
					break;
				bal = account.getbalance(transactions_file_name, in_stream);
				account.addTransaction("Deposit", transactions_file_name, out_stream, bal);
				break;
			case 6:  // 6. Transfer
				account.getCustomer(customers_file_name, in_stream, validCustomerID);
				if (validCustomerID != 1)
					break;
				bal = account.getbalance(transactions_file_name, in_stream);
				account.addTransaction("Transfer", transactions_file_name, out_stream, bal);
				break;
			case 7: // 7. Wire
				account.getCustomer(customers_file_name, in_stream, validCustomerID);
				if (validCustomerID != 1)
					break;
				bal = account.getbalance(transactions_file_name, in_stream);
				account.addTransaction("Wire", transactions_file_name, out_stream, bal);
				break;
			case 8:  // 8. Withdrawal
				account.getCustomer(customers_file_name, in_stream, validCustomerID);
				if (validCustomerID != 1)
					break;
				bal = account.getbalance(transactions_file_name, in_stream);
				account.addTransaction("Withdrawal", transactions_file_name, out_stream, bal);
				break;
			case 9: //9. Exit
			exit(1);
			break;	
			default:cout << "That is not a valid option. Please try again!\n\n";
				
		}
	}

	return 0;
}

void BankAccount :: setDate(int yyyy, int mm, int dd)
{
	year = yyyy;
	month = mm;
	day = dd;
}

string BankAccount :: addCustomer()
{	
	cout << " Add new customer:\n";
	do{		//Account ID = const int ID = 5 
			cout << "     Account ID: "; getline(cin, id);
			if(id.size() != IDlength) // Account ID must be 5 digits 
			{
				cout << "Please enter an Account ID that is 5 digits long:" << endl;
				getline(cin, id);
			}	
	  }while(id.size() != IDlength);
		
	cout << "     First Name: "; getline(cin, firstName);
	cout << "     Last Name:  "; getline(cin, lastName);
	cout << "     City:       "; getline(cin, city);
	cout << "     State:      "; getline(cin, state);

	return id + ',' + firstName + ',' + lastName + ',' + city + ',' + state + '\n';
}

void BankAccount :: addTransaction(string trantype, string transaction_file, ofstream& outfile, double balAmt)
{
	int z;
	double amt;
	char confirm = 'n';
	string Blank;

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	getline(cin, Blank); // clears the buffer

	while ( confirm != 'y' && confirm != 'Y' )
	{
		cout << " Enter " << trantype << " amount: ";
		cin >> amount;
		while (amount < 0)
		{
			if (amount < 0)
				cout << "The amount entered must be greater than 0" << endl;
			cout << " Enter " << trantype << " amount: ";
			cin >> amount;
		}	

		if (trantype == "Deposit")
			amt = amount;
		else
			amt = amount * -1;

		if (MINIMUM_BALANCE > balAmt + amt)
		{
			cout << "\n Your " << trantype << " of $" << amount << " would take your current balance of $" << balAmt 
			<< " to $" << balAmt + amt << " which is less than your minimum allowed balance of $" << MINIMUM_BALANCE << endl;
			cout << " The system will now abort! Please try another transaction for a lesser amount." << endl;
			getchar();
			cout << "\nPress any key to continue ...";
			getchar();
			exit(1);
		}

		cout.setf(ios::fixed);
		cout.setf(ios::showpoint);
		cout.precision(2);
		cout << "Please confirm your " << trantype << " transaction of $" << amount << " ( Y or N)";
		cin >> confirm;

	}
	outfile.open(transaction_file, ios::app); // open transaction file
	if (outfile.fail()) // validate that opened correctly
	{
		cout << "Failed to open file "
			 << transaction_file;
		cin >> z; // stop to read if message
		exit(1);
	}

	//cout << "\nTransaction Record added: " << id + ',' + trantype + ',' + to_string(amount) + ',' + to_string(year) + ',' + to_string(month) + ',' + to_string(day) + '\n';  // this line is used for testing
	outfile <<  id + ',' + trantype + ',' + to_string(amount) + ',' + to_string(year) + ',' + to_string(month) + ',' + to_string(day) + '\n';
	outfile.close();
	getchar();
	cout << "\nPress any key to continue ...";
	getchar();
	cout << endl;
}

void BankAccount :: getCustomer(string customer_file, ifstream& infile, int& validCust)
{
	int z; 
	validCust=0; // set valid customer flag = false
	string custID, customer="";

	cout << " Enter Customer ID: ";
				cin >> custID;
				cout << endl;
	infile.open(customer_file); // open customer file
	if (infile.fail()) // validate that opened correctly
	{
		cout << "Failed to open file "
			 << customer_file;
		cin >> z; // stop to read if message
		exit(1);
	}
	while ( ! infile.eof() ) // read records from cutomer file
	{
		getline(infile, customer);
		if ( custID == customer.substr(0, IDlength) ) // get customer record by id
		{
			int delim1 = customer.find(',')+1; // ',' represent end of field in the comma delimited file
			int delim2 = customer.find(',', delim1+1)+1;
			int delim3 = customer.find(',', delim2+1)+1;
			int delim4 = customer.find(',', delim3+1)+1;
			id = customer.substr(0, delim1-1);
			firstName = customer.substr(delim1, delim2-delim1-1);
			lastName = customer.substr(delim2, delim3-delim2-1);
			city = customer.substr(delim3, delim4-delim3-1);
			state = customer.substr(delim4, customer.length()-delim4);
			cout << "  |  AccountNo: " << id << "  |  "
				 << "Name: " << firstName << " " << lastName << "  |  "
				 << "Location: " << city << ", " << state << "  |" << endl;
			validCust=1; // set valid customer flag = true
		}
	}
	if (validCust != 1) // end of file, no customer id found
	{
		cout << "Customer ID not found!" << endl;
		getchar();
		cout << "\nPress any key to continue ...";
		getchar();
	}
		
	infile.close();
	cout << endl;
}

void BankAccount :: getTransactions(string transaction_file, ifstream& infile, string tran[])
{
	int size=0;
	double balance=0;
	string transaction, date;
	string yyyy, mm, dd;
	
	infile.open(transaction_file); // open customer file
	if (infile.fail()) // validate that opened correctly
	{
		cout << "Failed to open file "
			 << transaction_file;
		cin >> size; // stop to read if message
		exit(1);
	}
	while (! infile.eof()) // read records from transaction file
	{
		getline(infile, transaction);
		if ( id == transaction.substr(0, id.length()) ) // get transaction record by id
		{
			int delim1 = transaction.find(',')+1; // ',' represent end of field in the comma delimited file
			int delim2 = transaction.find(',', delim1+1)+1;
			int delim3 = transaction.find(',', delim2+1)+1;
			int delim4 = transaction.find(',', delim3+1)+1;
			int delim5 = transaction.find(',', delim4+1)+1;
			id = transaction.substr(0, delim1-1);
			type = transaction.substr(delim1, delim2-delim1-1);
			amount = stod(transaction.substr(delim2, delim3-delim2-1)); // converts string (amount) to double (amount)
			yyyy = transaction.substr(delim3, delim4-delim3-1);
			mm = transaction.substr(delim4, delim5-delim4-1);
			if (mm.length() == 1)
				mm = '0'+mm; // set to 2 character if only 1 character
			dd = transaction.substr(delim5, transaction.length()-delim5);
			if (dd.length() == 1)
				dd = '0'+dd; // set to 2 character if only 1 character
			date = yyyy+'/'+mm+'/'+dd;
			tran[size] = date+','+type+','+to_string(amount);
			size++; // number transactions matching account id
		}
	}
	infile.close();
	sort(tran, size); // sort the transactions retrieved by account id
	displayTranDet(tran,size);
}

void BankAccount :: getTransactionsSummary(string transaction_file, ifstream& infile)
{
	int z;
	double amt=0, amtDeposit=0, amtCheck=0, amtTransfer=0, amtWire=0, amtWithdrawal=0, amtTotal=0;
	string transaction;
	
	infile.open(transaction_file); // open customer file
	if (infile.fail()) // validate that opened correctly
	{
		cout << "Failed to open file "
			 << transaction_file;
		cin >> z; // stop to read if message
		exit(1);
	}
	while (! infile.eof()) // read records from transaction file
	{
		getline(infile, transaction);
		if ( id == transaction.substr(0, id.length()) ) // get transaction record by id
		{
			int delim1 = transaction.find(',')+1; // ',' represent end of field in the comma delimited file
			int delim2 = transaction.find(',', delim1+1)+1;
			int delim3 = transaction.find(',', delim2+1)+1;
			int delim4 = transaction.find(',', delim3+1)+1;
			int delim5 = transaction.find(',', delim4+1)+1;
			type = transaction.substr(delim1, delim2-delim1-1);
			amount = stod(transaction.substr(delim2, delim3-delim2-1)); // converts string (amount) to double (amount)

			if (type == "Deposit")
				amtDeposit += amount; // used to calculate deposit total
			if (type == "Check")
				amtCheck -= amount; // used to calculate check total
			if (type == "Transfer")
				amtTransfer -= amount; // used to calculate transfer total
			if (type == "Wire")
				amtWire -= amount; // used to calculate wire total
			if (type == "Withdrawal")
				amtWithdrawal -= amount; // used to calculate withdrawal total
			amtTotal = amtDeposit + amtCheck + amtTransfer + amtWire + amtWithdrawal; // balance Total
		}
	}
	infile.close();

	// Totals
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	cout.setf(ios::right);
	cout << "      Deposits:       " << setw(15) << amtDeposit << endl; //<< cout.setf(ios::right) 
	cout << "      Checks:         " << setw(15) << amtCheck << endl;
	cout << "      Transfers:      " << setw(15) << amtTransfer << endl;
	cout << "      Wires:          " << setw(15) << amtWire << endl;
	cout << "      Withdrawals:    " << setw(15) << amtWithdrawal << endl;
	cout << "      -------------   " << setw(15) << "-----------" <<endl;
	cout << "      Balance:        " << setw(15) << amtTotal << endl;
	cout << "      -------------   " << setw(15) << "-----------" <<endl;
	cout << endl;
}

double BankAccount :: getbalance(string transaction_file, ifstream& infile)
{
	int z;
	double balance=0;
	string transaction;
	
	infile.open(transaction_file); // open customer file
	if (infile.fail()) // validate that opened correctly
	{
		cout << "Failed to open file "
			 << transaction_file;
		cin >> z; // stop to read if message
		exit(1);
	}
	while (! infile.eof()) // read records from transaction file
	{
		getline(infile, transaction);
		if ( id == transaction.substr(0, id.length()) ) // get transaction record by id
		{
			int delim1 = transaction.find(',')+1; // ',' represent end of field in the comma delimited file
			int delim2 = transaction.find(',', delim1+1)+1;
			int delim3 = transaction.find(',', delim2+1)+1;
			int delim4 = transaction.find(',', delim3+1)+1;
			int delim5 = transaction.find(',', delim4+1)+1;
			id = transaction.substr(0, delim1-1);
			type = transaction.substr(delim1, delim2-delim1-1);
			amount = stod(transaction.substr(delim2, delim3-delim2-1)); // converts string (amount) to double (amount)
			if (type == "Deposit")
				balance += amount;
			else
				balance -= amount;
		}
	}
	infile.close();
	return balance;
}

void sort(string transaction[], int size)
{
	{
		int i, i_next_smallest;
		for (i = 0; i < size-1; i++)
		{
			// place the correct value in transaction[index]
			i_next_smallest = index_smallest(transaction, i, size);
			swap_values(transaction[i], transaction[i_next_smallest]);
		}
	}
}

int index_smallest(const string transaction[], int start_index, int size) 
{
	string min = transaction[start_index]; 
	int i_min = start_index;
	for (int i = start_index + 1; i < size; i++) 
		if (transaction[i] < min) 
			{
				min = transaction[i]; 
				i_min = i;
			} 
		
	return i_min;
}

void swap_values(string& v1, string& v2)
{
	string temp; 
	temp = v1;
	v1 = v2;
	v2 = temp;
}

void displayTranDet(const string transaction[], int size)
{
	double amount, debitAmt, creditAmt, balance=0;
	string date, type;

	cout << "   ------------------------------------------------------------------------------" << endl
		 << "  |  Date       |  Transaction Type  |      Debit  |      Credit  |     Balance  |" << endl
		 << "   ------------------------------------------------------------------------------" << endl;

	for (int i = 0; i < size; i++)
	{
		debitAmt=0; creditAmt=0;
		int delim1 = transaction[i].find(',')+1; // ',' represent end of field in the comma delimited file
		int delim2 = transaction[i].find(',', delim1+1)+1;
		date = transaction[i].substr(0, delim1-1);
		type = transaction[i].substr(delim1, delim2-delim1-1);
		amount = stod(transaction[i].substr(delim2, transaction[i].length()-delim2)); // converts string (amount) to double (amount)
		if (type == "Deposit")
			creditAmt = amount;
		else
			debitAmt = amount;
		balance = balance + creditAmt - debitAmt;

		cout << left;
		cout << "  | " << date << "  |  ";
		cout << setw(18) << type << "|";

		cout.setf(ios::fixed);
		cout.setf(ios::showpoint);
		cout.precision(2);
		cout << right;
		if (type == "Deposit")
		{
			creditAmt = amount;
			cout << setw(14) << " |";
			cout << setw(13) << creditAmt << " |";		
		}
		else
		{
			debitAmt = amount;
			cout << setw(12) << debitAmt << " |";
			cout << setw(15) << " |";
		}
		cout << setw(13) << balance << " |";
		cout << endl;
	}
	cout << endl;
}

BankAccount::BankAccount(string id, string firstName, string lastName, string city, string state, string type, double amount)
{
	id = "", firstName="", lastName="", city="", state="", type="";
	amount = 0;
}