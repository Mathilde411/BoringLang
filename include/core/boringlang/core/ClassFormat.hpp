//
// Created by mathilde on 16/07/23.
//

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
