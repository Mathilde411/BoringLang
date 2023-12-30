
//     BoringLang Project
//     Copyright (C) 2023  Mathilde Pascal
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Lesser General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU Lesser General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef BORINGLANG_BINARYSTREAMUTIL_HPP
#define BORINGLANG_BINARYSTREAMUTIL_HPP

#include <istream>

#if _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#if __BIG_ENDIAN__
# define htonll(x) (x)
# define ntohll(x) (x)
#else
# define htonll(x) (((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
# define ntohll(x) (((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif

namespace BoringLang {
    class BinaryStreamUtil {
    public:
        static void readStream(std::istream& stream, void* destination, std::streamsize count);
        static void writeStream(std::ostream& stream, const void* origin, std::streamsize count);

        static uint64_t read64BitsNumberStream(std::istream& stream);
        static void write64BitsNumberStream(std::ostream& stream, uint64_t number);

        static uint32_t read32BitsNumberStream(std::istream& stream);
        static void write32BitsNumberStream(std::ostream& stream, uint32_t number);

        static uint16_t read16BitsNumberStream(std::istream& stream);
        static void write16BitsNumberStream(std::ostream& stream, uint16_t number);

        static uint8_t read8BitsNumberStream(std::istream& stream);
        static void write8BitsNumberStream(std::ostream& stream, uint8_t number);
    };

    class StreamError : public std::runtime_error {
    public:
        StreamError():runtime_error("Error when reading stream."){}
        explicit StreamError(std::string const& msg):runtime_error(msg.c_str()){}
    };

    class EndOfStreamError final : public StreamError {
    public:
        EndOfStreamError():StreamError("End-of-stream reached before end."){}
        explicit EndOfStreamError(std::string const& msg):StreamError(msg){}
    };
}


#endif //BORINGLANG_BINARYSTREAMUTIL_HPP
