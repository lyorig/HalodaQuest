#include <halcyon/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/renderer.hpp>

using namespace hal;

pixel_point texture_base::size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

void texture_base::opacity(color::value value)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureAlphaMod(this->ptr(), value) == 0, ::SDL_GetError());
}

color texture_base::color_mod() const
{
    color c;

    HAL_ASSERT_VITAL(::SDL_GetTextureColorMod(this->ptr(), &c.r, &c.g, &c.b) == 0, ::SDL_GetError());

    return c;
}

void texture_base::color_mod(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureColorMod(this->ptr(), clr.r, clr.g, clr.b) == 0, ::SDL_GetError());
}

blend_mode texture_base::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetTextureBlendMode(this->ptr(), &bm) == 0, ::SDL_GetError());

    return blend_mode(bm);
}

void texture_base::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, ::SDL_GetError());
}

lyo::u8 texture_base::opacity() const
{
    Uint8 alpha;

    HAL_ASSERT_VITAL(::SDL_GetTextureAlphaMod(this->ptr(), &alpha) == 0,
        ::SDL_GetError());

    return alpha;
}

texture_base::texture_base(SDL_Texture* ptr)
    : object { ptr }
{
    this->blend(blend_mode::blend);
}

void texture_base::reset(SDL_Texture* ptr)
{
    object::reset(ptr);
    this->blend(blend_mode::blend);
}

void texture_base::query(Uint32* format, int* access, int* w, int* h) const
{
    HAL_ASSERT_VITAL(::SDL_QueryTexture(this->ptr(), format, access, w, h) == 0, ::SDL_GetError());
}

texture::texture(renderer& wnd, const surface& image)
    : texture_base { create(wnd, image) }
{
    this->blend(blend_mode::blend);
}

texture& texture::change(renderer& wnd, const surface& image)
{
    texture_base::reset(create(wnd, image));
    return *this;
}

SDL_Texture* texture::create(renderer& rnd, const surface& image)
{
    return ::SDL_CreateTextureFromSurface(rnd.ptr(), image.ptr());
}

target_texture::target_texture(renderer& rnd, const pixel_point& sz)
    : texture_base { create(rnd, sz) }
{
}

void target_texture::resize(renderer& rnd, pixel_point sz)
{
    texture_base::reset(create(rnd, sz));
}

SDL_Texture* target_texture::create(renderer& rnd, const pixel_point& sz)
{
    SDL_Window* wnd { ::SDL_RenderGetWindow(rnd.ptr()) };
    HAL_ASSERT(wnd != nullptr, ::SDL_GetError());

    const Uint32 pixel_format { ::SDL_GetWindowPixelFormat(wnd) };
    HAL_ASSERT(pixel_format != SDL_PIXELFORMAT_UNKNOWN, ::SDL_GetError());

    SDL_Texture* tex { ::SDL_CreateTexture(rnd.ptr(), pixel_format, SDL_TEXTUREACCESS_TARGET, sz.x, sz.y) };
    HAL_ASSERT(tex != nullptr, ::SDL_GetError());

    return tex;
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
        m_src.pos.x == unset<src_t> ? nullptr : &m_src,
        m_dst.pos.x == unset<dst_t> ? nullptr : &m_dst,
        m_angle,
        m_flip,
        {});
}