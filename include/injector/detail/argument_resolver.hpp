#pragma once

#include "injector/injector.hpp"

namespace injector::detail
{
    template<class T>
    class ConstructorArgumentResolver
    {
    public:
        explicit ConstructorArgumentResolver(injector::Injector& injector)
            : m_Injector(std::addressof(injector))
        {
        }

        template<class ConstructorArgument, typename std::enable_if_t<!std::is_same_v<ConstructorArgument, T> && !std::is_same_v<ConstructorArgument, ConstructorArgument&> && !std::is_pointer_v<ConstructorArgument>, bool> = true>
        operator ConstructorArgument() // NOLINT implicit conversion
        {
            return m_Injector->get<ConstructorArgument>();
        }

    private:
        injector::Injector* m_Injector;
    };
} // namespace injector::detail