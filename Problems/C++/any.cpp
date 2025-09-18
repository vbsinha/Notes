#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <stdexcept>

class Any {
private:
    struct BaseHolder {
        virtual ~BaseHolder() = default;
        virtual std::unique_ptr<BaseHolder> clone() const = 0;
        virtual std::type_index type() const = 0;
    };

    template <typename T>
    struct Holder : BaseHolder {
        T value;

        Holder(const T& val) : value(val) {}
        Holder(T&& val) : value(std::move(val)) {}

        std::unique_ptr<BaseHolder> clone() const override {
            return std::make_unique<Holder<T>>(value);
        }

        std::type_index type() const override {
            return typeid(T);
        }
    };

    std::unique_ptr<BaseHolder> content;

public:
    Any() = default;

    template <typename T>
    Any(const T& value) : content(std::make_unique<Holder<T>>(value)) {}

    template <typename T>
    Any(T&& value) : content(std::make_unique<Holder<T>>(std::move(value))) {}

    Any(const Any& other) : content(other.content ? other.content->clone() : nullptr) {}

    Any(Any&& other) noexcept = default;

    Any& operator=(const Any& other) {
        if (this != &other) {
            content = other.content ? other.content->clone() : nullptr;
        }
        return *this;
    }

    Any& operator=(Any&& other) noexcept = default;

    void reset() {
        content.reset();
    }

    bool has_value() const {
        return static_cast<bool>(content);
    }

    std::type_index type() const {
        return has_value() ? content->type() : typeid(void);
    }

    template <typename T>
    friend T any_cast(const Any& any);
};

template <typename T>
T any_cast(const Any& any) {
    if (any.type() != typeid(T)) {
        throw std::bad_cast();
    }
    return static_cast<Any::Holder<T>*>(any.content.get())->value;
}

int main() {
    Any value = 42;  // Store an integer
    std::cout << "Stored int: " << any_cast<int>(value) << "\n";

    value = std::string("Hello, Any!");
    std::cout << "Stored string: " << any_cast<std::string>(value) << "\n";

    // Checking type at runtime
    if (value.type() == typeid(std::string)) {
        std::cout << "The stored type is std::string\n";
    }

    // Trying incorrect type cast (throws an exception)
    try {
        std::cout << any_cast<double>(value) << "\n";  // Wrong type cast
    } catch (const std::bad_cast&) {
        std::cout << "Caught bad_cast exception!\n";
    }

    value.reset();
    std::cout << "Value reset. Has value? " << std::boolalpha << value.has_value() << "\n";

    return 0;
}
