#include <halcyon/video/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

pixel_point detail::texture_base::size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

void detail::texture_base::opacity(color::value_t value)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureAlphaMod(this->ptr(), value) == 0, debug::last_error());
}

color detail::texture_base::color_mod() const
{
    color c;

    HAL_ASSERT_VITAL(::SDL_GetTextureColorMod(this->ptr(), &c.r, &c.g, &c.b) == 0, debug::last_error());

    return c;
}

void detail::texture_base::color_mod(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureColorMod(this->ptr(), clr.r, clr.g, clr.b) == 0, debug::last_error());
}

blend_mode detail::texture_base::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetTextureBlendMode(this->ptr(), &bm) == 0, debug::last_error());

    return blend_mode(bm);
}

void detail::texture_base::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, debug::last_error());
}

lyo::u8 detail::texture_base::opacity() const
{
    Uint8 alpha;

    HAL_ASSERT_VITAL(::SDL_GetTextureAlphaMod(this->ptr(), &alpha) == 0,
        debug::last_error());

    return alpha;
}

detail::texture_base::texture_base(SDL_Texture* ptr)
    : object { ptr }
{
    this->blend(blend_mode::blend);
}

void detail::texture_base::reset(SDL_Texture* ptr)
{
    object::reset(ptr);
    this->blend(blend_mode::blend);
}

void detail::texture_base::query(std::uint32_t* format, int* access, int* w, int* h) const
{
    HAL_ASSERT_VITAL(::SDL_QueryTexture(this->ptr(), format, access, w, h) == 0, debug::last_error());
}

texture::texture(SDL_Texture* ptr, lyo::pass_key<renderer>)
    : texture_base { ptr }
{
}

target_texture::target_texture(SDL_Texture* ptr, lyo::pass_key<renderer>)
    : texture_base { ptr }
{
}

copyer& copyer::rotate(lyo::f64 angle)
{
    m_angle = angle;
    return *this;
}

copyer& copyer::flip(enum flip f)
{
    m_flip = f;
    return *this;
}

void copyer::operator()()
{
    m_pass.internal_render_copy(
        m_this,
        m_src.pos.x == detail::unset_pos<src_t> ? nullptr : &m_src,
        m_dst.pos.x == detail::unset_pos<dst_t> ? nullptr : &m_dst,
        m_angle,
        m_flip,
        {});
}