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

#ifndef BORINGLANG_CLASSFORMAT_HPP
#define BORINGLANG_CLASSFORMAT_HPP

namespace BoringLang {
    enum ClassFormat {
        FIXED            = 0b00  << 6,
        INDEXABLE        = 0b10  << 6,
        PRIMITIVE        = 0b01  << 6,
        FORMAT_MASK      = 0b11  << 6,

        NO_TYPE          = 0b000 << 3,
        INT              = 0b001 << 3,
        FLOAT            = 0b010 << 3,
        CHAR             = 0b011 << 3,
        BOOL             = 0b100 << 3,
        STRING           = 0b101 << 3,
        TYPE_MASK        = 0b111 << 3,

        NO_SIZE          = 0b000,
        SIZE8            = 0b001,
        SIZE16           = 0b010,
        SIZE32           = 0b011,
        SIZE64           = 0b100,
        SIZE128          = 0b101,
        SIZE_MASK        = 0b111,


        PRIMITIVE_INT    = PRIMITIVE | INT     | NO_SIZE,
        PRIMITIVE_FLOAT  = PRIMITIVE | FLOAT   | NO_SIZE,
        PRIMITIVE_CHAR   = PRIMITIVE | CHAR    | NO_SIZE,
        PRIMITIVE_BOOL   = PRIMITIVE | BOOL    | NO_SIZE,
        PRIMITIVE_STRING = PRIMITIVE | STRING  | NO_SIZE,

        BASE             = FIXED     | NO_TYPE | NO_SIZE
    };
}

#endif //BORINGLANG_CLASSFORMAT_HPP
