#pragma once

#include "halcyon/types/other.hpp"
#include <SDL2/SDL_render.h>

#include <halcyon/enums/anchor.hpp>
#include <halcyon/enums/blend.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class draw;

    class window;

    class texture_base;
    class target_texture;

    class renderer : public sdl_object<SDL_Renderer, &::SDL_DestroyRenderer>
    {
    public:
        enum flags : lyo::u8
        {
            none = 0,
            software = SDL_RENDERER_SOFTWARE,
            accelerated = SDL_RENDERER_ACCELERATED,
            vsync = SDL_RENDERER_PRESENTVSYNC
        };

        // Might as well leave the pure bitmask parameter here.
        renderer(window& wnd, il<flags> flags);

        void present();

        void draw_line(const coord& from, const coord& to);
        void draw_rect(const coord_area& area);

        void fill_rect(const SDL::FRect& area);
        void fill_target();

        void set_target(target_texture& tx);
        void reset_target();

        color draw_color() const;
        void  set_draw_color(color clr);

        blend_mode blend() const;
        void       set_blend(blend_mode bm);

        pixel_size output_size() const;

        // Public, but only accessible to the draw class.
        void internal_render_copy(const texture_base& tex, const SDL_Rect* src, const SDL_FRect* dst, lyo::f64 angle, flip f, lyo::pass_key<draw>);

    private:
        void clear();

        void internal_set_target(SDL_Texture* target);
    };

    // C-tor: Sets the renderer's draw color.
    // D-tor: Sets the draw color back to the previous one.
    class color_lock
    {
    public:
        color_lock(renderer& rnd, color new_clr);

        ~color_lock();

    private:
        renderer&   m_rnd;
        const color m_old;
    };

    // C-tor: Sets the renderer's target texture.
    // D-tor: Sets the target back to the renderer's window.
    class target_lock
    {
    public:
        target_lock(renderer& rnd, target_texture& tgt);

        ~target_lock();

    private:
        renderer& m_rnd;
    };
}