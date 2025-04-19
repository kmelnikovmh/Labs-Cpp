#ifndef SHARED_PTR_
#define SHARED_PTR_

#include <atomic>
#include <stdexcept>
#include <utility>

namespace ptrs::shared {
template <typename T>
// using T = int;
class shared_ptr {
private:
    struct wrapper {
        // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes,-warnings-as-errors)
        T* m_ptr = nullptr;
        // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes,-warnings-as-errors)
        std::atomic<size_t> m_count = 0;

        explicit wrapper(T* ptr, size_t count) : m_ptr(ptr), m_count(count) {
        }
    };

    wrapper* m_wrapper_ptr = nullptr;

    void default_delete() {
        if (m_wrapper_ptr != nullptr && --(m_wrapper_ptr->m_count) == 0) {
            delete m_wrapper_ptr->m_ptr;
            delete m_wrapper_ptr;
        }
    }

public:
    shared_ptr() noexcept = default;

    // cppcheck-suppress noExplicitConstructor
    shared_ptr(std::nullptr_t) noexcept : m_wrapper_ptr(nullptr) {
    }

    explicit shared_ptr(T* ptr) noexcept
        : m_wrapper_ptr(ptr == nullptr ? nullptr : new wrapper(ptr, 1)) {
    }

    ~shared_ptr() {
        default_delete();
    }

    // copy
    shared_ptr(const shared_ptr& other) noexcept : m_wrapper_ptr(other.m_wrapper_ptr) {
        if (m_wrapper_ptr != nullptr) {
            ++(m_wrapper_ptr->m_count);
        }
    }

    shared_ptr& operator=(const shared_ptr& other) noexcept {
        if (this == &other || m_wrapper_ptr == other.m_wrapper_ptr) {
            return *this;
        }

        default_delete();
        m_wrapper_ptr = other.m_wrapper_ptr;
        if (m_wrapper_ptr != nullptr) {
            ++(m_wrapper_ptr->m_count);
        }
        return *this;
    }

    // move
    shared_ptr(shared_ptr&& other) noexcept
        : m_wrapper_ptr(std::exchange(other.m_wrapper_ptr, nullptr)) {
    }

    shared_ptr& operator=(shared_ptr&& other) {
        if (this == &other || m_wrapper_ptr == other.m_wrapper_ptr) {
            return *this;
        }

        default_delete();
        m_wrapper_ptr = std::exchange(other.m_wrapper_ptr, nullptr);
        return *this;
    }

    // methods
    [[nodiscard]] T* get() const noexcept {
        if (m_wrapper_ptr == nullptr) {
            return nullptr;
        }
        return m_wrapper_ptr->m_ptr;
    }

    void reset(T* new_ptr = nullptr) {
        default_delete();
        if (new_ptr == nullptr) {
            m_wrapper_ptr = nullptr;
        } else {
            m_wrapper_ptr = new wrapper(new_ptr, 1);
        }
    }

    // operators
    explicit operator bool() const noexcept {
        return get() != nullptr;
    }

    T& operator*() const {
        return *get();
    }

    T* operator->() const {
        return get();
    }

    bool operator==(const shared_ptr<T>& other) const noexcept {
        return m_wrapper_ptr == other.m_wrapper_ptr;
    }

    bool operator!=(const shared_ptr<T>& other) const noexcept {
        return m_wrapper_ptr != other.m_wrapper_ptr;
    }

    friend void swap(shared_ptr& lhs, shared_ptr& rhs) {
        std::swap(lhs.m_wrapper_ptr, rhs.m_wrapper_ptr);
    }
};
}  // namespace ptrs::shared
#endif
