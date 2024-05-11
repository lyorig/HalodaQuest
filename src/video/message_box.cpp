#include <halcyon/video/message_box.hpp>

#include <utility>

#include <halcyon/debug.hpp>

using namespace hal;

using msbb = message_box::builder;

msbb::builder()
    : m_btn { { { .flags = 0, .buttonid = 0, .text = "Ok" } } }
    , m_data {
        .flags       = SDL_MESSAGEBOX_INFORMATION,
        .window      = nullptr,
        .title       = "Halcyon Message Box",
        .message     = "No message provided.",
        .numbuttons  = 1,
        .buttons     = m_btn.data(),
        .colorScheme = nullptr
    }
{
}

msbb& msbb::title(std::string_view text)
{
    m_data.title = text.data();

    return *this;
}

msbb& msbb::message(std::string_view text)
{
    m_data.message = text.data();

    return *this;
}

msbb& msbb::type(message_box::type tp)
{
    m_data.flags = std::to_underlying(tp);

    return *this;
}

msbb& msbb::buttons(std::initializer_list<std::string_view> names)
{
    namespace mb = message_box;

    HAL_ASSERT(names.size() <= mb::max_buttons(), "Too many buttons requested (>", to_printable_int(mb::max_buttons()), ')');

    m_data.numbuttons = static_cast<int>(names.size());

    for (mb::button_t i { 0 }; i < m_data.numbuttons; ++i)
    {
        auto btn_it = m_btn.begin() + i;
        auto str_it = names.begin() + i;

        btn_it->buttonid = i;
        btn_it->flags    = 0;
        btn_it->text     = str_it->data();
    }

    return *this;
}

msbb& msbb::enter(message_box::button_t id)
{
    HAL_ASSERT(id < m_data.numbuttons, "Out of range button ID");

    m_btn[id].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;

    return *this;
}

msbb& msbb::escape(message_box::button_t id)
{
    HAL_ASSERT(id < m_data.numbuttons, "Out of range button ID");

    m_btn[id].flags |= SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;

    return *this;
}

msbb& msbb::parent(window& wnd)
{
    m_data.window = wnd.get();

    return *this;
}

message_box::button_t msbb::operator()()
{
    int ret;

    HAL_ASSERT_VITAL(::SDL_ShowMessageBox(&m_data, &ret) == 0, debug::last_error());

    return ret;
}