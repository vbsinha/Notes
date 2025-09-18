#include<iostream>

struct foo {
    int a;
    bool b;
};

template<typename T>
class unique_pointer {
public:
    constexpr unique_pointer() : ptr{nullptr} {}

    unique_pointer(T* _ptr) : ptr{_ptr} {}

    // copy constructor
    unique_pointer(const unique_pointer<T> &other) = delete;

    // move constructor
    unique_pointer(unique_pointer<T> &&other) : ptr{other.ptr} {
        other.ptr = nullptr;
    }

    // assignment
    unique_pointer& operator= (const T* other) noexcept {
        delete ptr;
        ptr = other;
        return *this;
    }

    // copy assignment 
    void operator= (const unique_pointer<T> &other) = delete;

    // move assignment 
    unique_pointer& operator= (unique_pointer<T> &&other) {
        if (ptr) delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    ~unique_pointer() {
        delete ptr;
    }

    T* get() const noexcept {
        return ptr;
    }

    T& operator* () const noexcept { 
        return *ptr; 
    }

    T* operator-> () const noexcept { 
        return ptr; 
    }

    explicit operator bool () const noexcept { 
        return ptr != nullptr; 
    }

private:
    T* ptr;
};

template<typename T, typename U,
    typename = std::enable_if_t<std::is_convertible_v<T*, U*> || std::is_convertible_v<U*, T*> > >
bool operator== (const unique_pointer<T>& first, const unique_pointer<U>& second) noexcept {
    return first.get() == second.get();
}

int main() {
    unique_pointer<foo> u(new foo{5, true});

    foo u_copy = *u;
    u->a = 10;
    int ua = u->a;
    bool ub = u->b;

    std::cout << u_copy.a << " " << u_copy.b << std::endl;
    std::cout << ua << " " << ub << std::endl;

    unique_pointer<foo> u_array = new foo[10];
    u_array[0].a = 
}