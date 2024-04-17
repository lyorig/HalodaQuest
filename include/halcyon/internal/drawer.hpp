#pragma once
#include <halcyon/types/render.hpp>

#include <lyo/pass_key.hpp>

#include <limits>

namespace hal::detail
{
    template <lyo::arithmetic T>
    constexpr T unset_pos { std::numeric_limits<T>::max() };

    // A base drawer class, implementing the builder method for drawing textures.
    // Designed to be used as an rvalue - all functions should only be called once.
    // It's possible to store it, but this is only recommended for:
    // a) constant textures, and
    // b) those who know what they're doing. I'm sure you do, though.
    // "Now, now, if you follow standard insertion procedures, everything will be fine."
    template <typename T, lyo::one_of<sdl::pixel_t, sdl::coord_t> Dst_type, typename Pass, typename This>
    class drawer
    {
    protected:
        using src_t = sdl::pixel_t;
        using dst_t = Dst_type;

        using src_point = point<src_t>;
        using src_rect  = rectangle<src_t>;

        using dst_point = point<dst_t>;
        using dst_rect  = rectangle<dst_t>;

        using this_ref = std::conditional_t<std::is_void_v<This>, drawer, This>&;

    public:
        [[nodiscard]] drawer(Pass& ths, const T& src, lyo::pass_key<Pass>)
            : m_pass { ths }
            , m_this { src }
            , m_dst { tag::as_size, src.size() }
        {
            m_src.pos.x = unset_pos<src_t>;
        }

        // Set where to draw.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const dst_point& pos)
        {
            m_dst.pos = pos;
            return get_this();
        }

        // Set the destination rectangle.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const dst_rect& area)
        {
            m_dst = area;
            return get_this();
        }

        // Stretch across the board.
        // Do not use with scaling and anchoring.
        [[nodiscard]] this_ref to(LYO_TAG_NAME(fill))
        {
            m_dst.pos.x = unset_pos<dst_t>;
            return get_this();
        }

        // Set the source rectangle.
        // Can be called at any time.
        [[nodiscard]] this_ref from(const pixel_rect& src)
        {
            m_src      = src;
            m_dst.size = dst_point(src.size);
            return get_this();
        }

        // Set the destination's scale.
        // Call after setting the destination and before anchoring.
        [[nodiscard]] this_ref scale(lyo::f64 mul)
        {
            if (m_dst.pos.x != unset_pos<dst_t>)
                m_dst.size *= mul;
            return get_this();
        }

        // Anchor from the destination position.
        // Call after setting the destination and scaling.
        [[nodiscard]] this_ref anchor(anchor anch)
        {
            if (m_dst.pos.x != unset_pos<dst_t>)
                m_dst.pos = m_dst.pos.anchor(anch, m_dst.size);
            return get_this();
        }

        // Access the source rectangle.
        src_rect& src()
        {
            return m_src;
        }

        // Get the source rectangle.
        const src_rect& src() const
        {
            return m_src;
        }

        // Access the destination rectangle.
        dst_rect& dst()
        {
            return m_dst;
        }

        // Get the destination rectangle.
        const dst_rect& dst() const
        {
            return m_dst;
        }

    protected:
        this_ref get_this()
        {
            return static_cast<this_ref>(*this);
        }

        Pass&    m_pass;
        const T& m_this;

        dst_rect m_dst;
        src_rect m_src;
    };
}