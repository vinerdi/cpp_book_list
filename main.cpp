#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

struct Date {
    int month = 0;
    int year = 0;
};

struct BOOK {
    std::string Name;
    std::string Avtor;
    Date Data;
    double Cost = 0.0;
};

void readBooksFromFile(const std::string& filename, std::vector<BOOK>& books) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return;
    }

    BOOK book;
    while (file.read(reinterpret_cast<char*>(&book), sizeof(BOOK))) {
        books.push_back(book);
    }
    file.close();
}

void writeBooksToFile(const std::string& filename, const std::vector<BOOK>& books) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return;
    }

    for (const auto& book : books) {
        file.write(reinterpret_cast<const char*>(&book), sizeof(BOOK));
    }
    file.close();
}

void printBooks(const std::vector<BOOK>& books) {
    for (const auto& book : books) {
        std::cout << "Name: " << book.Name << ", Author: " << book.Avtor
                  << ", Date: " << book.Data.month << "/" << book.Data.year
                  << ", Cost: " << book.Cost << std::endl;
    }
}

void removeBooksByYear(std::vector<BOOK>& books, int year) {
    books.erase(std::remove_if(books.begin(), books.end(), [year](const BOOK& book) {
        return book.Data.year == year;
    }), books.end());
}

void addNewBooks(std::vector<BOOK>& books, int N) {
    for (int i = 0; i < N; ++i) {
        BOOK newBook;
        std::cout << "Enter details for book " << i + 1 << ":\n";
        std::cout << "Name: ";
        std::cin >> newBook.Name;
        std::cout << "Author: ";
        std::cin >> newBook.Avtor;
        std::cout << "Month: ";
        std::cin >> newBook.Data.month;
        std::cout << "Year: ";
        std::cin >> newBook.Data.year;
        std::cout << "Cost: ";
        std::cin >> newBook.Cost;
        books.push_back(newBook);
    }
}

void readLastTenBooks(const std::string& filename, std::vector<BOOK>& books) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamoff fileSize = file.tellg();
    int bookCount = static_cast<int>(fileSize / sizeof(BOOK));
    int start = std::max(0, bookCount - 10);

    file.seekg(start * sizeof(BOOK), std::ios::beg);
    BOOK book;
    while (file.read(reinterpret_cast<char*>(&book), sizeof(BOOK))) {
        books.push_back(book);
    }
    file.close();
}

void sortBooksByAuthor(std::vector<BOOK>& books) {
    std::sort(books.begin(), books.end(), [](const BOOK& a, const BOOK& b) {
        return a.Avtor < b.Avtor;
    });
}

int main() {
    std::vector<BOOK> books;
    readBooksFromFile("SHOP.dat", books);
    printBooks(books);

    int year;
    std::cout << "Enter the year of books to remove: ";
    std::cin >> year;
    removeBooksByYear(books, year);

    int N;
    std::cout << "Enter the number of new books to add: ";
    std::cin >> N;
    addNewBooks(books, N);

    writeBooksToFile("SHOP_NEW.DAT", books);

    std::vector<BOOK> lastTenBooks;
    readLastTenBooks("SHOP_NEW.DAT", lastTenBooks);
    sortBooksByAuthor(lastTenBooks);
    writeBooksToFile("BOOK.DAT", lastTenBooks);

    return 0;
}
