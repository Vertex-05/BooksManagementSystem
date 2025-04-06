#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sys/stat.h>
using namespace std;
using namespace std::chrono;


time_t getFileModificationTime(const std::string& fileName) {
	struct stat fileStat;
	if (stat(fileName.c_str(), &fileStat) == 0) {
		return fileStat.st_mtime;
	}
	return -1;
}


// Invalid Input Solving
string getValidStringInput(const string& prompt, const vector<string>& validvector);
char getValidCharInput(const string& prompt, const string& validOptions);
int getValidatedIntInput(const string& prompt, int minVal, int maxVal);


// class declaration
class AuthizedUser {
private:
	string id;
	string name;
	string password;
	vector <string>usernames;
	unordered_map<string, string>credentials;
	unordered_map<string, string>IDs;
	char usertype;



	void loadsystem(const string& filename) {
		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Failed to open file: " << filename << endl;
			return;
		}
		string line, name, Password, Id;
		getline(file, line);
		while (getline(file, line)) {
			stringstream rowStream(line);
			if (getline(rowStream, Id, ',') && getline(rowStream, name, ',') && getline(rowStream, Password, ',')) {
				usernames.push_back(name);
				IDs[name] = Id;
				credentials[name] = Password;
			}
		}
		file.close();
	}

public:
	void setname(string n) { name = n; }
	void setpassword(string p) { password = p; }
	void setType(char t) { usertype = t; }

	string getID() { return id; }
	string getName() { return name; }
	string getpassword() { return password; }
	char getType() { return usertype; }


	bool login() {
		if (usertype == 'a') {
			loadsystem("adminload.csv");
		}
		else if (usertype == 'b') {
			loadsystem("userload.csv");
		}
		name = getValidStringInput("Please enter your username: ", usernames);

		int loginTimes = 0;
		id = IDs[name];
		password = credentials[name];

		while (loginTimes < 5) {
			cout << "Please enter your password\n";
			string input_password;
			cin >> input_password;

			if (input_password == password) {
				cout << "Successful login as a " << (usertype == 'a' ? "admin" : "library user") << "!\n";
				cout << "Hello, " << name << "!" << endl << endl;
				return true;
			}
			cout << "Incorrect password, please enter again.\n";
			loginTimes++;
		}

		return false;
	}

	void printinfo() {
		cout << "User ID: " << id << endl;
		cout << "User name: " << name << endl;
		cout << "User type: " << usertype << endl;
		cout << endl;
	}

};
class Book {
private:
	string ID;
	string title;
	string author;
	string category;
	int availability;

public:
	Book() = default;
	Book(string id, string t, string a, string c, int avail)
		: ID(id), title(t), author(a), category(c), availability(avail) {}

	string getID() const { return ID; }
	string getTitle() const { return title; }
	string getAuthor() const { return author; }
	string getCategory() const { return category; }
	int getAvailability() const { return availability; }

	void setID(string id) { ID = id; }
	void setTitle(string t) { title = t; }
	void setAuthor(string a) { author = a; }
	void setCategory(string c) { category = c; }
	void setAvailability(int avail) { availability = avail; }
	void updateAvailability(int quantity) {
		availability -= quantity;
	}

	void display() {
		cout << "ID: " << ID << ", Title: " << title << ", Author: " << author
			<< ", Category: " << category << ", Availability: " << availability << endl;
	}
};
class Transaction {
public:
	string receiptNumber, userID, userName, borrowDate, returnDate;
	vector<Book> borrowedBooks;

	string get_receiptNumber() { return receiptNumber; }
	string get_userID() { return userID; }
	string get_borrowDate() { return borrowDate; }
	string get_returnDate() { return returnDate; }
	Transaction(string receiptNumber, string userID, string userName, string borrowDate, string returnDate, vector<Book> borrowedBooks)
		: receiptNumber(receiptNumber), userID(userID), userName(userName), borrowDate(borrowDate), returnDate(returnDate), borrowedBooks(borrowedBooks) {}


	void display() {
		cout << "Receipt Number: " << receiptNumber << "\nUser ID: " << userID << "\nUser Name: " << userName
			<< "\nBorrow Date: " << borrowDate << "\nReturn Date: " << returnDate << "\nBooks Borrowed:\n";
		for (int i = 0; i < borrowedBooks.size(); i++) {
			cout << "  " << borrowedBooks[i].getID() << " - " << borrowedBooks[i].getTitle() << "\n";
		}
		cout << "-----------------------------\n";
	}
};


// Algorithms
// Sort by ID using merge
void merge(int left, int mid, int right, vector<Book>& books);
void mergeSort(int left, int right, vector<Book> books);
void sortID(vector<Book> books);
// Sort by ID using quick
int partition(vector<Book>& books, int low, int high);
void quickSort(vector<Book>& books, int low, int high);
// Sort by title using bubble
void sortTitle(vector<Book> books);


// Search with ID using binary
Book* searchID(string id, vector<Book>& books);
// Search with partial title using linear
Book* selectBookFromPartialMatch(const string& partialTitle, vector<Book>& books);


// Administration Functions
bool saveToCSV(const string& filename, const vector<Book>& books);
void addBooktoVector(string id, string title, string author, string category, int availability, vector<Book>& books);
void readCSV(const string& filename, vector<Book>& books);
void addBooktoCSV(string id, string title, string author, string category, int availability, vector<Book>& books);
void editBook(string id, char element, string modify, vector<Book>& books);
void deleteBook(string id, vector<Book>& books);

void displayAllTransactions(string filename);
void searchTransaction(string& filename, string& UserID);


// User Functions
void borrowBooks(vector<Book>& books, string userID, string userName, string booksFile, string transactionFile);
void displayUserTransactions(string userID, string filename);
string generateReceiptNumber();
string getDueDate();
string getCurrentDate();
void saveTransactionToCSV(Transaction transaction, string filename);
bool compareTransactions(const Transaction& a, const Transaction& b);


// Search with Partial Title using binary
void searchPartialTitleRecursive(const string& partialTitle, int left, int right, vector<Book*>& result, vector<Book>& books);
vector<Book*> searchPartialTitle(const string& partialTitle, vector<Book>& books);
Book* searchTitle(string partialTitle, vector<Book>& books);





int main() {
	cout << "Welcome to the Library Management System!\n";

	vector<Book> books;
	string booksFile = "updated_book.csv";
	string transactionFile = "transactions.csv";
	readCSV(booksFile, books);

	if (books.empty()) {
		cout << "No books loaded from file!" << endl;
		return 1;
	}

	while (true) {
		cout << "Main Control Panel:" << endl;
		char isLogin = getValidCharInput("a. admin\nb. user\nc. log out\nPlease make a selection: ", "abc");
		if (isLogin == 'c') {
			cout << "Thank you for using LMS. Goodbye!\n";
			return 0;
		}

		AuthizedUser u1;
		u1.setType(isLogin);
		Book* selectedBook = nullptr;
		

		if (u1.login()) {
			if (isLogin == 'b') {
				while (true) {

					cout << "\nChoose an option:\n";
					cout << "a. Search books by title\n";
					cout << "b. Borrow Books\n";
					cout << "c. View Transaction History\n";
					cout << "d. Exit\n";
					char choice = getValidCharInput("Enter your choice: ", "abcd");
					if (choice == 'd')
						break;
					string partialTitle;
					cin.ignore();

					switch (choice) {

					case 'a': // Search books by title (part of title) to get their ID
						cout << "Please key in the title or part of the title of the book: ";
						getline(cin, partialTitle);
						// partialTitle = "a";
						selectedBook = selectBookFromPartialMatch(partialTitle, books);
						if (selectedBook) {
							cout << "\nFound book by " << partialTitle << ": " << selectedBook->getID() << " " << selectedBook->getTitle() << " by " << selectedBook->getAuthor() << endl;
						}
						else {
							cout << "\nBook with " << partialTitle << " not found." << endl;
						}
						break;

					case 'b': // Borrow Books 
						cout << "Borrow Books" << endl;
						borrowBooks(books, u1.getID(), u1.getName(), booksFile, transactionFile);
						break;

					case 'c': // view transaction history
						cout << "Borrow Books" << endl;
						displayUserTransactions(u1.getID(), transactionFile);
						break;
					}
				}
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲区

			}
			else {
				while (true) {

					cout << "\nChoose an option:\n";
					cout << "a. Add new books\n";
					cout << "b. Edit book details\n";
					cout << "c. Delete book\n";
					cout << "d. display books\n";
					cout << "e. View transaction receipts\n";
					cout << "f. Search transaction receipts\n";
					cout << "g. Exit\n";
					char choice = getValidCharInput("Enter your choice: ", "abcdefg");
					if (choice == 'g')
						break;
					string bookID, bookName, author, category;
					int bookNum;
					char type;
					string newData;
					string userID;
					int foundIndex = -1;

					switch (choice) {
					case 'a': // Add new books
						cout << "BookID: ";
						cin >> bookID;
						cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲区

						cout << "Book Name: ";
						getline(cin, bookName);

						cout << "Author: ";
						getline(cin, author);

						cout << "Book type: ";
						cin >> category;
						cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 再次清除缓冲区
						
						cout << "Number of the book: ";
						while (!(cin >> bookNum)) {
							cin.clear(); // 清除错误状态
							cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲区
							cout << "Invalid input. Please enter a valid number: ";
						}

						addBooktoCSV(bookID, bookName, author, category, bookNum, books);
						// addBooktoCSV("BK00032", "New Book", "New Author", "book", 5, books);
						break;

					case 'b': // Edit book details 
						
						
						cout << "Book ID: ";
						cin >> bookID;

						while (true) {
							for (int i = 0; i < books.size(); i++) {
								if (books[i].getID() == bookID) {
									foundIndex = i;
									break;
								}
							}

							if (foundIndex != -1) {
								cout << "t. Set title\ta. Set author\tc. Set category\tv. Set availability\n";
								cout << "Please choose the operation type: ";
								cin >> type;
								cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲区
								cout << "Please enter the new content: ";
								getline(cin, newData);
								editBook(bookID, type, newData, books);
								cout << "Edit successfully" << endl;
								break;
								// editBook("BK00032", 't', "Updated Book", books);
							}
							else {
								cout << "No book found!\n";
								cout << "Try Again. Enter Book ID: ";
								cin >> bookID;
							}
							
						}
						break;

					case 'c': // Delete book
						cout << "Please enter the bookID you want to delete: ";
						cin >> bookID;

						while (true) {
							for (int i = 0; i < books.size(); i++) {
								if (books[i].getID() == bookID) {
									foundIndex = i;
									break;
								}
							}

							if (foundIndex != -1) {
								deleteBook(bookID, books);
								cout << "Delete successfully!" << endl;
								break;
							}
							else {
								cout << "No book found!\n";
								cout << "Try Again. Enter Book ID: ";
								cin >> bookID;
							}
							
						}
						break;

					case 'd': // display books
						sortID(books);
						sortTitle(books);

						for (auto book : books) {
							book.display();
						}
						
						break;

					case 'e': // View transaction receipts
						displayAllTransactions("transactions.csv");
						break;

					case 'f': // Search transaction receipts
						
						cout << "Please enter the userID you want to check: ";
						cin >> userID;
						searchTransaction(transactionFile, userID);
						break;
					}
				}
			}
		}


		else {
			cout << "You have exceeded the maximum number of incorrect password attempts.\n";
			return 0;
		}

	}


	return 0;
}



// Invalid Input Solving
string getValidStringInput(const string& prompt, const vector<string>& validvector) {
	string input;
	while (true) {
		cout << prompt;
		cin >> input;
		if (find(validvector.begin(), validvector.end(), input) != validvector.end()) {
			break;
		}
		cout << "Username does not exist. Please try again.\n";
	}
	return input;
}

char getValidCharInput(const string& prompt, const string& validOptions) {
	char input;
	while (true) {
		cout << prompt;
		cin >> input;
		if (validOptions.find(input) != string::npos && cin.peek() == '\n') {
			break;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please try again.\n";
	}
	return input;
}

int getValidatedIntInput(const string& prompt, int minVal, int maxVal) {
	int input;
	while (true) {
		cout << prompt;
		cin >> input;
		if (!cin.fail() && cin.peek() == '\n' && input >= minVal && input <= maxVal) {
			break;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a valid number.\n";

	}
	return input;
}


// Algorithms
// Sort by ID using merge
void merge(int left, int mid, int right, vector<Book>& books) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	vector<Book> leftBooks(n1);
	vector<Book> rightBooks(n2);

	for (int i = 0; i < n1; ++i)
		leftBooks[i] = books[left + i];
	for (int j = 0; j < n2; ++j)
		rightBooks[j] = books[mid + 1 + j];

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (leftBooks[i].getID() <= rightBooks[j].getID()) {
			books[k] = leftBooks[i];
			++i;
		}
		else {
			books[k] = rightBooks[j];
			++j;
		}
		++k;
	}

	while (i < n1) {
		books[k] = leftBooks[i];
		++i;
		++k;
	}

	while (j < n2) {
		books[k] = rightBooks[j];
		++j;
		++k;
	}
}

void mergeSort(int left, int right, vector<Book> books) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		mergeSort(left, mid, books);
		mergeSort(mid + 1, right, books);

		merge(left, mid, right, books);
	}
}

void sortID(vector<Book> books) { // Merge Sort
	if (!books.empty()) {
		mergeSort(0, books.size() - 1, books);
	}
}

// Sort by ID using quick
int partition(vector<Book>& books, int low, int high) {
	Book pivot = books[high]; // 选择最后一个元素作为基准
	int i = low - 1;

	for (int j = low; j < high; j++) {
		if (books[j].getID() < pivot.getID()) { // 按书籍 ID 比较
			i++;
			swap(books[i], books[j]);
		}
	}
	swap(books[i + 1], books[high]);
	return i + 1;
}

void quickSort(vector<Book>& books, int low, int high) {
	if (low < high) {
		int pi = partition(books, low, high); // 获取分区点
		quickSort(books, low, pi - 1);       // 排序左侧部分
		quickSort(books, pi + 1, high);      // 排序右侧部分
	}
}

// Sort by title using bubble
void sortTitle(vector<Book> books) { // Bubble Sort
	for (size_t i = 0; i < books.size(); ++i) {
		for (size_t j = 0; j < books.size() - i - 1; ++j) {
			if (books[j].getTitle() > books[j + 1].getTitle()) {
				swap(books[j], books[j + 1]);
			}
		}
	}
}


// Search with ID using binary
Book* searchID(string id, vector<Book>& books) { // Binary Search
	sortID(books);
	int left = 0;
	int right = books.size() - 1;

	while (left <= right) {
		int mid = left + (right - left) / 2;

		if (books[mid].getID() == id)
			return &books[mid];
		else if (books[mid].getID() < id)
			left = mid + 1;
		else
			right = mid - 1;
	}

	return nullptr;
}

// Search with partial title using linear
Book* selectBookFromPartialMatch(const string& partialTitle, vector<Book>& books) {
	// 查找部分匹配的书籍
	vector<Book*> result;
	for (Book& book : books) {
		if (book.getTitle().find(partialTitle) != string::npos) {
			result.push_back(&book);
		}
	}

	// 如果没有匹配的书籍
	if (result.empty()) {
		cout << "No books found containing '" << partialTitle << "' in the title." << endl;
		return nullptr;
	}

	// 显示匹配的书籍
	cout << "\nBooks containing '" << partialTitle << "' in the title:" << endl;
	for (size_t i = 0; i < result.size(); ++i) {
		cout << i + 1 << ". " << result[i]->getTitle() << " by " << result[i]->getAuthor() << endl;
	}

	// 让用户选择
	int choice;
	cout << "Enter the number of the book you want to select: ";
	cin >> choice;
	if (choice > 0 && choice <= result.size()) {
		Book* selectedBook = result[choice - 1];
		cout << "You selected: " << selectedBook->getTitle() << " by " << selectedBook->getAuthor() << endl;
		return selectedBook;
	}
	else {
		cout << "Invalid choice." << endl;
		return nullptr;
	}
}


// Administration Functions
bool saveToCSV(const string& filename, const vector<Book>& books) {
	

	// modification time before operation
	time_t beforeWriteTime = getFileModificationTime(filename);
	if (beforeWriteTime == -1) {
		std::cout << "File does not exist. It will be created." << std::endl;
	}
	else {}

	ofstream file(filename);
	file << "ID, title, author, category, availability\n";
	
	for (const auto& book : books) {
		file << book.getID() << "," << book.getTitle() << "," << book.getAuthor() << "," << book.getCategory() << "," << book.getAvailability() << "\n";
	}

	time_t afterWriteTime = getFileModificationTime(filename);
	if (afterWriteTime == beforeWriteTime) {
		cout << "Error: Failed to save changes. Modifications have been rolled back." << endl;
		return false;
	}
	else {
		return true;
	}
}

void addBooktoVector(string id, string title, string author, string category, int availability, vector<Book>& books) {
	books.push_back(Book(id, title, author, category, availability));
}

void readCSV(const string& filename, vector<Book>& books) {
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Failed to open file: " << filename << endl;
		return;
	}
	string line, id, title, author, category, availability;
	getline(file, line); // Skip header
	while (getline(file, line)) {
		stringstream ss(line);
		getline(ss, id, ',');
		getline(ss, title, ',');
		getline(ss, author, ',');
		getline(ss, category, ',');
		getline(ss, availability, ',');
		addBooktoVector(id, title, author, category, stoi(availability), books);
	}
}

void addBooktoCSV(string id, string title, string author, string category, int availability, vector<Book>& books) {
	books.push_back(Book(id, title, author, category, availability));
	if (!saveToCSV("updated_book.csv", books)){
		books.pop_back();
	}
	else {
		cout << "Add book Successfully!" << endl;
	}
}

void editBook(string id, char element, string modify, vector<Book>& books) {
	Book* book = searchID(id, books);
	if (book) {

		Book originalBook = *book;

		switch (element) {
		case 't':
			book->setTitle(modify);
			break;
		case 'a':
			book->setAuthor(modify);
			break;
		case 'c':
			book->setCategory(modify);
			break;
		case 'v':
			book->setAvailability(stoi(modify));
			break;
		}
		if (!saveToCSV("updated_book.csv", books)) {
			*book = originalBook;
			
		}
		else {
			cout << "Edit Successfully!" << endl;
		}
	}
}

void deleteBook(string id, vector<Book>& books) {
	vector<Book> backupBooks = books;
	for (auto it = books.begin(); it != books.end(); ++it) {
		if (it->getID() == id) {
			books.erase(it);
			if (!saveToCSV("updated_book.csv", books)) {
				books = backupBooks;
			}
			else {
				cout << "Delete Successfully!" << endl;
			}
			return;
		}
	}
}

void displayAllTransactions(string filename) {
	ifstream file(filename);
	vector<Transaction> transactions; // 用于存储所有记录（transaction 类对象）
	string line;

	if (file.is_open()) {
		getline(file, line); // 跳过表头

		while (getline(file, line)) {
			stringstream ss(line);
			string receiptNumber, userID, userName, borrowDate, returnDate, bookID, title;

			getline(ss, receiptNumber, ',');
			getline(ss, userID, ',');
			getline(ss, userName, ',');
			getline(ss, borrowDate, ',');
			getline(ss, returnDate, ',');
			getline(ss, bookID, ',');
			getline(ss, title, ',');

			bool found = false;
			for (int i = 0; i < transactions.size(); i++) {
				if (transactions[i].get_receiptNumber() == receiptNumber) {
					transactions[i].borrowedBooks.push_back(Book(bookID, title, "", "", 0));
					found = true;
					break;
				}
			}

			if (!found) {
				vector<Book> borrowedBooks = { Book(bookID, title, "", "", 0) };
				transactions.push_back(Transaction(receiptNumber, userID, userName, borrowDate, returnDate, borrowedBooks));
			}
		}
		file.close();

		for (int i = 0; i < transactions.size() - 1; ++i) {
			int minIndex = i;
			for (int j = i + 1; j < transactions.size(); ++j) {
				if (transactions[j].get_receiptNumber() < transactions[minIndex].get_receiptNumber()) {
					minIndex = j;
				}
			}
			if (minIndex != i) {
				Transaction temp = transactions[i];
				transactions[i] = transactions[minIndex];
				transactions[minIndex] = temp;
			}
		}


		cout << "All Transactions:\n";
		for (int i = 0; i < transactions.size(); i++) {
			transactions[i].display();
		}
	}
	else {
		cout << "Unable to open transactions file.\n";
	}
}

void searchTransaction(string& filename, string& UserID) {
	ifstream file(filename.c_str());
	vector<Transaction> transactions;
	string line;

	if (file.is_open()) {
		getline(file, line);

		while (getline(file, line)) {
			stringstream ss(line);
			string receiptNumber, userID, userName, borrowDate, returnDate, bookID, title;


			getline(ss, receiptNumber, ',');
			getline(ss, userID, ',');
			getline(ss, userName, ',');
			getline(ss, borrowDate, ',');
			getline(ss, returnDate, ',');
			getline(ss, bookID, ',');
			getline(ss, title, ',');

			vector<Book> borrowedBooks = { Book(bookID, title, "", "", 0) };

			Transaction transaction(receiptNumber, userID, userName, borrowDate, returnDate, borrowedBooks);
			transaction.borrowedBooks = borrowedBooks;
			transactions.push_back(transaction);
		}
		file.close();

		sort(transactions.begin(), transactions.end(), compareTransactions);

		int left = 0, right = transactions.size() - 1;

		bool found = false;

		while (left <= right) {
			int mid = left + (right - left) / 2;

			if (transactions[mid].userID == UserID) {
				transactions[mid].display();
				found = true;

				int i = mid - 1;
				while (i >= left && transactions[i].userID == UserID) {
					transactions[i].display();
					found = true;
					i--;
				}

				int j = mid + 1;
				while (j <= right && transactions[j].userID == UserID) {
					transactions[j].display();
					found = true;
					j++;
				}

				break;
			}
			else if (transactions[mid].userID < UserID) {
				left = mid + 1;
			}
			else {
				right = mid - 1;
			}
		}


		cout << "No transactions found for User ID: " << UserID << "\n";
	}
	else {
		cout << "Unable to open file.\n";
	}
}


// User Functions
string generateReceiptNumber() {
	srand(time(0));
	return to_string(rand() % 1000000 + 100000);
}

string getDueDate() {
	time_t now = time(0);
	tm* due = localtime(&now);

	due->tm_mday += 30;
	time_t adjusted = mktime(due); // 自动处理溢出问题
	if (adjusted == -1) {
		return "Error calculating due date";
	}

	char buffer[11];
	strftime(buffer, 11, "%Y-%m-%d", localtime(&adjusted));
	return std::string(buffer);
}

string getCurrentDate() {
	// 获取当前时间
	time_t now = time(0);

	// 转换为本地时间
	tm* localTime = localtime(&now);

	// 格式化日期为 YYYY-MM-DD
	char buffer[11];
	strftime(buffer, 11, "%Y-%m-%d", localTime);

	// 返回日期字符串
	return string(buffer);
}

void saveTransactionToCSV(Transaction transaction, string filename) {
	ofstream file(filename, ios::app); // 打开文件，使用追加模式

	if (!file.is_open()) {
		cout << "Error: Could not open the transaction file for writing!" << endl;
		return;
	}

	// 如果文件为空，写入表头
	file.seekp(0, ios::end);
	if (file.tellp() == 0) {
		file << "receiptID,userID,userName,borrow_date,return_date,bookID,title\n";
	}

	// 保存每本书的记录
	for (int i = 0; i < transaction.borrowedBooks.size(); i++) {
		Book book = transaction.borrowedBooks[i];
		file << transaction.receiptNumber << ","
			<< transaction.userID << ","
			<< transaction.userName << ","
			<< transaction.borrowDate << ","
			<< transaction.returnDate << ","
			<< book.getID() << ","
			<< book.getTitle() << "\n";
	}
	file.close();
}

void borrowBooks(vector<Book>& books, string userID, string userName, string booksFile, string transactionFile) {
	vector<Book> borrowedBooks; // 存储本次循环中借的所有书
	string bookID;
	int quantity;

	while (true) {
		
		cout << "Enter the Book ID to borrow (or type 'done' to finish): ";
		
		getline(cin, bookID);
		if (bookID == "done") break;

		// 查找书籍
		int foundIndex = -1;
		for (int i = 0; i < books.size(); i++) {
			if (books[i].getID() == bookID) {
				foundIndex = i;
				break;
			}
		}

		if (foundIndex != -1) {
			quantity = getValidatedIntInput("Enter the quantity: ", 0, 50);
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲区
			

			if (books[foundIndex].getAvailability() >= quantity) {
				books[foundIndex].updateAvailability(quantity); // 更新库存
				borrowedBooks.push_back(books[foundIndex]);  // 添加到本次借阅的集合

				string receiptNumber = generateReceiptNumber();
				string borrowDate = getCurrentDate();
				string returnDate = getDueDate();
				vector<Book> singleBookList;
				singleBookList.push_back(books[foundIndex]);
				Transaction transaction(receiptNumber, userID, userName, borrowDate, returnDate, singleBookList);
				saveTransactionToCSV(transaction, transactionFile);
				
			}

			else {
				cout << "Insufficient quantity available!\n";
			}

		}
		else {
			cout << "Book not found!\n";
		}
	}

	// 循环结束后，统一显示借书总结
	if (!borrowedBooks.empty()) {
		quickSort(borrowedBooks, 0, borrowedBooks.size() - 1);
		cout << "\nTransaction Summary for This Session:\n";
		string summaryReceipt = generateReceiptNumber();
		string summaryBorrowDate = getCurrentDate();
		string summaryReturnDate = getDueDate();
		Transaction summaryTransaction(summaryReceipt, userID, userName, summaryBorrowDate, summaryReturnDate, borrowedBooks);
		summaryTransaction.display();
		saveToCSV(booksFile, books);
	}
	else {
		cout << "No books borrowed.\n";
	}
}

void displayUserTransactions(string userID, string filename) {
	ifstream file(filename);
	string line;

	if (file.is_open()) {
		cout << "Transaction History for User: " << userID << "\n";
		getline(file, line); // 跳过表头
		while (getline(file, line)) {
			stringstream ss(line);
			string receiptNumber, storedUserID, userName, borrowDate, returnDate, bookID, bookTitle;

			// 按交易文件的存储格式解析
			getline(ss, receiptNumber, ',');
			getline(ss, storedUserID, ',');
			getline(ss, userName, ',');
			getline(ss, borrowDate, ',');
			getline(ss, returnDate, ',');
			getline(ss, bookID, ',');
			getline(ss, bookTitle, ',');

			if (storedUserID == userID) {
				cout << "Receipt Number: " << receiptNumber
					<< "\nBorrow Date: " << borrowDate
					<< "\nReturn Date: " << returnDate
					<< "\nBooks Borrowed:\n";
				cout << "  ID: " << bookID << ", Title: " << bookTitle << "\n";
				cout << "-----------------------------\n";
			}
		}
		file.close();
	}
	else {
		cout << "Unable to open transactions file.\n";
	}
}

bool compareTransactions(const Transaction& a, const Transaction& b) {
	return a.userID < b.userID;
}


// Search with Partial Title using binary
void searchPartialTitleRecursive(const string& partialTitle, int left, int right, vector<Book*>& result, vector<Book>& books) {
	if (left > right) {
		return;
	}

	int mid = left + (right - left) / 2;
	const string& midTitle = books[mid].getTitle();

	if (midTitle.find(partialTitle) != string::npos) {
		result.push_back(const_cast<Book*>(&books[mid]));
		searchPartialTitleRecursive(partialTitle, left, mid - 1, result, books);
		searchPartialTitleRecursive(partialTitle, mid + 1, right, result, books);
	}
	else if (midTitle < partialTitle) {
		searchPartialTitleRecursive(partialTitle, mid + 1, right, result, books);
	}
	else {
		searchPartialTitleRecursive(partialTitle, left, mid - 1, result, books);
	}
}

vector<Book*> searchPartialTitle(const string& partialTitle, vector<Book>& books) {
	if (books.empty()) {
		return {}; // Return an empty vector if the book list is empty
	}

	// Ensure books are sorted by title
	sortTitle(books);

	vector<Book*> result;
	searchPartialTitleRecursive(partialTitle, 0, books.size() - 1, result, books);
	return result;
}

Book* searchTitle(string partialTitle, vector<Book>& books) {
	Book* selectedBook = nullptr;
	vector<Book*> partialBooks = searchPartialTitle(partialTitle, books);
	if (!partialBooks.empty()) {
		cout << "\nBooks containing '" << partialTitle << "' in the title:" << endl;
		for (size_t i = 0; i < partialBooks.size(); ++i) {
			cout << i + 1 << ". " << partialBooks[i]->getTitle() << " by " << partialBooks[i]->getAuthor() << endl;
		}

		// Allow the user to choose a book
		int choice;
		cout << "Enter the number of the book you want to select: ";
		cin >> choice;

		// Validate user input
		if (cin.fail() || choice <= 0 || choice > static_cast<int>(partialBooks.size())) {
			cout << "Invalid choice." << endl;
		}
		else {
			selectedBook = partialBooks[choice - 1];
			cout << "You selected: " << selectedBook->getTitle() << " by " << selectedBook->getAuthor() << endl;
		}
	}
	else {
		cout << "No books found containing '" << partialTitle << "' in the title." << endl;
	}
	return selectedBook;
}
