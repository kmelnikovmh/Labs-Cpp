#ifndef UNIQUE_PTR_
#define UNIQUE_PTR_

#include <stdexcept>
#include <utility>

namespace ptrs::unique {
struct dereferencing_nullptr : std::runtime_error {
    dereferencing_nullptr() : std::runtime_error("Dereferencing nullptr") {
    }
};

template <typename T>
struct default_delete {
    void operator()(T *m_ptr) const {
        delete m_ptr;
    }
};

template <typename T, typename Deleter = default_delete<T>>
class unique_ptr {
private:
    T *m_ptr = nullptr;
    Deleter m_deleter;

public:
    unique_ptr() noexcept = default;

    // cppcheck-suppress noExplicitConstructor
    unique_ptr(std::nullptr_t) noexcept {
    }

    explicit unique_ptr(T *ptr) noexcept : m_ptr(ptr) {
    }

    template <typename custom_deleter = Deleter>
    explicit unique_ptr(T *ptr, custom_deleter &&deleter)
        : m_ptr(ptr), m_deleter(std::forward<custom_deleter>(deleter)) {
    }

    ~unique_ptr() {
        reset();
    }

    unique_ptr(const unique_ptr &other) = delete;
    unique_ptr &operator=(const unique_ptr &other) = delete;

    unique_ptr(unique_ptr &&other)
        : m_ptr(std::exchange(other.m_ptr, nullptr)),
          m_deleter(std::move(other.m_deleter)) {
    }

    unique_ptr &operator=(unique_ptr &&other) {
        if (this == &other) {
            return *this;
        }
        reset();
        m_ptr = std::exchange(other.m_ptr, nullptr);
        m_deleter = std::move(other.m_deleter);
        return *this;
    }

    [[nodiscard]] T *get() const noexcept {
        return m_ptr;
    }

    [[nodiscard]] T *release() {
        return std::exchange(m_ptr, nullptr);
    }

    void reset(T *new_ptr = nullptr) {
        if (new_ptr == m_ptr) {
            return;
        } else if (m_ptr != nullptr) {
            m_deleter(m_ptr);
        }
        m_ptr = new_ptr;
    }

    [[nodiscard]] explicit operator bool() const noexcept {
        return m_ptr != nullptr;
    }

    [[nodiscard]] T &operator*() const {
        if (m_ptr == nullptr) {
            throw dereferencing_nullptr();
        }
        return *m_ptr;
    }

    [[nodiscard]] T *operator->() const {
        if (m_ptr == nullptr) {
            throw dereferencing_nullptr();
        }
        return m_ptr;
    }

    bool operator==(const unique_ptr<T> &other) const noexcept {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const unique_ptr<T> &other) const noexcept {
        return m_ptr != other.m_ptr;
    }

    friend void swap(unique_ptr &lhs, unique_ptr &rhs) {
        std::swap(lhs.m_ptr, rhs.m_ptr);
        std::swap(lhs.m_deleter, rhs.m_deleter);
    };
};
}  // namespace ptrs::unique
#endif
