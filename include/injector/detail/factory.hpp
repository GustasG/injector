#pragma once

#include <memory>
#include <functional>

namespace injector
{
    class Injector;
} // namespace injector

namespace injector::detail
{
    using injector::Injector;

    template<class T>
    class ConstructorArgumentResolver;

    template<class T>
    class ComponentFactory
    {
    public:
        virtual std::shared_ptr<T> build(Injector& injector) = 0;

        virtual ~ComponentFactory() = default;
    };

    template<class T, class Enable = void>
    class ConstructorFactory : public ComponentFactory<T>
    {
    public:
        std::shared_ptr<T> build(Injector& injector) override
        {
            return nullptr;
        }
    };

    // Specialization for constructors with no arguments
    template<class T>
    class ConstructorFactory<T, typename std::enable_if_t<std::is_default_constructible_v<T> && !std::is_abstract_v<T>>> : public ComponentFactory<T>
    {
    public:
        std::shared_ptr<T> build(Injector& injector) override
        {
            return std::make_shared<T>();
        }
    };

    // Specialization for constructors up to 16 arguments
    template<class T>
    class ConstructorFactory<T, typename std::enable_if_t<!std::is_default_constructible_v<T> && !std::is_abstract_v<T>>> : public ComponentFactory<T>
    {
    public:
        std::shared_ptr<T> build(Injector& injector) override
        {
            return try_build(
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector),
                ConstructorArgumentResolver<T>(injector));
        }

    private:
        template<class Arg1, class Arg2, class... Args,
                 typename std::enable_if_t<std::is_constructible_v<T, Arg1, Arg2, Args...>, bool> = true>
        std::shared_ptr<T> try_build(Arg1& arg1, Arg2& a2, Args&&... args)
        {
            return std::make_shared<T>(arg1, a2, std::forward<Args>(args)...);
        }

        template<class Arg1, class Arg2, class... Args,
                 typename std::enable_if_t<!std::is_constructible_v<T, Arg1, Arg2, Args...>, bool> = true>
        std::shared_ptr<T> try_build(Arg1& arg1, Arg2& arg2, Args&&... args)
        {
            return try_build(arg2, std::forward<Args>(args)...);
        }

        template<class Arg,
                 typename std::enable_if_t<std::is_constructible_v<T, Arg>, bool> = true>
        std::shared_ptr<T> try_build(Arg& arg)
        {
            return std::make_shared<T>(arg);
        }

        template<class Arg,
                 typename std::enable_if_t<!std::is_constructible_v<T, Arg>, bool> = true>
        std::shared_ptr<T> try_build(Arg& arg)
        {
            return nullptr;
        }
    };

    template<class T>
    class FunctionFactory : public ComponentFactory<T>
    {
    public:
        explicit FunctionFactory(const std::function<std::shared_ptr<T>()>& factory)
            : m_Factory(factory)
        {
        }

        std::shared_ptr<T> build(Injector& injector) override
        {
            return m_Factory();
        }

    private:
        std::function<std::shared_ptr<T>()> m_Factory;
    };

    template<class T>
    class ConstantFactory : public ComponentFactory<T>
    {
    public:
        explicit ConstantFactory(const std::shared_ptr<T>& data)
            : m_Data(data)
        {
        }

        std::shared_ptr<T> build(Injector& injector) override
        {
            return m_Data;
        }

    private:
        std::shared_ptr<T> m_Data;
    };
} // namespace injector::detail