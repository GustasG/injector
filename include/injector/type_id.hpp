#pragma once

#include <atomic>

namespace injector
{
    class TypeId
    {
    public:
        template<class T>
        static std::size_t id() noexcept
        {
            static const auto id = s_LastIndex.fetch_add(1, std::memory_order_relaxed); // NOLINT short name
            return id;
        }

    private:
        static std::atomic<std::size_t> s_LastIndex; // NOLINT non const variable
    };

    template<class T>
    std::size_t type_id() noexcept
    {
        return TypeId::id<T>();
    }
} // namespace injector