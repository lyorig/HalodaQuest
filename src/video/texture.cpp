#include <halcyon/video/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

pixel::point detail::texture_base::size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

void detail::texture_base::opacity(color::value_t value)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureAlphaMod(get(), value) == 0, debug::last_error());
}

color::value_t detail::texture_base::alpha_mod() const
{
    color::value_t ret;

    ::SDL_GetTextureAlphaMod(get(), &ret);

    return ret;
}

void detail::texture_base::alpha_mod(color::value_t val)
{
    ::SDL_SetTextureAlphaMod(get(), val);
}

color detail::texture_base::color_mod() const
{
    color c;

    HAL_ASSERT_VITAL(::SDL_GetTextureColorMod(get(), &c.r, &c.g, &c.b) == 0, debug::last_error());

    return c;
}

void detail::texture_base::color_mod(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureColorMod(get(), clr.r, clr.g, clr.b) == 0, debug::last_error());
}

blend_mode detail::texture_base::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetTextureBlendMode(get(), &bm) == 0, debug::last_error());

    return blend_mode(bm);
}

void detail::texture_base::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureBlendMode(get(), SDL_BlendMode(bm)) == 0, debug::last_error());
}

pixel::format detail::texture_base::pixel_format() const
{
    Uint32 ret;

    query(&ret, nullptr, nullptr, nullptr);

    return static_cast<pixel::format>(ret);
}

u8 detail::texture_base::opacity() const
{
    Uint8 alpha;

    HAL_ASSERT_VITAL(::SDL_GetTextureAlphaMod(get(), &alpha) == 0,
        debug::last_error());

    return alpha;
}

detail::texture_base::texture_base(SDL_Texture* ptr)
    : raii_object { ptr }
{
    this->blend(blend_mode::blend);
}

void detail::texture_base::query(Uint32* format, int* access, int* w, int* h) const
{
    HAL_ASSERT_VITAL(::SDL_QueryTexture(get(), format, access, w, h) == 0, debug::last_error());
}

texture::texture(renderer& rnd, const surface& surf)
    : texture_base { ::SDL_CreateTextureFromSurface(rnd.get(), surf.get()) }
{
}

target_texture::target_texture(renderer& rnd, pixel::format fmt, pixel::point size)
    : texture_base { ::SDL_CreateTexture(rnd.get(), static_cast<Uint32>(fmt), SDL_TEXTUREACCESS_TARGET, size.x, size.y) }
{
}