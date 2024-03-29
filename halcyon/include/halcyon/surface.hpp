#pragma once

#include <SDL_surface.h>

#include <halcyon/enums/blend.hpp>
#include <halcyon/other/drawer.hpp>
#include <halcyon/types/color.hpp>
#include <lyo/pass_key.hpp>
#include <span>

#include <halcyon/internal/sdl_object.hpp>

// surface.hpp:
// A "software surface" used by SDL. The user should never
// directly create a surface, but instead use various loading
// functions of other classes, which are responsible for providing
// a valid SDL_Surface pointer. On top of staying true to the SDL object
// structure, another upside comes in the form of early error handling.

namespace hal
{
    class image_loader;
    class font;

    class blitter;

    LYO_TAG(keep_dst);

    class surface : public sdl::object<SDL_Surface, &::SDL_FreeSurface>
    {
    public:
        // Create a sized surface.
        surface(pixel_point sz);

        // Special c-tor for factory classes.
        surface(SDL_Surface* surf, lyo::pass_key<image_loader>);
        surface(SDL_Surface* surf, lyo::pass_key<font>);

        // Get a resized copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(pixel_point sz);
        surface resize(lyo::f64 scale);

        void fill(color clr);
        void fill_rect(const sdl::pixel_rect& area, color clr);
        void fill_rects(const std::span<const sdl::pixel_rect>& areas, color clr);

        pixel_point size() const;

        // Get pixel at position.
        // This functionality is exclusive to surfaces, as textures
        // are extremely slow to retrieve pixel information.
        color operator[](const pixel_point& pos) const;

        blend_mode blend() const;
        void       blend(blend_mode bm);

        [[nodiscard]] blitter blit(surface& dst) const;

        // Public, but only available to the blit class.
        void internal_blit(const surface& to, const sdl::pixel_rect* src, sdl::pixel_rect* dst, lyo::pass_key<blitter>) const;

    private:
        // I'd rather use the pixel format enum, but SDL uses an integer
        // in pretty much every API function, so I'll save myself the hassle.
        surface(pixel_point sz, int depth, Uint32 format);

        // Construct a new surface from an existing one's pixel format.
        // Used for resizing.
        surface(pixel_point sz, const SDL_PixelFormat* fmt);

        // Get a color value corresponding to this surface's format.
        Uint32 mapped(color clr) const;
        Uint32 pixel_at(const pixel_point& pos) const;
    };

    class blitter : public drawer<surface, sdl::pixel_t, const surface, blitter>
    {
    public:
        using drawer::drawer;

        void operator()();

        // SDL's blitting function overwrites the destination rectangle.
        // This overload creates a copy to ensure it remains unchanged.
        void operator()(keep_dst_tag) const;
    };

    template <typename T>
    class blend_lock
    {
    public:
        blend_lock(T& obj, blend_mode new_mode)
            : m_obj { obj }
            , m_old { obj.blend() }
        {
            obj.blend(new_mode);
        }

        ~blend_lock()
        {
            m_obj.blend(m_old);
        }

    private:
        T& m_obj;

        blend_mode m_old;
    };
} // namespace hal