#pragma once

#include <cstring>
#include <iomanip>
#include <sstream>

#include "concepts.hpp"

/* utility.hpp:
   Various uncategorized helper functions. */

namespace lyo
{
    template <character T>
        requires(lyo::is_present_v<T, char, wchar_t>)
    constexpr bool streq(const T* first, const T* second) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
            return std::strcmp(first, second) == 0;

        else if constexpr (std::is_same_v<T, wchar_t>)
            return std::wcscmp(first, second) == 0;
    }

    // Checks if the first character of a string is a null terminator.
    template <character T>
    constexpr bool is_c_string_empty(const T* string) noexcept
    {
        return *string == static_cast<T>('\0');
    }

    // Input all arguments into a stringstream and return them as a string.
    template <typename... Args>
    std::string string_from_pack(Args&&... args) noexcept
    {
        // Warning suppression.
        if constexpr (sizeof...(Args) == 0)
            return {};

        else
        {
            std::stringstream stream;

            stream << std::fixed;

            (stream << ... << std::forward<Args>(args));

            return stream.str();
        }
    }
} // namespace lyo