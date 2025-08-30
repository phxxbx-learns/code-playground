// LIBRARY MANAGEMENT SYSTEM in Data Structures and Algorithms (BSIT2D)
// Ranon, Ma. Phoebe N.
// Tercero, Princis Diana D.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// store book info
class Book {
private:
    string title;
    string author;
    string isbn;
    bool available;

public:
    Book() {
        title = "";
        author = "";
        isbn = "";
        available = true;
    }

    Book(string bookTitle, string bookAuthor, string bookISBN) {
        title = bookTitle;
        author = bookAuthor;
        isbn = bookISBN;
        available = true;
    }
    
    // getter
    string getTitle() {
        return title;
    }

    string getAuthor() {
        return author;
    }

    string getISBN() {
        return isbn;
    }

    bool getAvailable() {
        return available;
    }
    
    // setter
    void setTitle(string bookTitle) {
        title = bookTitle;
    }

    void setAuthor(string bookAuthor) {
        author = bookAuthor;
    }

    void setISBN(string bookISBN) {
        isbn = bookISBN;
    }

    void setAvailable(bool status) {
        available = status;
    }
    
    // display book info
    void displayBook() {
        cout<<"                         Title: "<<title<<endl;
        cout<<"                         Author: "<<author<<endl;
        cout<<"                         ISBN: "<<isbn<<endl;
        cout<<"                         Status: "<<(available ? "Available" : "Borrowed")<<endl<<endl;
        
        cout<<"_______________________________________________________________________________________"<<endl<<endl;
    }
};

// store user info
class LibraryUser {
private:
    string userID;
    string name;
    vector<string> borrowedBooks; // isbn of borrowed books

public:
    LibraryUser() {
        userID = "";
        name = "";
    }

    LibraryUser(string id, string userName) {
        userID = id;
        name = userName;
    }
    
    // getter
    string getUserID() {
        return userID;
    }

    string getName() {
        return name;
    }

    vector<string> getBorrowedBooks() {
        return borrowedBooks;
    }
    
    // setter
    void setUserID(string id) {
        userID = id;
    }

    void setName(string userName) {
        name = userName;
    }
    
    // add book to borrowed list
    void borrowBook(string bookISBN) {
        borrowedBooks.push_back(bookISBN);
    }
    
    // remove book from borrowed list
    void returnBook(string bookISBN) {
        for (int i = 0; i < borrowedBooks.size(); i++) {
            if (borrowedBooks[i] == bookISBN) {
                borrowedBooks.erase(borrowedBooks.begin() + i);
                break;
            }
        }
    }
    
    // displays user info and borrowed books
    void displayUser() {
        cout<<endl<<"                               User ID: "<<userID<<endl;
        cout<<"                               Name: "<<name<<endl;
        if (borrowedBooks.empty()) {
            cout<<"                               No books borrowed."<<endl;
        } else {
            cout<<"                               Borrowed books ISBNs: ";
            for (int i = 0; i < borrowedBooks.size(); i++) {
                cout<<borrowedBooks[i]<<" ";
            }
            cout<<endl;
        }
        cout<<"_______________________________________________________________________________________"<<endl;
    }
};

// manage books and users
class Library {
private:
    vector<unique_ptr<Book>> books;
    vector<unique_ptr<LibraryUser>> users;
    string booksFile = "books.txt";
    string usersFile = "users.txt";
    
    // load books from file
    void loadBooksFromFile() {
        ifstream file(booksFile);
        if (!file.is_open()) return;

        string title, author, isbn;
        bool available;
        while (getline(file, title) && getline(file, author) && getline(file, isbn)) {
            file>>available;
            file.ignore();
            Book newBook(title, author, isbn);
            newBook.setAvailable(available);
            books.push_back(std::unique_ptr<Book>(new Book(title, author, isbn)));
            books.back()->setAvailable(available);
        }
        file.close();
    }
    
    // save books to file
    void saveBooksToFile() {
        ofstream file(booksFile);
        for (int i = 0; i < books.size(); i++) {
            file<<books[i]->getTitle()<<endl;
            file<<books[i]->getAuthor()<<endl;
            file<<books[i]->getISBN()<<endl;
            file<<books[i]->getAvailable()<<endl;
        }
        file.close();
    }

    void loadUsersFromFile() {
        ifstream file(usersFile);
        if (!file.is_open()) return;

        string userID, name, borrowedLine;
        while (getline(file, userID) && getline(file, name) && getline(file, borrowedLine)) {
            LibraryUser newUser(userID, name);
            
            // parse borrowed books
            size_t pos = 0;
            while ((pos = borrowedLine.find(',')) != string::npos) {
                string bookISBN = borrowedLine.substr(0, pos);
                if (!bookISBN.empty()) {
                    newUser.borrowBook(bookISBN);
                }
                borrowedLine.erase(0, pos + 1);
            }
            if (!borrowedLine.empty()) {
                newUser.borrowBook(borrowedLine);
            }
            users.push_back(std::unique_ptr<LibraryUser>(new LibraryUser(newUser)));
        }
        file.close();
    }

    void saveUsersToFile() {
        ofstream file(usersFile);
        for (int i = 0; i < users.size(); i++) {
            file<<users[i]->getUserID()<<endl;
            file<<users[i]->getName()<<endl;
            vector<string> borrowed = users[i]->getBorrowedBooks();
            for (int j = 0; j < borrowed.size(); j++) {
                file<<borrowed[j];
                if (j < borrowed.size() - 1) file<<",";
            }
            file<<endl;
        }
        file.close();
    }

public:
    // load data from files
    Library() {
        loadBooksFromFile();
        loadUsersFromFile();
    }
    
    // save data to files
    ~Library() {
        saveBooksToFile();
        saveUsersToFile();
    }
    
    // add new book
    void addBook(string title, string author, string isbn) {
        // checks if book already exists
        for (int i = 0; i < books.size(); i++) {
            if (books[i]->getISBN() == isbn) {
                cout<<"Error: Book with this ISBN already exists!"<<endl;
                return;
            }
        }
        books.push_back(std::unique_ptr<Book>(new Book(title, author, isbn)));
        cout<<"Book added successfully!"<<endl;
    }
    
    // remove a book
    void removeBook(string isbn) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i]->getISBN() == isbn) {
                // checks if book is borrowed
                if (!books[i]->getAvailable()) {
                    cout<<"Error: Cannot remove a borrowed book!"<<endl;
                    return;
                }
                books.erase(books.begin() + i);
                cout<<"Book removed successfully!"<<endl;
                return;
            }
        }
        cout<<"Error: Book not found!"<<endl;
    }

    void registerUser(string userID, string name) {
        // check if user already exists
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getUserID() == userID) {
                cout<<"Error: User ID already exists!"<<endl;
                return;
            }
        }
        users.push_back(std::unique_ptr<LibraryUser>(new LibraryUser(userID, name)));

        cout<<"User registered successfully!"<<endl;
    }
    
    // remove a library user
    void removeUser(string userID) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getUserID() == userID) {
                // check if user has borrowed books
                if (!users[i]->getBorrowedBooks().empty()) {
                    cout<<"Error: Cannot remove user with borrowed books!"<<endl;
                    return;
                }
                users.erase(users.begin() + i);
                cout<<"User removed successfully!"<<endl;
                return;
            }
        }
        cout<<"Error: User not found!"<<endl;
    }
    
    // handle book borrowing process
    void borrowBook(string isbn, string userID) {
        // find the book
        Book* foundBook = nullptr;
        int bookIndex = -1;
        for (int i = 0; i < books.size(); i++) {
            if (books[i]->getISBN() == isbn) {
                foundBook = books[i].get();
                bookIndex = i;
                break;
            }
        }

        if (foundBook == nullptr) {
            cout<<"Error: Book not found!"<<endl;
            return;
        }
        
        // checks if book is available
        if (!foundBook->getAvailable()) {
            cout<<"Error: Book is already borrowed!"<<endl;
            return;
        }

        // find the user
        LibraryUser* foundUser = nullptr;
        int userIndex = -1;
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getUserID() == userID) {
                foundUser = users[i].get();
                userIndex = i;
                break;
            }
        }

        if (foundUser == nullptr) {
            cout<<"Error: User not found!"<<endl;
            return;
        }

        // update book status and user's borrowed books
        books[bookIndex]->setAvailable(false);
        users[userIndex]->borrowBook(isbn);
        cout<<"Book borrowed successfully!"<<endl;
    }
    
    // handle book return process
    void returnBook(string isbn, string userID) {
        // find the book
        Book* foundBook = nullptr;
        int bookIndex = -1;
        for (int i = 0; i < books.size(); i++) {
            if (books[i]->getISBN() == isbn) {
                foundBook = books[i].get();
                bookIndex = i;
                break;
            }
        }

        if (foundBook == nullptr) {
            cout<<"Error: Book not found!"<<endl;
            return;
        }

        // find the user
        LibraryUser* foundUser = nullptr;
        int userIndex = -1;
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getUserID() == userID) {
                foundUser = users[i].get();
                userIndex = i;
                break;
            }
        }

        if (foundUser == nullptr) {
            cout<<"Error: User not found!"<<endl;
            return;
        }

        // check if user actually borrowed this book
        bool hasBook = false;
        vector<string> borrowed = users[userIndex]->getBorrowedBooks();
        for (int i = 0; i < borrowed.size(); i++) {
            if (borrowed[i] == isbn) {
                hasBook = true;
                break;
            }
        }

        if (!hasBook) {
            cout<<"Error: This user didn't borrow this book!"<<endl;
            return;
        }

        // update book status and user's borrowed books
        books[bookIndex]->setAvailable(true);
        users[userIndex]->returnBook(isbn);
        cout<<"Book returned successfully!"<<endl;
    }
    
    // displays all books in the library
    void displayAllBooks() {
        cout<<endl<<"===================================== ALL BOOKS ======================================="<<endl<<endl;
        if (books.empty()) {
            cout<<"No books in the library!"<<endl;
        } else {
            for (int i = 0; i < books.size(); i++) {
                books[i]->displayBook();
            }
        }
    }
    
    // displays all registered users
    void displayAllUsers() {
        cout<<endl<<"===================================== ALL USERS ======================================="<<endl;
        if (users.empty()) {
            cout<<"No users registered!"<<endl;
        } else {
            for (int i = 0; i < users.size(); i++) {
                users[i]->displayUser();
            }
        }
    }
    
    // displays books borrowed by a specific user
    void displayBorrowedBooks(string userID) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getUserID() == userID) {
                cout<<endl<<"============================= BORROWED BOOKS FOR USER "<<userID<<" ========================"<<endl<<endl;
                vector<string> borrowed = users[i]->getBorrowedBooks();
                if (borrowed.empty()) {
                    cout<<"No books borrowed!"<<endl;
                } else {
                    for (int j = 0; j < borrowed.size(); j++) {
                        // find and display book details
                        bool found = false;
                        for (int k = 0; k < books.size(); k++) {
                            if (books[k]->getISBN() == borrowed[j]) {
                                books[k]->displayBook();
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout<<"Book with ISBN "<<borrowed[j]<<" (not found in library)!"<<endl;
                        }
                    }
                }
                return;
            }
        }
        cout<<"Error: User not found!"<<endl;
    }
};

// main menu
void displayMenu() {
    cout<<endl;
    cout<<"======================================================================================="<<endl<<endl;

    cout<<"                           BSIT2D's LIBRARY MANAGEMENT SYSTEM                          "<<endl<<endl;

    cout<<"======================================================================================="<<endl<<endl;

    cout<<"                        Please choose from the following options:                      "<<endl<<endl;

    cout<<"                           [1] Add Book"<<endl;
    cout<<"                           [2] Remove Book"<<endl;
    cout<<"                           [3] Register User"<<endl;
    cout<<"                           [4] Remove User"<<endl;
    cout<<"                           [5] Borrow Book"<<endl;
    cout<<"                           [6] Return Book"<<endl;
    cout<<"                           [7] Display All Books"<<endl;
    cout<<"                           [8] Display All Users"<<endl;
    cout<<"                           [9] Display User's Borrowed Books"<<endl;
    cout<<"                           [10] Exit Program"<<endl<<endl;

    cout<<"======================================================================================="<<endl<<endl;

    cout<<"                           Enter your choice (1-10): ";
}

int main() {
    Library library;
    int choice;
    string title, author, isbn, userID, name;

    do {
        displayMenu();
        cin>>choice;
        cin.ignore(); // clears input buffer

        switch (choice) {
            case 1: // add book
            	cout<<endl;
                cout<<"Enter book title: ";
                getline(cin, title);
                cout<<"Enter book author: ";
                getline(cin, author);
                cout<<"Enter book ISBN: ";
                getline(cin, isbn);
                library.addBook(title, author, isbn);
                break;

            case 2: // remove book
            	cout<<endl;
                cout<<"Enter ISBN of book to remove: ";
                getline(cin, isbn);
                library.removeBook(isbn);
                break;

            case 3: // register user
            	cout<<endl;
                cout<<"Enter user ID: ";
                getline(cin, userID);
                cout<<"Enter user name: ";
                getline(cin, name);
                library.registerUser(userID, name);
                break;

            case 4: // remove user
                cout<<endl;
				cout<<"Enter user ID to remove: ";
                getline(cin, userID);
                library.removeUser(userID);
                break;

            case 5: // borrow book
                cout<<endl;
				cout<<"Enter ISBN of book to borrow: ";
                getline(cin, isbn);
                cout<<"Enter user ID: ";
                getline(cin, userID);
                library.borrowBook(isbn, userID);
                break;

            case 6: // return book
                cout<<endl;
				cout<<"Enter ISBN of book to return: ";
                getline(cin, isbn);
                cout<<"Enter user ID: ";
                getline(cin, userID);
                library.returnBook(isbn, userID);
                break;

            case 7: // display all books
            	cout<<endl;
            	cout<<"Displaying all books in the library..."<<endl;
                library.displayAllBooks();
                break;

            case 8: // display all users
                cout<<endl;
            	cout<<"Displaying all registered users..."<<endl;
				library.displayAllUsers();
                break;

            case 9: // display user's borrowed books
                cout<<endl;
				cout<<"Enter user ID: ";
                getline(cin, userID);
                library.displayBorrowedBooks(userID);
                break;

            case 10: // exit
                cout<<endl;
				cout<<"         _____________________________________________________________________         "<<endl;
				cout<<"        |                                                                     |        "<<endl;
				cout<<"        |                      Thank you for using the                        |        "<<endl;
				cout<<"        |                    Library Management System!                       |        "<<endl;
				cout<<"        |                                                                     |        "<<endl;
                cout<<"        |               Your data has been saved successfully.                |        "<<endl;
                cout<<"        |                                                                     |        "<<endl;
                cout<<"        |_____________________________________________________________________|        "<<endl;
                break;

            default:
                cout<<endl<<"Invalid choice! Please enter a number between 1 and 10."<<endl;
        }
        
        // pause
        if (choice != 10) {
            cout<<endl;
			cout<<"                              Press Enter to continue...";
            cin.get();
        }
    } while (choice != 10);

    return 0;
}

