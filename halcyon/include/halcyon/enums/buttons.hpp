#pragma once

#include <SDL_mouse.h>
#include <SDL_scancode.h>

#include <lyo/types.hpp>

// buttons.hpp:
// A map of SDL's scancodes for use with Halcyon's event wrapper.

namespace hal
{
    enum class button : lyo::u16
    {
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,

        one   = SDL_SCANCODE_1,
        two   = SDL_SCANCODE_2,
        three = SDL_SCANCODE_3,
        four  = SDL_SCANCODE_4,
        five  = SDL_SCANCODE_5,
        six   = SDL_SCANCODE_6,
        seven = SDL_SCANCODE_7,
        eight = SDL_SCANCODE_8,
        nine  = SDL_SCANCODE_9,

        F1  = SDL_SCANCODE_F1,
        F2  = SDL_SCANCODE_F2,
        F3  = SDL_SCANCODE_F3,
        F4  = SDL_SCANCODE_F4,
        F5  = SDL_SCANCODE_F5,
        F6  = SDL_SCANCODE_F6,
        F7  = SDL_SCANCODE_F7,
        F8  = SDL_SCANCODE_F8,
        F9  = SDL_SCANCODE_F9,
        F10 = SDL_SCANCODE_F10,
        F11 = SDL_SCANCODE_F11,
        F12 = SDL_SCANCODE_F12,

        esc        = SDL_SCANCODE_ESCAPE,
        tab        = SDL_SCANCODE_TAB,
        caps_lock  = SDL_SCANCODE_CAPSLOCK,
        left_shift = SDL_SCANCODE_LSHIFT,
        left_ctrl  = SDL_SCANCODE_LCTRL,

        backspace   = SDL_SCANCODE_BACKSPACE,
        enter       = SDL_SCANCODE_RETURN,
        right_shift = SDL_SCANCODE_RSHIFT,
        right_ctrl  = SDL_SCANCODE_RCTRL,

        left_alt  = SDL_SCANCODE_LALT,
        right_alt = SDL_SCANCODE_RALT,

        left_mouse    = SDL_NUM_SCANCODES + SDL_BUTTON_LEFT - 1,
        middle_moouse = SDL_NUM_SCANCODES + SDL_BUTTON_MIDDLE - 1,
        right_mouse   = SDL_NUM_SCANCODES + SDL_BUTTON_RIGHT - 1,
        mouse_x1      = SDL_NUM_SCANCODES + SDL_BUTTON_X1 - 1,
        mouse_x2      = SDL_NUM_SCANCODES + SDL_BUTTON_X2 - 1
    };
}