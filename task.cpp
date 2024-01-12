#include <iostream>
#include <string>

const int max_member = 100;
const int max_books = 100;

class Book {
public:
    int id;
    std::string name;
    std::string author, type;
    int page_count;

    void setBookInfo(int bookId, const std::string& bookName, const std::string& bookAuthor, const std::string& bookType, int pageCount) {
        id = bookId;
        name = bookName;
        author = bookAuthor;
        type = bookType;
        page_count = pageCount;
    }
};

class Member {
public:
    std::string name;
    int n_of_books_issued;
    int id;
    int issuedBookIds[max_member];
};

class Library {
private:
    Member members[max_member];
    Book books[max_books];
    int numMembers;
    int numBooks;

public:
    Library() : numMembers(0), numBooks(0) {}

    void addMember(const std::string& memberName, int memberId) {
        if (numMembers < max_member) {
            members[numMembers].name = memberName;
            members[numMembers].id = memberId;
            members[numMembers].n_of_books_issued = 0;
            numMembers++;
            std::cout << "Member added successfully!\n";
        } else {
            std::cout << "Max members reached. You can't add any more members\n";
        }
    }

    void addBook(int bookId, const std::string& bookName, const std::string& bookAuthor, const std::string& bookType, int pageCount) {
        if (numBooks < max_books) {
            books[numBooks++].setBookInfo(bookId, bookName, bookAuthor, bookType, pageCount);
            std::cout << "Book added successfully!\n";
        } else {
            std::cout << "Max books reached. You can't add any more books\n";
        }
    }

    void issueBook() {
        int memberId;
        std::cout << "Enter member id to issue a book\n";
        std::cin >> memberId;

        if (memberId >= 1 && memberId <= numMembers) {
            int bookType;
            std::cout << "Select the type of book:\n";
            for (int i = 0; i < numBooks; i++) {
                std::cout << i + 1 << ". " << books[i].type << "\n";
            }
            std::cin >> bookType;

            if (bookType >= 1 && bookType <= numBooks) {
                int bookIndex = bookType - 1;

                int bookId = members[memberId - 1].n_of_books_issued + 1;
                members[memberId - 1].issuedBookIds[members[memberId - 1].n_of_books_issued++] = bookId;

                std::cout << "Book issued successfully!\n";
                std::cout << "User Name: " << members[memberId - 1].name << "\n";
                std::cout << "User ID: " << members[memberId - 1].id << "\n";
                std::cout << "Issued Book ID: " << bookId << "\n";
            } else {
                std::cout << "Invalid book type. Please select a valid book type.\n";
            }
        } else {
            std::cout << "Member not found. Enter a valid id\n";
        }
    }

    void returnBook() {
        int memberId;
        std::cout << "Enter member id to return a book\n";
        std::cin >> memberId;
        int bookId;
        std::cout << "Enter book id to return\n";
        std::cin >> bookId;

        if (memberId >= 1 && memberId <= numMembers) {
            for (int j = 0; j < members[memberId - 1].n_of_books_issued; ++j) {
                if (bookId == members[memberId - 1].issuedBookIds[j]) {
                    std::cout << "Book returned successfully!\n";
                    for (int k = j; k < members[memberId - 1].n_of_books_issued - 1; ++k) {
                        members[memberId - 1].issuedBookIds[k] = members[memberId - 1].issuedBookIds[k + 1];
                    }
                    members[memberId - 1].n_of_books_issued--;
                    return;
                }
            }
            std::cout << "Book not found in member's borrowed list.\n";
        } else {
            std::cout << "Member not found. Enter a valid id\n";
        }
    }
    void displayBorrowedBooks(int memberId) {
        
         if (members[memberId - 1].n_of_books_issued == 0) {
        std::cout << "No books borrowed by this member.\n";
        return;
    }else
    {
        std::cout << "Books borrowed by member with ID " << memberId << ":\n";
        for (int i = 0; i < members[memberId - 1].n_of_books_issued; ++i) {
            int bookId = members[memberId - 1].issuedBookIds[i];
            for (int j = 0; j < numBooks; ++j) {
                if (books[j].id == bookId) {
                    std::cout << "Book ID: " << books[j].id << "\tName: " << books[j].name << "\tAuthor: " << books[j].author << "\tType: " << books[j].type << "\n";
                    break;
                }
               
            }
        }
    }
    }

};

int main() {
    Library library;

    library.addBook(1, "The Time Machine", "H.G. Wells", "Science fiction", 200);
    library.addBook(2, "Animal Farm", "George Orwell", "Satire", 150);
    library.addBook(3, "Hamlet", "William Shakespeare", "Drama", 300);

    int choice;
    while (true) {
        std::cout << "\nLibrary Management System Menu:\n";
        std::cout << "1. Add Library Member\n";
        std::cout << "2. Issue Book\n";
        std::cout << "3. Return Book\n";
        std::cout << "4. Display Borrowed Books\n";
        std::cout << "5. Calculate Fine\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice (1-6): ";

        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string name;
            int id;
            std::cout << "Enter member name: ";
            std::cin >> name;
            std::cout << "Enter member id: ";
            std::cin >> id;
            library.addMember(name, id);
            break;
        }
        case 2: {
            library.issueBook();
            break;
        }
        case 3: {
            library.returnBook();
            break;
        }
        case 4: {
            int memberId;
            std::cout << "Enter member id to display borrowed books: ";
            std::cin >> memberId;
            library.displayBorrowedBooks(memberId);
            break;
        }
        case 6: {
            std::cout << "Exiting the program.\n";
            return 0;
        }
        default:
            std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
        }
    }
    return 0;
}
