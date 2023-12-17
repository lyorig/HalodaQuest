#pragma once

#include <SDL2/SDL_mixer.h>

#include <halcyon/types/tags.hpp>
#include <lyo/pass_key.hpp>
#include <lyo/timer.hpp>

#include "sdl_object.hpp"

namespace hal
{
    class mixer;

    class music : public sdl_object<Mix_Music, &::Mix_FreeMusic>
    {
    public:
        music(lyo::pass_key<mixer>);

        music& load(const char* path);

        void play(lyo::u16 loops = 0);
        void play(infinite_loop_tag);

        void fade_in(lyo::f64 time, lyo::u16 loops = 0);
        void fade_in(lyo::f64 time, infinite_loop_tag);

        void fade_out(lyo::f64 time);

        void pause();
        void resume();

        bool playing() const;

        lyo::u8  volume() const;
        lyo::f64 position() const;
        lyo::f64 duration() const;

        void set_volume(lyo::u8 volume) const;
        void jump(lyo::f64 time);

    private:
        void internal_load(const char* path);
        void internal_play(int loops);
        void internal_fade(lyo::f64 time, int loops);
    };
} // namespace hal