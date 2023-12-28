#include <halcyon/internal/subsystem.hpp>

using namespace hal;

display::display(display::index_type idx, lyo::pass_key<video>)
    : index { idx }
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(idx, static_cast<SDL_DisplayMode*>(this)), ::SDL_GetError());
}

pixel_size display::size() const
{
    return { w, h };
}

display::hz_type display::hz() const
{
    return refresh_rate;
}

const char* display::name() const
{
    const char* name { ::SDL_GetDisplayName(index) };

    HAL_DEBUG_ASSERT(name != nullptr, ::SDL_GetError());

    return name;
}

display::index_type video::num_displays() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_DEBUG_ASSERT(ret != -1, ::SDL_GetError());

    return ret;
}

struct display video::display_at(display::index_type idx) const
{
    return { idx, {} };
}