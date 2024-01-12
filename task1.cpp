#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Member {
private:
    int member_id;
    string name;
  string address;
    string email;
    int* books_borrowed;
    int num_books_borrowed;
    int max_books;

public:
    Member(int id, const std::string& n, const std::string& addr, const std::string& e)
        : member_id(id), name(n), address(addr), email(e), books_borrowed(nullptr), num_books_borrowed(0), max_books(5) {}

    ~Member() {
        delete[] books_borrowed;
    }

    int getMemberId() const {
        return member_id;
    }

    const string& getName() const {
        return name;
    }

    const string& getAddress() const {
        return address;
    }

    const string& getEmail() const {
        return email;
    }

    const int* getBooksBorrowed() const {
        return books_borrowed;
    }

    int getNumBooksBorrowed() const {
        return num_books_borrowed;
    }

    void setBookBorrowed(int bookId) {
        if (num_books_borrowed < max_books) {
            int* temp = new int[num_books_borrowed + 1];
            for (int i = 0; i < num_books_borrowed; ++i) {
                temp[i] = books_borrowed[i];
            }
            temp[num_books_borrowed] = bookId;
            delete[] books_borrowed;
            books_borrowed = temp;
            ++num_books_borrowed;
        }
    }

    void returnBook(int bookId) {
        int* temp = new int[num_books_borrowed - 1];
        int index = 0;
        for (int i = 0; i < num_books_borrowed; ++i) {
            if (books_borrowed[i] != bookId) {
                temp[index++] = books_borrowed[i];
            }
        }
        delete[] books_borrowed;
        books_borrowed = temp;
        --num_books_borrowed;
    }
};

class Book {
private:
    int book_id;
    string book_name;
    string author_fname;
    string author_lname;
    bool available;
    int borrower_id;

public:
    Book(int id, const std::string& name, const std::string& fname, const std::string& lname)
        : book_id(id), book_name(name), author_fname(fname), author_lname(lname), available(true), borrower_id(-1) {}

    int getBookId() const {
        return book_id;
    }

    const string& getBookName() const {
        return book_name;
    }

    const string& getAuthorFirstName() const {
        return author_fname;
    }

    const string& getAuthorLastName() const {
        return author_lname;
    }

    bool isAvailable() const {
        return available;
    }

    int getBorrowerId() const {
        return borrower_id;
    }

    void borrowBook(int memberId) {
        available = false;
        borrower_id = memberId;
    }

    void returnBook() {
        available = true;
        borrower_id = -1;
    }
};

class Librarian {
private:
    int staff_id;
    string name;
    string address;
    string email;
    int salary;
    Member* members;
    Book* books;
    int num_members;
    int num_books;
    int max_members;
    int max_books;

public:
    Librarian(int id, const string& n, const string& addr, const string& e, int s)
        : staff_id(id), name(n), address(addr), email(e), salary(s), members(nullptr), books(nullptr),
          num_members(0), num_books(0), max_members(100), max_books(100) {}

    ~Librarian() {
        delete[] members;
        delete[] books;
    }

    void addMember(int memberId, const string& memberName, const string& memberAddress, const string& memberEmail) {
        if (num_members < max_members) {
            members[num_members] = Member(memberId, memberName, memberAddress, memberEmail);
            ++num_members;
        }
    }

    void issueBook(int memberId, int bookId) {
        Member* member = findMemberById(memberId);
        Book* book = findBookById(bookId);

        if (member && book && book->isAvailable()) {
            member->setBookBorrowed(bookId);
            book->borrowBook(memberId);
            cout << "Book issued successfully to member " << member->getName() << ".\n";
        } else {
            cout << "Invalid member ID or book ID or the book is not available. Please check and try again.\n";
        }
    }

    void returnBook(int memberId, int bookId) {
        Member* member = findMemberById(memberId);
        Book* book = findBookById(bookId);

        if (member && book && !book->isAvailable() && member->getNumBooksBorrowed() > 0) {
            member->returnBook(bookId);
            book->returnBook();
            cout << "Book returned successfully by member " << member->getName() << ".\n";
        } else {
            cout << "Invalid member ID or book ID or the member has no books borrowed. Please check and try again.\n";
        }
    }

    void displayBorrowedBooks(int memberId) {
        Member* member = findMemberById(memberId);

        if (member && member->getNumBooksBorrowed() > 0) {
            cout << "Books borrowed by member " << member->getName() << " (ID: " << member->getMemberId() << "):\n";
            for (int i = 0; i < member->getNumBooksBorrowed(); ++i) {
                Book* book = findBookById(member->getBooksBorrowed()[i]);
                if (book) {
                    cout << "Book ID: " << book->getBookId() << "\n";
                    cout << "Book Name: " << book->getBookName() << "\n";
                    cout << "Author: " << book->getAuthorFirstName() << " " << book->getAuthorLastName() << "\n";
                  
                }
            }
        } else {
            cout << "No books borrowed by this member.\n";
        }
    }

    Member* findMemberById(int memberId) {
        for (int i = 0; i < num_members; ++i) {
            if (members[i].getMemberId() == memberId) {
                return &members[i];
            }
        }
        return nullptr;
    }

    Book* findBookById(int bookId) {
        for (int i = 0; i < num_books; ++i) {
            if (books[i].getBookId() == bookId) {
                return &books[i];
            }
        }
        return nullptr;
    }

    void readBooksFromCSV(const string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << ".\n";
            return;
        }

        string line;
        while (std::getline(file, line)) {
            istringstream ss(line);
            string token;

            int bookId;
            string bookName, authorFirstName, authorLastName;

            getline(ss, token, ',');
            bookId = std::stoi(token);

            getline(ss, bookName, ',');
            getline(ss, authorFirstName, ',');
            getline(ss, authorLastName, ',');

            addBook(bookId, bookName, authorFirstName, authorLastName);
        }

        file.close();
    }

    void addBook(int bookId, const string& bookName, const string& authorFirstName, const string& authorLastName) {
        if (num_books < max_books) {
            books[num_books] = Book(bookId, bookName, authorFirstName, authorLastName);
            ++num_books;
        }
    }

    void printBookList() const {
        cout << "List of available books:\n";
        for (int i = 0; i < num_books; ++i) {
            if (books[i].isAvailable()) {
                cout << "Book ID: " << books[i].getBookId() << "\n";
                cout << "Book Name: " << books[i].getBookName() << "\n";
                cout << "Author: " << books[i].getAuthorFirstName() << " " << books[i].getAuthorLastName() << "\n";
            
            }
        }
    }

    void printMemberList() const {
        cout << "List of library members:\n";
        for (int i = 0; i < num_members; ++i) {
            cout << "Member ID: " << members[i].getMemberId() << "\n";
            cout << "Name: " << members[i].getName() << "\n";
            cout << "Address: " << members[i].getAddress() << "\n";
            cout << "Email: " << members[i].getEmail() << "\n";
            
        }
    }
};

int main() {
    Librarian librarian(1, "ubaid", "123 Library ", "ubaid.@email.com", 9000);

    
    librarian.readBooksFromCSV("library_books.csv");

    =
    librarian.printBookList();
    librarian.printMemberList();

    librarian.issueBook(101, 1); 
    librarian.issueBook(102, 2); 
    librarian.displayBorrowedBooks(121); 
    librarian.displayBorrowedBooks(112); 

    librarian.returnBook(121, 1); 
    librarian.returnBook(101, 2); 

    librarian.displayBorrowedBooks(121);
    librarian.displayBorrowedBooks(102); 

    return 0;
}
