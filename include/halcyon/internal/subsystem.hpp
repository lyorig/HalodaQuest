#pragma once

#include <cstdint>

#include <SDL.h>

#include <halcyon/debug.hpp>

#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    class context;

    namespace detail
    {
        // Available systems. More will be added as Halcyon adds
        // more features from SDL.
        enum class system
        {
            video  = SDL_INIT_VIDEO,
            events = SDL_INIT_EVENTS
        };

        // In the detail namespace; not meant to be seen by the end user.
        constexpr std::string_view to_string(system s)
        {
            using enum system;

            switch (s)
            {
            case video:
                return "Video";

            case events:
                return "Events";
            }
        }

        template <system S>
        class subinit;

        // A subsystem that doesn't actually (de)initialize anything.
        // This is more of a base class to ensure dependent classes
        // accept proxies as well as actual self-initialized systems.
        template <system S>
        class subsystem
        {
            // Specialized classes provide their own constructors.
            subsystem() = delete;
        };

        // A subsystem that initializes itself.
        template <system S>
        class subinit : public subsystem<S>
        {
        public:
            explicit subinit(context&)
                : subsystem<S> { pass_key<subinit<S>> {} }
            {
                HAL_WARN_IF(initialized(), to_string(S), " subsystem is already initialized");
                HAL_ASSERT_VITAL(::SDL_InitSubSystem(static_cast<std::uint32_t>(S)) == 0, debug::last_error());
                HAL_PRINT(debug::severity::init, to_string(S), " subsystem initialized");
            }

            ~subinit()
            {
                ::SDL_QuitSubSystem(static_cast<std::uint32_t>(S));
                HAL_PRINT(to_string(S), " subsystem quit");
            }

            static bool initialized()
            {
                return ::SDL_WasInit(static_cast<std::uint32_t>(S)) == static_cast<std::uint32_t>(S);
            }
        };
    }
}