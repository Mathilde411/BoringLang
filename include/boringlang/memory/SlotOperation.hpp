#ifndef BORINGLANG_SLOTOPERATION_HPP
#define BORINGLANG_SLOTOPERATION_HPP

#include <bit>
#include <cassert>
#include <cfloat>
#include <climits>
#include <cmath>

#include "Types.hpp"


namespace BoringLang::Memory {
        constexpr uint64_t double_total_bits = sizeof(double) * CHAR_BIT;
    constexpr uint64_t double_sign_bits = 1;
    constexpr uint64_t double_sign_mask = (1L << double_sign_bits) - 1;
    constexpr auto double_mantissa_bits = static_cast<uint64_t>(
        std::ceil((DBL_MANT_DIG - 1) * std::log2(FLT_RADIX))
    );
    constexpr uint64_t double_mantissa_mask = (1L << double_mantissa_bits) - 1;
    constexpr auto double_exponent_bits = static_cast<uint64_t>(
        std::ceil(
            std::log2((DBL_MAX_EXP - DBL_MIN_EXP) + 1)
        )
    );
    constexpr uint64_t double_exponent_mask = (1L << double_exponent_bits) - 1;

    static_assert(double_total_bits == double_mantissa_bits + double_exponent_bits + double_sign_bits);

    constexpr uint64_t leading_bits = 2;
    constexpr uint64_t useful_size = sizeof(Slot) * CHAR_BIT - leading_bits;
    constexpr uint64_t useful_mask = (1L << useful_size) - 1;
    constexpr uint64_t slot_float_sign_bits = 1;
    constexpr uint64_t slot_float_sign_mask = (1L << slot_float_sign_bits) - 1;
    constexpr uint64_t slot_float_exponent_bits = 11;
    constexpr uint64_t slot_float_exponent_mask = (1L << slot_float_exponent_bits) - 1;
    constexpr uint64_t slot_float_mantissa_bits = useful_size - (slot_float_sign_bits + slot_float_exponent_bits);
    constexpr uint64_t slot_float_mantissa_mask = (1L << slot_float_mantissa_bits) - 1;

    static_assert(slot_float_exponent_bits >= double_exponent_bits);
    static_assert(slot_float_mantissa_bits <= double_mantissa_bits);

    constexpr Float slot_to_float(Slot slot) {
        assert(slot >> useful_size == FLOAT);

        uint64_t mantissa = slot & slot_float_mantissa_mask;
        slot >>= slot_float_mantissa_bits;
        uint64_t exponent = slot & slot_float_exponent_mask;
        slot >>= slot_float_exponent_bits;
        uint64_t sign = slot & slot_float_sign_mask;

        mantissa <<= double_mantissa_bits - slot_float_mantissa_bits;

        return std::bit_cast<Float>((sign
            << double_exponent_bits | exponent)
            << double_mantissa_bits | mantissa);
    }

    constexpr Slot float_to_slot(Float num) {
        auto value = std::bit_cast<uint64_t>(num);

        uint64_t mantissa = value & double_mantissa_mask;
        value >>= double_mantissa_bits;
        uint64_t exponent = value & double_exponent_mask;
        value >>= double_exponent_bits;
        uint64_t sign = value & double_sign_mask;

        mantissa >>= double_mantissa_bits - slot_float_mantissa_bits;

        return FLOAT << useful_size |
            (sign << slot_float_exponent_bits | exponent) << slot_float_mantissa_bits | mantissa;
    }

    constexpr Integer slot_to_int(Slot slot) {
        assert(slot >> useful_size == INTEGER);
        if (1L << (useful_size - 1) & slot)
            return std::bit_cast<Integer>(slot | (0b11L << useful_size));
        return std::bit_cast<Integer>(slot&useful_mask);
    }

    constexpr Slot int_to_slot(const Integer num) {
        assert(num > std::bit_cast<Integer>(0xC000000000000000) && num < std::bit_cast<Integer>(0x3FFFFFFFFFFFFFFF));
        return INTEGER << useful_size | (num & useful_mask);
    }

    constexpr Character slot_to_char(const Slot slot) {
        assert(slot >> useful_size == CHARACTER);
        return static_cast<Character>(slot & 0xFFFFFFFF);
    }

    constexpr Slot char_to_slot(const Character num) {
        return (CHARACTER << useful_size) | num;
    }

    constexpr Pointer slot_to_ptr(const Slot slot) {
        assert(slot >> useful_size == POINTER);
        return slot;
    }

    constexpr Slot ptr_to_slot(const Pointer num) {
        return num;
    }
}

#endif //BORINGLANG_SLOTOPERATION_HPP
