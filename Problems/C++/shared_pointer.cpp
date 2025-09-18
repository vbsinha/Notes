#include <iostream>

template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* ref_count;

public:
    // Default constructor
    SharedPtr() : ptr(nullptr), ref_count(new int(0)) {}

    // Constructor with raw pointer
    explicit SharedPtr(T* p) : ptr(p), ref_count(new int(1)) {}

    // Copy constructor (increases reference count)
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), ref_count(other.ref_count) {
        if (ptr) {
            (*ref_count)++;
        }
    }

    // Move constructor (steals ownership)
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), ref_count(other.ref_count) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    // Copy assignment
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ptr) {
                (*ref_count)++;
            }
        }
        return *this;
    }

    // Move assignment
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    // Destructor
    ~SharedPtr() {
        release();
    }

    // Dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Arrow operator
    T* operator->() const {
        return ptr;
    }

    // Get raw pointer
    T* get() const {
        return ptr;
    }

    // Get reference count
    int use_count() const {
        return ptr ? *ref_count : 0;
    }

    // Release function to manage reference count
    void release() {
        if (ptr && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
        }
    }
};

int main() {
    SharedPtr<int> sp1(new int(42));
    std::cout << "sp1 use_count: " << sp1.use_count() << "\n";

    SharedPtr<int> sp2 = sp1; // Copy
    std::cout << "After copy, sp1 use_count: " << sp1.use_count() << "\n";
    std::cout << "After copy, sp2 use_count: " << sp2.use_count() << "\n";

    SharedPtr<int> sp3;
    sp3 = sp1; // Assignment
    std::cout << "After assignment, sp3 use_count: " << sp3.use_count() << "\n";

    {
        SharedPtr<int> sp4 = sp1;
        std::cout << "Inside scope, sp1 use_count: " << sp1.use_count() << "\n";
    } // sp4 goes out of scope

    std::cout << "After sp4 is destroyed, sp1 use_count: " << sp1.use_count() << "\n";

    return 0;
}
