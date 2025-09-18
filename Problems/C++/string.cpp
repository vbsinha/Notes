#include <iostream>
#include <cstring>  // For strlen, strcpy

class String {
private:
    char* data;   // Pointer to the character array
    std::size_t len;  // Length of the string

public:
    // Default constructor
    String() : data(new char[1]{'\0'}), len(0) {}

    // Constructor from C-string
    String(const char* str) : len(std::strlen(str)) {
        data = new char[len + 1];
        std::strcpy(data, str);
    }

    // Copy constructor (deep copy)
    String(const String& other) : len(other.len) {
        data = new char[len + 1];
        std::strcpy(data, other.data);
    }

    // Move constructor
    String(String&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            len = other.len;
            data = new char[len + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }

    // Move assignment
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            len = other.len;
            other.data = nullptr;
            other.len = 0;
        }
        return *this;
    }

    // Destructor
    ~String() {
        delete[] data;
    }

    // Get length of the string
    std::size_t size() const {
        return len;
    }

    // Access individual character
    char& operator[](std::size_t index) {
        return data[index];
    }

    const char& operator[](std::size_t index) const {
        return data[index];
    }

    // Concatenation (operator+)
    String operator+(const String& other) const {
        String result;
        result.len = len + other.len;
        result.data = new char[result.len + 1];

        std::strcpy(result.data, data);
        std::strcat(result.data, other.data);

        return result;
    }

    // Print function
    void print() const {
        std::cout << data << "\n";
    }

    // Get C-string
    const char* c_str() const {
        return data;
    }
};

int main() {
    String s1("Hello");
    String s2(" World");

    s1.print();
    s2.print();

    String s3 = s1 + s2;  // Concatenation
    s3.print();

    String s4 = s3;  // Copy constructor
    s4.print();

    return 0;
}
