#pragma once

#include <halcyon/context.hpp>

#include <halcyon/surface.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>
#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/internal/accessor.hpp>
#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    namespace detail
    {
        template <>
        class subsystem<system::video>
        {
            class clipboard_proxy
            {
            public:
                // Get current clipboard text.
                // This has to be freed, so a std::string is provided instead.
                std::string operator()() const;

                // Set clipboard text.
                void operator()(std::string_view text);

                bool has_text() const;
            };

            class display_proxy
            {
            public:
                video::display::id_t size() const;

                video::display operator[](video::display::id_t idx) const;
            };

            using event_proxy = hal::detail::subsystem<hal::detail::system::events>;

        public:
            subsystem() = default;

            HAL_NO_SIZE event_proxy events;

            HAL_NO_SIZE clipboard_proxy clipboard;
            HAL_NO_SIZE display_proxy   displays;
        };
    }
    namespace video
    {
        class system : public hal::detail::subinit<hal::detail::system::video>
        {
        public:
            using subinit::subinit;
        };

        static_assert(std::is_empty_v<system>);
    }
}