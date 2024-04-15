#include <halcyon/ttf.hpp>

using namespace hal;

ttf::context::context()
{
    HAL_ASSERT_VITAL(::TTF_Init() == 0, ::TTF_GetError());

    HAL_PRINT(severity::init, "Initialized TTF engine");
}

ttf::context::~context() { ::TTF_Quit(); }

ttf::font ttf::context::load(accessor src, lyo::u8 size) &
{
    return { ::TTF_OpenFontRW(src.get(), false, size), {} };
}

bool ttf::context::initialized()
{
    return ::TTF_WasInit() > 0;
}

ttf::font::font(TTF_Font* ptr, lyo::pass_key<ttf::context>)
    : object { ptr }
{
    HAL_WARN_IF(height() != skip(), '\"', family(), ' ', style(), "\" has different height (", height(), "px) & skip (", skip(), "px). size_text() might not return accurate vertical results.");
}

ttf::font::~font()
{
    HAL_ASSERT(ttf::context::initialized(), "TTF context inactive in font destructor");
}

surface ttf::font::render(const std::string_view& text, color color) const
{
    return ::TTF_RenderUTF8_LCD_Wrapped(this->ptr(), text.data(), color.to_sdl_color(), {}, 0);
}

pixel_point ttf::font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(this->ptr(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

pixel_t ttf::font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(this->ptr()));
}

pixel_t ttf::font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(this->ptr()));
}

std::string_view ttf::font::family() const
{
    return ::TTF_FontFaceFamilyName(ptr());
}

std::string_view ttf::font::style() const
{
    return ::TTF_FontFaceStyleName(ptr());
}