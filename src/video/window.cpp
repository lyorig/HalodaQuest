#include <halcyon/video/window.hpp>

#include <halcyon/event/handler.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal::video;

window::window(authority&, std::string_view name, pixel_point size, std::initializer_list<flags> flags)
    : object { ::SDL_CreateWindow(name.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, detail::to_bitmask<std::uint32_t>(flags)) }
{
    HAL_PRINT(debug::severity::init, "Created window \"", title(), "\", flags = 0x", std::hex, detail::to_bitmask<std::uint32_t>(flags), ", ID = ", to_printable_int(id()));
}

hal::pixel_point window::pos() const
{
    point<int> ret;

    ::SDL_GetWindowPosition(ptr(), &ret.x, &ret.y);

    return ret;
}

void window::pos(hal::pixel_point ps)
{
    ::SDL_SetWindowPosition(ptr(), ps.x, ps.y);
}

hal::pixel_point window::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(this->ptr(), &size.x, &size.y);

    return size;
}

void window::size(pixel_point sz)
{
    HAL_WARN_IF(fullscreen(), "Setting size of fullscreen window - this does nothing");

    ::SDL_SetWindowSize(this->ptr(), sz.x, sz.y);
}

display::id_t window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(this->ptr());

    HAL_ASSERT(ret >= 0, debug::last_error());

    return static_cast<display::id_t>(ret);
}

std::string_view window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}

void window::title(std::string_view val)
{
    ::SDL_SetWindowTitle(ptr(), val.data());
}

window::id_t window::id() const
{
    const auto ret = ::SDL_GetWindowID(ptr());

    HAL_ASSERT(ret != 0, debug::last_error());

    return static_cast<window::id_t>(ret);
}

bool window::fullscreen() const
{
    return static_cast<bool>(::SDL_GetWindowFlags(ptr()) & (std::to_underlying(flags::fullscreen) | std::to_underlying(flags::fullscreen_borderless)));
}

void window::fullscreen(bool set)
{
    HAL_ASSERT_VITAL(::SDL_SetWindowFullscreen(
                         ptr(),
                         set ? std::to_underlying(flags::fullscreen_borderless) : 0)
            == 0,
        debug::last_error());
}