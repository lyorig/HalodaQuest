#pragma once

#include <span>

#include <halcyon/audio/types.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/subsystem.hpp>

#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    namespace audio
    {
        namespace builder
        {
            class device;
        }

        class spec : private SDL_AudioSpec
        {
        public:
            // Default constructor.
            // Warning - it doesn't initialize the members within.
            spec() = default;

            spec(i32 freq, audio::format fmt, u8 channels, u16 buffer_size_in_frames);

            SDL_AudioSpec* get(pass_key<builder::device>);
        };

        class device;

        namespace builder
        {
            class device
            {
            public:
                device(pass_key<proxy::audio>);

                // Choose a specific audio device.
                device& name(std::string_view name);

                // Have this be a capture device.
                device& capture();

                device& spec(const spec& s);

                device& changes(std::initializer_list<change> vals);

                audio::device operator()();
                audio::device operator()(audio::spec& obtained);

            private:
                audio::spec m_spec;
                const char* m_name;
                int         m_allowedChanges;
                bool        m_capture;
            };
        }

        static_assert(sizeof(spec) == sizeof(SDL_AudioSpec));

        class device
        {
        public:
            using id_t = SDL_AudioDeviceID;

            // Device querying functions return -1 to signalize an unknown
            // amount of devices etc., but don't intend it to be an error.
            constexpr static id_t invalid_id { static_cast<id_t>(-1) };

            device(const char* name, bool capture, const SDL_AudioSpec* desired, SDL_AudioSpec* obtained, int allowed_changes, pass_key<builder::device>);
            ~device();

            void queue(std::span<const std::byte> data);
            
            void pause(bool p);

            void lock();
            void unlock();

        private:
            id_t m_id;
        };
    }
}