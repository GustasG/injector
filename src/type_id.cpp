#include "injector/type_id.hpp"

namespace injector
{
    std::atomic<std::size_t> TypeId::s_LastIndex(0); // NOLINT non const variable
} // namespace injector