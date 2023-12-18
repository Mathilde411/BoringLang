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

#include "boringlang/core/util/BinaryStreamUtil.hpp"
using namespace BoringLang;

void BinaryStreamUtil::readStream(std::istream& stream, char* destination, std::streamsize count) {
    if(!stream.read(destination, count)) {
        if(stream.eof())
            throw EndOfStreamError();
        else
            throw StreamError();
    }
}

void BinaryStreamUtil::writeStream(std::ostream& stream, char* origin, std::streamsize count) {
    if(!stream.write(origin, count)) {
        throw StreamError();
    }
}

uint64_t BinaryStreamUtil::read64BitsNumberStream(std::istream& stream) {
    uint64_t res;
    BinaryStreamUtil::readStream(stream, (char*)&res, 8);
    return ntohll(res);
}

void BinaryStreamUtil::write64BitsNumberStream(std::ostream& stream, uint64_t number) {
    uint64_t res = htonll(number);
    BinaryStreamUtil::writeStream(stream, (char*)&res, 8);
}

uint32_t BinaryStreamUtil::read32BitsNumberStream(std::istream& stream) {
    uint32_t res;
    BinaryStreamUtil::readStream(stream, (char*)&res, 4);
    return ntohl(res);
}

void BinaryStreamUtil::write32BitsNumberStream(std::ostream& stream, uint32_t number) {
    uint32_t res = htonl(number);
    BinaryStreamUtil::writeStream(stream, (char*)&res, 4);
}

uint16_t BinaryStreamUtil::read16BitsNumberStream(std::istream& stream) {
    uint16_t res;
    BinaryStreamUtil::readStream(stream, (char*)&res, 2);
    return ntohs(res);
}

void BinaryStreamUtil::write16BitsNumberStream(std::ostream& stream, uint16_t number) {
    uint16_t res = htons(number);
    BinaryStreamUtil::writeStream(stream, (char*)&res, 2);
}

uint8_t BinaryStreamUtil::read8BitsNumberStream(std::istream& stream) {
    uint8_t res;
    BinaryStreamUtil::readStream(stream, (char*)&res, 1);
    return res;
}

void BinaryStreamUtil::write8BitsNumberStream(std::ostream& stream, uint8_t number) {
    BinaryStreamUtil::writeStream(stream, (char*)&number, 1);
}
