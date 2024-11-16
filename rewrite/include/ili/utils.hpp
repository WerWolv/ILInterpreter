#pragma once

#include <coroutine>
#include <exception>

namespace ili::util {

    template<typename T>
    T alignUp(T value, T alignment) {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    template<typename T>
    class Generator {
    public:
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type {
            std::optional<T> current_value;

            Generator get_return_object() {
                return Generator{handle_type::from_promise(*this)};
            }

            std::suspend_always initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }

            std::suspend_always yield_value(T value) noexcept {
                current_value = value;
                return {};
            }

            void return_void() noexcept {}
            void unhandled_exception() { std::terminate(); }
        };

        explicit Generator(handle_type h) : m_coroutineHandle(h) {}
        ~Generator() {
            if (m_coroutineHandle)
                m_coroutineHandle.destroy();
        }

        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& other) noexcept : m_coroutineHandle(other.m_coroutineHandle) {
            other.m_coroutineHandle = nullptr;
        }

        Generator& operator=(Generator&& other) noexcept {
            if (this != &other) {
                if (m_coroutineHandle)
                    m_coroutineHandle.destroy();
                m_coroutineHandle = other.m_coroutineHandle;
                other.m_coroutineHandle = nullptr;
            }
            return *this;
        }

        class Iterator {
        public:
            explicit Iterator(handle_type h) : m_coroutineHandle(h) {}

            Iterator& operator++() {
                m_coroutineHandle.resume();
                if (m_coroutineHandle.done())
                    m_coroutineHandle = nullptr;

                return *this;
            }

            T operator*() const { return m_coroutineHandle.promise().current_value.value(); }
            bool operator==(std::default_sentinel_t) const { return !m_coroutineHandle || m_coroutineHandle.done(); }
            bool operator!=(std::default_sentinel_t) const { return m_coroutineHandle && !m_coroutineHandle.done(); }

        private:
            handle_type m_coroutineHandle;
        };

        Iterator begin() {
            if (m_coroutineHandle)
                m_coroutineHandle.resume();

            return Iterator(m_coroutineHandle);
        }
        std::default_sentinel_t end() { return {}; }

    private:
        handle_type m_coroutineHandle;
    };

}