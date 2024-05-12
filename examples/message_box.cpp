#include <ctime>

#include <halcyon/video/message_box.hpp>

constexpr std::string_view
    titles[] {
        "Hello there...",
        "Womp womp.",
        "All your base are belong to us!",
        "<< Surprise Popup >>"
    },
    messages[] {
        "What a thrill",
        "rm -rf /",
        "Shawty like a melody in my head",
        "Le fishe au chocolat",
        "Aeaeaeaeaeaeeaeeaeaeaeaeaeea",
        "This post was made by M14 gang"
    };

template <std::size_t N>
std::string_view random(const std::string_view (&span)[N])
{
    return span[std::time(nullptr) % N];
}

int main(int, char*[])
{
    using enum hal::message_box::type;
    using namespace hal::palette;

    static_assert(!hal::compile_settings::exit_on_panic);

    auto msgb = hal::message_box::builder()
                    .buttons({ "Ok", "Yuh", "Nah", "Well", "I mean", "Probably", "Not really", "I'll see", "No idea" })
                .colors(blue, red, red, orange, yellow);

    for (auto type : { info, warning, error })
    {
        const auto ret = msgb.type(type)
                             .title(random(titles))
                             .body(random(messages))();

        HAL_PRINT("User pressed button ", hal::to_printable_int(ret));
    }

    return EXIT_SUCCESS;
}