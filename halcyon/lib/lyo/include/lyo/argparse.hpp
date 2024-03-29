#pragma once

#include <cstring>
#include <sstream>

/* argparse.hpp:
   Argument-parsing functions. */

namespace lyo
{
    class parser
    {
    public:
        using constified_argv = const char* const* const;

        parser(int argc, constified_argv argv) noexcept
            : m_argv { argv }
            , m_argc { argc }
        {
        }

        bool has(const char* name) const noexcept
        {
            for (int i { 0 }; i < m_argc; ++i)
            {
                if (std::strcmp(m_argv[i], name) == 0)
                    return true;
            }

            return false;
        }

        template <typename T = const char*>
        T parse(const char* prefix, T default_value) const noexcept
        {
            const char* tok { nullptr };

            for (int i { 0 }; i < m_argc; ++i)
            {
                if ((tok = strstr(m_argv[i], prefix)) != nullptr)
                    break;
            }

            if (!tok) // Nothing found, GTFO.
                return default_value;

            if constexpr (std::is_same_v<T, const char*>)
                return tok + std::strlen(prefix);

            else
            {
                T ret;

                std::istringstream s { tok + std::strlen(prefix) };

                return (s >> ret) ? ret : default_value;
            };
        }

        const char* const* begin() const noexcept
        {
            return m_argv;
        }

        const char* const* end() const noexcept
        {
            return m_argv + m_argc;
        }

    private:
        constified_argv m_argv;
        const int       m_argc;
    };
}