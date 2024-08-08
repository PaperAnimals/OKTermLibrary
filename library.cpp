#include <iostream>
#include <string>
#include <map>

class Book {
    private:
        std::string title;
        std::string author;
        std::string ISBN;
        bool available;

    public:
        Book(std::string title, std::string author, std::string ISBN, bool available) : 
        title(title), author(author), ISBN(ISBN), available(available) {}

        virtual void display_info() {
            std::cout << "Your book with ISBN " << ISBN << " is '" << title << "' by " << author << ". ";
        }

        std::string get_title() {
            return title;
        }

        std::string get_author() {
            return author;
        }

        std::string get_isbn() {
            return ISBN;
        }

        std::string get_available() {
            if (available) {
                return "available";
            } else {
                return "unavailable";
            }
        }

        void alter_availability() {
            available = !available;
        }

};

class NonFictionBook : public Book {
    private:
        std::string bookClass = "non-fiction";
        std::string subject;
        std::string type;
        std::string level;

    public:
        NonFictionBook(std::string title, std::string author, std::string ISBN, bool available, std::string subject, std::string type, std::string level) :
        Book(title, author, ISBN, available), subject(subject), type(type), level(level) {}

        std::string get_subject() {
            return subject;
        }
        std::string get_type() {
            return type;
        }
        std::string get_level() {
            return level;
        }

        void display_info() override {
            Book::display_info();
            std::cout << "The book is " << bookClass << ", it is " << type << " on the subject of " << subject << " recommended at level " << level << ". It is currently " << get_available() << "." << std::endl;
        } 

};

class FictionBook : public Book {
    private:
        std::string bookClass = "fiction";
        std::string genre;
        std::string recommendedAge;
        std::string pageCount;

    public:
        FictionBook(std::string title, std::string author, std::string ISBN, bool available, std::string genre, std::string recommendedAge, std::string pageCount) :
        Book(title, author, ISBN, available), genre(genre), recommendedAge(recommendedAge), pageCount(pageCount) {}

        std::string get_genre() {
            return genre;
        }
        std::string get_age() {
            return recommendedAge;
        }
        std::string get_page_count() {
            return pageCount;
        }

        void display_info() override {
            Book::display_info();
            std::cout << "This book is " << bookClass << ", it is in the " << genre << " genre, recommended from age " << recommendedAge << ". It is " << pageCount << " pages long and currently " << get_available() << "." << std::endl;
        }
};

class Library {
    private:
        struct LibraryNode {
            std::string data;
            std::map<std::string, LibraryNode*> children;
            Book* book;

            LibraryNode(const std::string& data) :
            data(data), book(nullptr) {}

        bool has_children() const {
            return !children.empty();
        }
        };

        LibraryNode* libraryRoot;

        LibraryNode* look_up_book_helper(LibraryNode* node, const std::string& searchTerm) {
            if (node==nullptr) {
                return nullptr;
            }
            if (node->data == searchTerm) {
                return node;
            }

            for (auto& child : node->children) {
                LibraryNode* foundBook = look_up_book_helper(child.second, searchTerm);
                if (foundBook != nullptr) {
                    if (foundBook->has_children()) {
                        return foundBook->children.begin()->second;
                    } else {
                        return foundBook;
                    }
                }
            }
            return nullptr;
        }

        std::string retrieve_search_term() {
            std::string searchTerm;
            std::cout << "Please enter the title or ISBN of the book: ";
            std::getline(std::cin, searchTerm);
            return searchTerm;
        }

    public:
        Library() {
            libraryRoot = new LibraryNode("Library");
            libraryRoot->children["non-fiction"] = new LibraryNode("non-fiction");
            libraryRoot->children["fiction"] = new LibraryNode("fiction");
        }

        ~Library() {
            delete libraryRoot;
        }

        LibraryNode* create_node(LibraryNode* parent, const std::string& data) {
            parent->children[data] = new LibraryNode(data);
            return parent->children[data];
        }

        LibraryNode* find_or_create_node(LibraryNode* parent, const std::string& data) {
            auto it = parent->children.find(data);
            if (it != parent->children.end()) {
                return it->second;
            } else {
                LibraryNode* newNode = create_node(parent, data);
                return newNode;
            }
        }

        void add_nonfict_book(NonFictionBook* book) {
            LibraryNode* subjectNode = find_or_create_node(libraryRoot->children["non-fiction"], book->get_subject());
            LibraryNode* typeNode = find_or_create_node(subjectNode, book->get_type());
            LibraryNode* levelNode = find_or_create_node(typeNode, book->get_level());
            LibraryNode* authorNode = find_or_create_node(levelNode, book->get_author());
            if (authorNode->children.find(book->get_isbn()) != authorNode->children.end()) {
                std::cout << "Book already in collection" << std::endl;
            }
            else {
                LibraryNode* isbnNode = find_or_create_node(authorNode, book->get_isbn());
                LibraryNode* titleNode = find_or_create_node(isbnNode, book->get_title());
                titleNode->book = book;
                std::cout << "Book added to library" << std::endl;
            }
        }

        void input_nonfict_info() {
            std::string title, author, ISBN, subject, type, level;
            std::cout << "Title: " << std::endl;
            std::getline(std::cin, title);
            std::cout << "Author: " << std::endl;
            std::getline(std::cin, author);
            std::cout << "ISBN: " << std::endl;
            std::getline(std::cin, ISBN);
            std::cout << "Subject: " << std::endl;
            std::getline(std::cin, subject);
            std::cout << "Type: " << std::endl;
            std::getline(std::cin, type);
            std::cout << "Level: " << std::endl;
            std::getline(std::cin, level);

            NonFictionBook* book = new NonFictionBook(title, author, ISBN, true, subject, type, level);
            add_nonfict_book(book);
        }

        void add_fict_book(FictionBook* book) {
            LibraryNode* genreNode = find_or_create_node(libraryRoot->children["fiction"], book->get_genre());
            LibraryNode* ageNode = find_or_create_node(genreNode, book->get_age());
            LibraryNode* pageNode = find_or_create_node(ageNode, book->get_page_count());
            LibraryNode* authorNode = find_or_create_node(pageNode, book->get_author());
            if (authorNode->children.find(book->get_isbn()) != authorNode->children.end()) {
                std::cout << "Book already in collection" << std::endl;
            }
            else {
                LibraryNode* isbnNode = find_or_create_node(authorNode, book->get_isbn());
                LibraryNode* titleNode = find_or_create_node(isbnNode, book->get_title());
                titleNode->book = book;
                std::cout << "Book added to library" << std::endl;
            }
        }
        void input_fict_info() {
            std::string title, author, ISBN, genre, age, pages;
            std::cout << "Title: " << std::endl;
            std::getline(std::cin, title);
            std::cout << "Author: " << std::endl;
            std::getline(std::cin, author);
            std::cout << "ISBN: " << std::endl;
            std::getline(std::cin, ISBN);
            std::cout << "Genre: " << std::endl;
            std::getline(std::cin, genre);
            std::cout << "Recommended Age: " << std::endl;
            std::getline(std::cin, age);
            std::cout << "Page count to nearest hundred: " << std::endl;
            std::getline(std::cin, pages);

            FictionBook* book = new FictionBook(title, author, ISBN, true, genre, age, pages);
            add_fict_book(book);
        }

        void add_book() {
            std::string bookClass;
            std::cout << "Please enter the book details: " << std::endl;
            std::cout << "Is the book fiction or non-fiction? ";
            std::getline(std::cin, bookClass);
            if (bookClass=="non-fiction") {
                input_nonfict_info();
            }
            if (bookClass=="fiction") {
                input_fict_info();
            }
        }

        void look_up_book() {
            std::string searchTerm = retrieve_search_term();
            LibraryNode* foundBook = look_up_book_helper(libraryRoot, searchTerm);
            if (foundBook == nullptr) {
                std::string newInput;
                std::cout << "Book not in library, would you like to add book (Y) or retry search (N)? Enter E to return to menu." << std::endl;
                std::getline(std::cin, newInput);
                while (true) {
                    if (newInput == "Y") {
                        add_book();
                        break;
                    } else if (newInput == "N") {
                        look_up_book();
                        break;
                    } else if (newInput == "E") {
                        return;
                    } else {
                        std::cout << "Please enter a valid input" << std::endl;
                        std::getline(std::cin, newInput);
                    }
                }
            }

            foundBook->book->display_info();
        }

        LibraryNode* find_parent(LibraryNode* node, LibraryNode* target) {
            if (node==nullptr || node==target) {
                return nullptr;
            }
            for (auto& child : node->children) {
                if (child.second == target) {
                    return node;
                }
                LibraryNode* parent = find_parent(child.second, target);
                if (parent != nullptr) {
                    return parent;
                }
            }
            return nullptr;
        }

        void remove_book() {
            std::string searchTerm = retrieve_search_term();
            LibraryNode* foundBook = look_up_book_helper(libraryRoot, searchTerm);
            if (foundBook == nullptr) {
                std::cout << "Book not found in library" << std::endl;
                return;
            }
            delete foundBook->book;
            foundBook->book = nullptr;

            LibraryNode* parent = find_parent(libraryRoot, foundBook);
            if (parent != nullptr) {
                parent->children.erase(foundBook->data);
                delete foundBook;
            }

            std::cout << "Book removed from library." << std::endl;
        }

        void borrow_return_book() {
            std::string searchTerm = retrieve_search_term();
            LibraryNode* foundBook = look_up_book_helper(libraryRoot, searchTerm);
            Book* book = foundBook->book;
            if (foundBook == nullptr) {
                std::cout << "Book not found in library" << std::endl;
                return;
            }
            std::string borrowOrReturn;
            std::string newInput;
            if (book->get_available()=="available") {
                borrowOrReturn = "borrow";
            } else {
                borrowOrReturn = "return";
            }
            std::cout << "Book is currently " << book->get_available() << std::endl;
            std::cout << "Would you like to " << borrowOrReturn << " '" << book->get_title() << "'? (Y/N)" << std::endl;
            std::getline(std::cin, newInput);
            if (newInput == "Y") {
                book->alter_availability();
                std::cout << "Book is no longer available" << std::endl;
            } else {
                std::cout << "Book is now available" << std::endl;
            }
        }
};

class LibraryUI {
    private:
        Library& library;

    public:
        LibraryUI(Library& lib) : library(lib) {}

        void run() {
            int choice;
            bool online = true;

            while (online) {
                choice = display_menu();
                switch (choice) {
                    case 1:
                        library.add_book();
                        break;
                    case 2:
                        library.look_up_book();
                        break;
                    case 3:
                        library.remove_book();
                        break;
                    case 4:
                    case 5:
                        library.borrow_return_book();
                        break;
                    case 0:
                        online = false;
                        break;
                    default:
                        std::cout << "Please try again." << std::endl;
                }
            }

        }

        int display_menu() {
            std::string choiceStr;
            int choice;

            std::cout << "\nLibrary Menu" << std::endl;
            std::cout << "1. Add a book" << std::endl;
            std::cout << "2. Look-up a book" << std::endl;
            std::cout << "3. Remove a book" << std::endl;
            std::cout << "4. Borrow a book" << std::endl;
            std::cout << "5. Return a book" << std::endl;
            std::cout << "0. Quit" << std::endl;
            std::cout << "\nPlease enter the number of the option." << std::endl;

            std::getline(std::cin, choiceStr);
            choice = std::stoi(choiceStr);
            return choice;
        }
};

int main() {
    Library library;
    LibraryUI libUI = LibraryUI(library);
    libUI.run();
    return 0;
}