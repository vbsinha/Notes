#include <iostream>
#include <cstring>  // For strlen, strcpy, strcat

class String {
private:
    static constexpr std::size_t SSO_THRESHOLD = 15; // Strings ≤ 15 characters use SSO

    union {
        struct {
            char* heap_data;  // Pointer for dynamically allocated memory
            std::size_t len;  // Length of heap-allocated string
        };
        char sso_buffer[SSO_THRESHOLD + 1]; // Inline buffer for short strings
    };

    bool is_sso() const {
        return sso_buffer[SSO_THRESHOLD] == '\0'; // If last byte is null, SSO is active
    }

    void enable_sso(const char* str, std::size_t length) {
        std::strcpy(sso_buffer, str);
        sso_buffer[length] = '\0';
        sso_buffer[SSO_THRESHOLD] = '\0'; // SSO indicator
    }

    void allocate_heap(const char* str, std::size_t length) {
        heap_data = new char[length + 1];
        std::strcpy(heap_data, str);
        len = length;
    }

public:
    // Default constructor
    String() {
        sso_buffer[0] = '\0';
        sso_buffer[SSO_THRESHOLD] = '\0'; // Enable SSO
    }

    // Constructor from C-string
    String(const char* str) {
        std::size_t length = std::strlen(str);
        if (length <= SSO_THRESHOLD) {
            enable_sso(str, length);
        } else {
            allocate_heap(str, length);
        }
    }

    // Copy constructor
    String(const String& other) {
        if (other.is_sso()) {
            std::strcpy(sso_buffer, other.sso_buffer);
            sso_buffer[SSO_THRESHOLD] = '\0';
        } else {
            allocate_heap(other.heap_data, other.len);
        }
    }

    // Move constructor
    String(String&& other) noexcept {
        if (other.is_sso()) {
            std::strcpy(sso_buffer, other.sso_buffer);
            sso_buffer[SSO_THRESHOLD] = '\0';
        } else {
            heap_data = other.heap_data;
            len = other.len;
            other.heap_data = nullptr;
            other.len = 0;
        }
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {
            if (!is_sso()) delete[] heap_data;
            if (other.is_sso()) {
                std::strcpy(sso_buffer, other.sso_buffer);
                sso_buffer[SSO_THRESHOLD] = '\0';
            } else {
                allocate_heap(other.heap_data, other.len);
            }
        }
        return *this;
    }

    // Move assignment
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            if (!is_sso()) delete[] heap_data;
            if (other.is_sso()) {
                std::strcpy(sso_buffer, other.sso_buffer);
                sso_buffer[SSO_THRESHOLD] = '\0';
            } else {
                heap_data = other.heap_data;
                len = other.len;
                other.heap_data = nullptr;
                other.len = 0;
            }
        }
        return *this;
    }

    // Destructor
    ~String() {
        if (!is_sso()) {
            delete[] heap_data;
        }
    }

    // Get string length
    std::size_t size() const {
        return is_sso() ? std::strlen(sso_buffer) : len;
    }

    // Access character by index
    char& operator[](std::size_t index) {
        return is_sso() ? sso_buffer[index] : heap_data[index];
    }

    const char& operator[](std::size_t index) const {
        return is_sso() ? sso_buffer[index] : heap_data[index];
    }

    // Concatenation
    String operator+(const String& other) const {
        std::size_t new_length = size() + other.size();
        String result;

        if (new_length <= SSO_THRESHOLD) {
            std::strcpy(result.sso_buffer, c_str());
            std::strcat(result.sso_buffer, other.c_str());
            result.sso_buffer[SSO_THRESHOLD] = '\0';
        } else {
            result.allocate_heap(c_str(), size());
            std::strcat(result.heap_data, other.c_str());
        }
        return result;
    }

    // Get C-string
    const char* c_str() const {
        return is_sso() ? sso_buffer : heap_data;
    }

    // Print function for debugging
    void print() const {
        std::cout << c_str() << " (size: " << size() << ", " 
                  << (is_sso() ? "SSO" : "Heap") << ")\n";
    }
};

int main() {
    String s1("Short");
    String s2("This is a long string that exceeds SSO");

    s1.print();  // Should use SSO
    s2.print();  // Should use heap

    String s3 = s1 + " fit!";
    s3.print();  // Should use SSO if short

    String s4 = s1 + s2;
    s4.print();  // Should use heap

    return 0;
}
