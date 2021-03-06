#pragma once

#include <unordered_map>

#include "errors.hpp"
#include "traits.hpp"
#include "type_id.hpp"
#include "injector/detail/provider.hpp"

namespace injector
{
    using detail::ConstantFactory;
    using detail::FunctionFactory;
    using detail::ConstructorFactory;

    using detail::InstanceStorage;
    using detail::SingletonInstanceStorage;

    using detail::IComponentProvider;
    using detail::ComponentProviderBase;
    using detail::CastingComponentProvider;
    using detail::NonCastingComponentProvider;

    class Injector
    {
    public:
        /**
         * Add binding to given type.
         * With this binding given type object will be constructed on each retrieval request.
         * @tparam T target for binding
         */
        template<class T>
        void add()
        {
            auto factory = std::make_unique<ConstructorFactory<T>>();
            auto storage = std::make_unique<InstanceStorage<T>>(std::move(factory));

            add_registration<T>(std::move(storage));
        }

        /**
         * Try add binding to given type.
         * This method only adds given type if it has not already been added.
         * With this binding given type object will be constructed on each retrieval request.
         * @tparam T target for binding
         * @see add
         */
        template<class T>
        void try_add()
        {
            if (!contains<T>())
            {
                add<T>();
            }
        }

        /**
         * Add binding from Base to Derived type.
         * With this binding given type object will be constructed on each retrieval request.
         * @tparam Base base on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         */
        template<class Base, class Derived>
        void add()
        {
            auto factory = std::make_unique<ConstructorFactory<Derived>>();
            auto storage = std::make_unique<InstanceStorage<Derived>>(std::move(factory));

            add_registration<Base, Derived>(std::move(storage));
        }

        /**
         * Try to add binding from Base to Derived type.
         * This method only adds given binding if Base has not already been added.
         * With this binding given type object will be constructed on each retrieval request.
         * @tparam Base base type on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         * @see add
         */
        template<class Base, class Derived>
        void try_add()
        {
            if (!contains<Base>())
            {
                add<Base, Derived>();
            }
        }

        /**
         * Add binding to given type in singleton scope (each request to given type will produce same object).
         * With this binding given type will be created only on first retrieval request, on subsequent requests it will return same object
         * @tparam T target for binding
         */
        template<class T>
        void add_singleton()
        {
            auto factory = std::make_unique<ConstructorFactory<T>>();
            auto storage = std::make_unique<SingletonInstanceStorage<T>>(std::move(factory));

            add_registration<T>(std::move(storage));
        }

        /**
         * Try to add binding to given type in singleton scope (each request to given type will produce same object).
         * This method only adds given type if it has not already been added.
         * With this binding given type will be created only on first retrieval request, on subsequent requests it will return same object
         * @tparam T target for binding
         * @seem add_singleton
         */
        template<class T>
        void try_add_singleton()
        {
            if (!contains<T>())
            {
                add_singleton<T>();
            }
        }

        /**
         * Add binding from Base to Derived type in singleton scope (each request to Base type will produce same Derived instance object).
         * With this binding given type will be created only on first retrieval request, on subsequent requests it will return same object
         * @tparam Base base on which binding will be performed
         * @tparam Derived Derived actual type that will be constructed when requesting Base type
         */
        template<class Base, class Derived>
        void add_singleton()
        {
            auto factory = std::make_unique<ConstructorFactory<Derived>>();
            auto storage = std::make_unique<SingletonInstanceStorage<Derived>>(std::move(factory));

            add_registration<Base, Derived>(std::move(storage));
        }

        /**
         * Try to add binding from Base to Derived type in singleton scope (each request to Base type will produce same Derived instance object).
         * This method only adds given binding if Base type has not already been added.
         * With this binding given type will be created only on first retrieval request, on subsequent requests it will return same object
         * @tparam Base base on which binding will be performed
         * @tparam Derived Derived actual type that will be constructed when requesting Base type
         */
        template<class Base, class Derived>
        void try_add_singleton()
        {
            if (!contains<Base>())
            {
                add_singleton<Base, Derived>();
            }
        }

        /**
         * Add binding to given type with function for instance retrieval.
         * With this binding given function will be invoked on each retrieval request
         * @tparam T target for binding
         * @param fn function to invoke when creating given type object
         */
        template<class T>
        void add(const std::function<std::shared_ptr<T>()>& fn) // NOLINT short name
        {
            auto factory = std::make_unique<FunctionFactory<T>>(fn);
            auto storage = std::make_unique<InstanceStorage<T>>(std::move(factory));

            add_registration<T>(std::move(storage));
        }

        /**
         * Try add binding to given type with function for instance retrieval.
         * This method only adds binding if type has not already been added.
         * With this binding given function will be invoked on each retrieval request
         * @tparam T target for binding
         * @param fn function to invoke when creating given type object
         */
        template<class T>
        void try_add(const std::function<std::shared_ptr<T>()>& fn) // NOLINT short name
        {
            if (!contains<T>())
            {
                add<T>(fn);
            }
        }

        /**
         * Add binding from Base to Derived type with function as instance retrieval.
         * With this binding given function will be invoked on each retrieval request
         * @tparam Base base on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         * @param fn function to invoke when creating given Base type object
         */
        template<class Base, class Derived>
        void add(const std::function<std::shared_ptr<Derived>()>& fn) // NOLINT short name
        {
            auto factory = std::make_unique<FunctionFactory<Derived>>(fn);
            auto storage = std::make_unique<InstanceStorage<Derived>>(std::move(factory));

            add_registration<Base, Derived>(std::move(storage));
        }

        /**
         * Try add binding from Base to Derived type with function as instance retrieval.
         * This method only adds binding if Base type has not already been added.
         * With this binding given function will be invoked on each retrieval request.
         * @tparam Base base on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         * @param fn function to invoke when creating given Base type object
         */
        template<class Base, class Derived>
        void try_add(const std::function<std::shared_ptr<Derived>()>& fn) // NOLINT short name
        {
            if (!contains<Base>())
            {
                add<Base, Derived>(fn);
            }
        }

        /**
         * Add binding to given type in singleton scope with function for instance retrieval.
         * With this binding given function will be invoked only on first retrieval request, on subsequent requests it will return same object
         * @tparam T target for binding
         * @param fn function to invoke when creating given type object
         */
        template<class T>
        void add_singleton(const std::function<std::shared_ptr<T>()>& fn) // NOLINT short name
        {
            auto factory = std::make_unique<FunctionFactory<T>>(fn);
            auto storage = std::make_unique<SingletonInstanceStorage<T>>(std::move(factory));

            add_registration<T>(std::move(storage));
        }

        /**
         * Try add binding to given type in singleton scope with function for instance retrieval.
         * This method only adds binding if type has not already been added.
         * With this binding given function will be invoked only on first retrieval request, on subsequent requests it will return same object
         * @tparam T target for binding
         * @param fn function to invoke when creating given type object
         */
        template<class T>
        void try_add_singleton(const std::function<std::shared_ptr<T>()>& fn) // NOLINT short name
        {
            if (!contains<T>())
            {
                add_singleton<T>(fn);
            }
        }

        /**
         * Add binding from Base to Derived type in singleton scope with function as instance retrieval.
         * With this binding given function will be invoked only on first retrieval request, on subsequent requests it will return same object
         * @tparam Base base on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         * @param fn function to invoke when creating given Base type object
         */
        template<class Base, class Derived>
        void add_singleton(const std::function<std::shared_ptr<Derived>()>& fn) // NOLINT short name
        {
            auto factory = std::make_unique<FunctionFactory<Derived>>(fn);
            auto storage = std::make_unique<SingletonInstanceStorage<Derived>>(std::move(factory));

            add_registration<Base, Derived>(std::move(storage));
        }

        /**
         * Try add binding from Base to Derived type in singleton scope with function as instance retrieval.
         * This method only adds binding if Base type has not already been added.
         * With this binding given function will be invoked only on first retrieval request, on subsequent requests it will return same object
         * @tparam Base base on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         * @param fn function to invoke when creating given Base type object
         */
        template<class Base, class Derived>
        void try_add_singleton(const std::function<std::shared_ptr<Derived>()>& fn) // NOLINT short name
        {
            if (!contains<Base>())
            {
                add_singleton<Base, Derived>(fn);
            }
        }

        /**
         * Add binding from Base to Derived type with given object.
         * With this binding value same object will be returned on each retrieval request.
         * This effectively makes given binding a binding in singleton scope
         * @tparam Base base on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         * @param data value to return when requesting Base type object
         */
        template<class Base, class Derived>
        void add(const std::shared_ptr<Derived>& data)
        {
            auto factory = std::make_unique<ConstantFactory<Derived>>(data);
            auto storage = std::make_unique<InstanceStorage<Derived>>(std::move(factory));

            add_registration<Base, Derived>(std::move(storage));
        }

        /**
         * Try add binding from Base to Derived type with given object.
         * This method only adds binding if type has not already been added.
         * With this binding value same object will be returned on each retrieval request.
         * This effectively makes given binding a binding in singleton scope
         * @tparam Base base on which binding will be performed
         * @tparam Derived actual type that will be constructed when requesting Base type
         * @param data value to return when requesting Base type object
         */
        template<class Base, class Derived>
        void try_add(const std::shared_ptr<Derived>& data)
        {
            if (!contains<Base>())
            {
                add<Base, Derived>(data);
            }
        }

        // get<T>
        template<class T,
                 typename std::enable_if_t<!is_vector_v<T> && !is_shared_v<T>, bool> = true>
        std::shared_ptr<T> get()
        {
            auto value = get_unchecked<T>();

            if (!value)
            {
                throw ComponentCreationException();
            }

            return value;
        }

        // get<std::shared_ptr<T>>
        template<class T,
                 typename std::enable_if_t<!is_vector_v<T> && is_shared_v<T>, bool> = true>
        std::shared_ptr<typename T::element_type> get()
        {
            return get<typename T::element_type>();
        };

        // get<const T&>
        template<class T,
                 typename std::enable_if_t<std::is_reference_v<T> && std::is_const_v<typename std::remove_reference<T>>, bool> = true>
        std::shared_ptr<std::remove_reference<typename std::remove_const<T>>> get()
        {
            return get<std::remove_reference<typename std::remove_const<T>>>();
        }

        // get<std::vector<T>>
        template<class T,
                 typename std::enable_if_t<is_vector_v<T> && !is_shared_v<typename T::value_type>, bool> = true>
        std::vector<std::shared_ptr<typename T::value_type>> get()
        {
            using instance_type = typename T::value_type;
            using provider_base = ComponentProviderBase<instance_type>;

            std::vector<std::shared_ptr<instance_type>> instances;
            auto it = m_Registrations.find(type_id<instance_type>()); // NOLINT short name

            if (it != m_Registrations.end())
            {
                auto& providers = it->second;
                instances.reserve(providers.size());

                for (auto& provider : providers)
                {
                    auto* component_provider = static_cast<provider_base*>(provider.get());
                    instances.push_back(component_provider->get(*this));
                }
            }

            return instances;
        }

        // get<std::vector<std::shared_ptr<T>>>
        template<class T,
            typename std::enable_if_t<is_vector_v<T> && is_shared_v<typename T::value_type>, bool> = true>
        std::vector<typename T::value_type> get()
        {
            return get<std::vector<typename T::value_type::element_type>>();
        }

        template<class T>
        [[nodiscard]] bool contains() const noexcept
        {
            return m_Registrations.find(type_id<T>()) != m_Registrations.end();
        }

    private:
        template<class T>
        std::shared_ptr<T> get_unchecked()
        {
            auto it = m_Registrations.find(type_id<T>()); // NOLINT short name

            if (it != m_Registrations.end())
            {
                auto& last_provider = it->second.back();
                auto* provider = static_cast<ComponentProviderBase<T>*>(last_provider.get());

                return provider->get(*this);
            }

            ConstructorFactory<T> factory;
            return factory.build(*this);
        }

        template<class Base, class Derived>
        void add_registration(std::unique_ptr<InstanceStorage<Derived>>&& storage)
        {
            static_assert(std::is_base_of_v<Base, Derived>, "Cannot bind unrelated types");

            auto provider = std::make_unique<CastingComponentProvider<Base, Derived>>(std::move(storage));
            m_Registrations[type_id<Base>()].push_back(std::move(provider));
        }

        template<class T>
        void add_registration(std::unique_ptr<InstanceStorage<T>>&& storage)
        {
            auto provider = std::make_unique<NonCastingComponentProvider<T>>(std::move(storage));
            m_Registrations[type_id<T>()].push_back(std::move(provider));
        }

        std::unordered_map<std::size_t, std::vector<std::unique_ptr<IComponentProvider>>> m_Registrations;
    };
} // namespace injector