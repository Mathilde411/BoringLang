//
// Created by mathilde on 16/07/23.
//

#ifndef BORINGVM_FORMAT_HPP
#define BORINGVM_FORMAT_HPP

namespace BoringLang {
    enum Format {
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


        PRIMITIVE_INT    = PRIMITIVE | INT     | SIZE64,
        PRIMITIVE_FLOAT  = PRIMITIVE | FLOAT   | SIZE64,
        PRIMITIVE_CHAR   = PRIMITIVE | CHAR    | SIZE64,
        PRIMITIVE_BOOL   = PRIMITIVE | BOOL    | SIZE64,
        PRIMITIVE_STRING = PRIMITIVE | STRING  | SIZE64,

        BASE             = FIXED     | NO_TYPE | NO_SIZE
    };
}

#endif //BORINGVM_FORMAT_HPP
