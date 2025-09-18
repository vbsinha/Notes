#include <iostream>
#include <stdexcept>

template <typename T>
class Vector {
private:
    T* data;       // Pointer to dynamically allocated array
    std::size_t sz; // Number of elements in the vector
    std::size_t cap; // Allocated capacity

    void resize_capacity(std::size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        cap = new_capacity;
    }

public:
    // Constructor
    Vector() : data(nullptr), sz(0), cap(0) {}

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Copy constructor
    Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (std::size_t i = 0; i < sz; ++i) {
            data[i] = other.data[i];
        }
    }

    // Move constructor
    Vector(Vector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Copy assignment
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            cap = other.cap;
            data = new T[cap];
            for (std::size_t i = 0; i < sz; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move assignment
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    // Return size of the vector
    std::size_t size() const {
        return sz;
    }

    // Return capacity of the vector
    std::size_t capacity() const {
        return cap;
    }

    // Check if vector is empty
    bool empty() const {
        return sz == 0;
    }

    // Access element at index (with bounds checking)
    T& operator[](std::size_t index) {
        if (index >= sz) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= sz) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Add element to the end
    void push_back(const T& value) {
        if (sz == cap) {
            resize_capacity(cap == 0 ? 1 : cap * 2);
        }
        data[sz++] = value;
    }

    // Remove last element
    void pop_back() {
        if (sz > 0) {
            --sz;
        }
    }

    // Clear the vector
    void clear() {
        sz = 0;
    }
};

int main() {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "Vector size: " << vec.size() << "\n";
    std::cout << "Vector capacity: " << vec.capacity() << "\n";

    for (std::size_t i = 0; i < vec.size(); ++i) {
        std::cout << "vec[" << i << "] = " << vec[i] << "\n";
    }

    vec.pop_back();
    std::cout << "After pop_back, size: " << vec.size() << "\n";

    return 0;
}
