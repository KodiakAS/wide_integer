#pragma once

///////////////////////////////////////////////////////////////
//  Distributed under the Boost Software License, Version 1.0.
//  (See at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////

/*  Divide and multiply
 *
 *
 * Copyright (c) 2008
 * Evan Teran
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the same name not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission. We make no representations about the
 * suitability this software for any purpose. It is provided "as is"
 * without express or implied warranty.
 */

#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <fmt/format.h>
#include "endian_compat.h"

#if __cplusplus < 202002L
namespace std
{
template <class T>
using remove_cvref_t = typename remove_cv<typename remove_reference<T>::type>::type;
}
#endif

// NOLINTBEGIN(*)

namespace wide
{
template <size_t Bits, typename Signed>
class integer;
}

namespace std
{

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
struct common_type<wide::integer<Bits, Signed>, wide::integer<Bits2, Signed2>>;

template <size_t Bits, typename Signed, typename Arithmetic>
struct common_type<wide::integer<Bits, Signed>, Arithmetic>;

template <typename Arithmetic, size_t Bits, typename Signed>
struct common_type<Arithmetic, wide::integer<Bits, Signed>>;

}

namespace wide
{

template <size_t Bits, typename Signed>
class integer
{
public:
    using base_type = uint64_t;
    using signed_base_type = int64_t;

    // ctors
    constexpr integer() noexcept = default;

    template <typename T>
    constexpr integer(T rhs) noexcept;

    template <typename T>
    constexpr integer(std::initializer_list<T> il) noexcept;

    // assignment
    template <size_t Bits2, typename Signed2>
    constexpr integer<Bits, Signed> & operator=(const integer<Bits2, Signed2> & rhs) noexcept;

    template <typename Arithmetic>
    constexpr integer<Bits, Signed> & operator=(Arithmetic rhs) noexcept;

    template <typename Arithmetic>
    constexpr integer<Bits, Signed> & operator*=(const Arithmetic & rhs);

    template <typename Arithmetic>
    constexpr integer<Bits, Signed> & operator/=(const Arithmetic & rhs);

    template <typename Arithmetic>
    constexpr integer<Bits, Signed> & operator+=(const Arithmetic & rhs) noexcept(std::is_same_v<Signed, unsigned>);

    template <typename Arithmetic>
    constexpr integer<Bits, Signed> & operator-=(const Arithmetic & rhs) noexcept(std::is_same_v<Signed, unsigned>);

    template <typename Integral>
    constexpr integer<Bits, Signed> & operator%=(const Integral & rhs);

    template <typename Integral>
    constexpr integer<Bits, Signed> & operator&=(const Integral & rhs) noexcept;

    template <typename Integral>
    constexpr integer<Bits, Signed> & operator|=(const Integral & rhs) noexcept;

    template <typename Integral>
    constexpr integer<Bits, Signed> & operator^=(const Integral & rhs) noexcept;

    constexpr integer<Bits, Signed> & operator<<=(int n) noexcept;
    constexpr integer<Bits, Signed> & operator>>=(int n) noexcept;

    constexpr integer<Bits, Signed> & operator++() noexcept(std::is_same_v<Signed, unsigned>);
    constexpr integer<Bits, Signed> operator++(int) noexcept(std::is_same_v<Signed, unsigned>);
    constexpr integer<Bits, Signed> & operator--() noexcept(std::is_same_v<Signed, unsigned>);
    constexpr integer<Bits, Signed> operator--(int) noexcept(std::is_same_v<Signed, unsigned>);

    // observers

    constexpr explicit operator bool() const noexcept;

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr operator T() const noexcept;

    constexpr operator long double() const noexcept;
    constexpr operator double() const noexcept;
    constexpr operator float() const noexcept;

    struct _impl;

    base_type items[_impl::item_count];

private:
    template <size_t Bits2, typename Signed2>
    friend class integer;

    friend class std::numeric_limits<integer<Bits, signed>>;
    friend class std::numeric_limits<integer<Bits, unsigned>>;
};

template <typename T>
static constexpr bool ArithmeticConcept() noexcept;

template <class T1, class T2>
using _only_arithmetic = typename std::enable_if<ArithmeticConcept<T1>() && ArithmeticConcept<T2>()>::type;

template <typename T>
static constexpr bool IntegralConcept() noexcept;

template <class T, class T2>
using _only_integer = typename std::enable_if<IntegralConcept<T>() && IntegralConcept<T2>()>::type;

// Unary operators
template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator~(const integer<Bits, Signed> & lhs) noexcept;

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator-(const integer<Bits, Signed> & lhs) noexcept(std::is_same_v<Signed, unsigned>);

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator+(const integer<Bits, Signed> & lhs) noexcept(std::is_same_v<Signed, unsigned>);

// Binary operators
template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator*(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator*(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator/(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator/(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator+(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator+(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator-(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator-(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator%(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Integral, typename Integral2, class = _only_integer<Integral, Integral2>>
std::common_type_t<Integral, Integral2> constexpr operator%(const Integral & rhs, const Integral2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator&(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Integral, typename Integral2, class = _only_integer<Integral, Integral2>>
std::common_type_t<Integral, Integral2> constexpr operator&(const Integral & rhs, const Integral2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator|(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Integral, typename Integral2, class = _only_integer<Integral, Integral2>>
std::common_type_t<Integral, Integral2> constexpr operator|(const Integral & rhs, const Integral2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator^(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Integral, typename Integral2, class = _only_integer<Integral, Integral2>>
std::common_type_t<Integral, Integral2> constexpr operator^(const Integral & rhs, const Integral2 & lhs);

// TODO: Integral
template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator<<(const integer<Bits, Signed> & lhs, int n) noexcept;

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator>>(const integer<Bits, Signed> & lhs, int n) noexcept;

template <size_t Bits, typename Signed, typename Int, std::enable_if_t<!std::is_same_v<Int, int>, int> = 0>
constexpr integer<Bits, Signed> operator<<(const integer<Bits, Signed> & lhs, Int n) noexcept
{
    return lhs << int(n);
}
template <size_t Bits, typename Signed, typename Int, std::enable_if_t<!std::is_same_v<Int, int>, int> = 0>
constexpr integer<Bits, Signed> operator>>(const integer<Bits, Signed> & lhs, Int n) noexcept
{
    return lhs >> int(n);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator<(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
constexpr bool operator<(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator>(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
constexpr bool operator>(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator<=(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
constexpr bool operator<=(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator>=(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
constexpr bool operator>=(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator==(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
constexpr bool operator==(const Arithmetic & rhs, const Arithmetic2 & lhs);

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator!=(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs);
template <typename Arithmetic, typename Arithmetic2, class = _only_arithmetic<Arithmetic, Arithmetic2>>
constexpr bool operator!=(const Arithmetic & rhs, const Arithmetic2 & lhs);

}

namespace std
{

template <size_t Bits, typename Signed>
struct hash<wide::integer<Bits, Signed>>;

}

// NOLINTEND(*)


[[noreturn]] inline void throwError(const char * err)
{
    throw std::runtime_error(err);
}


// NOLINTBEGIN(*)

/// Use same extended double for all platforms
#if (LDBL_MANT_DIG == 64)
#    define CONSTEXPR_FROM_DOUBLE constexpr
using FromDoubleIntermediateType = long double;
#else
#    include <boost/math/special_functions/fpclassify.hpp>
#    include <boost/multiprecision/cpp_bin_float.hpp>
/// `wide_integer_from_builtin` can't be constexpr with non-literal `cpp_bin_float_double_extended`
#    define CONSTEXPR_FROM_DOUBLE
using FromDoubleIntermediateType = boost::multiprecision::cpp_bin_float_double_extended;
#endif

namespace CityHash_v1_0_2
{
struct uint128;
}

namespace wide
{

constexpr bool supportsBitInt256()
{
#if defined(__clang__) && defined(__x86_64__)
    return true;
#else
    return false;
#endif
}

#if defined(__clang__) && defined(__x86_64__)
/// TODO C23 standardized _BitInt(N). Theoretically, it is not necessary to restrict the platform to x86.
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wbit-int-extension"
using BitInt256 = signed _BitInt(256);
using BitUInt256 = unsigned _BitInt(256);
#    pragma clang diagnostic pop

struct Error
{
};

template <typename Signed>
struct ConstructBitInt256
{
    using Type = Error;
};

template <>
struct ConstructBitInt256<signed>
{
    using Type = BitInt256;
};

template <>
struct ConstructBitInt256<unsigned>
{
    using Type = BitUInt256;
};

/// Converts a 256-bit wide integer to Clang's built-in 256-bit integer representation.
/// The source and target types have the same byte order.
template <size_t Bits, typename Signed, std::enable_if_t<Bits == 256, int> = 0>
constexpr const auto & toBitInt256(const wide::integer<Bits, Signed> & n)
{
    using T = ConstructBitInt256<Signed>::Type;
    return *reinterpret_cast<const T *>(&n);
}

/// Converts a Clang's built-in 256-bit integer representation to a 256-bit wide integer.
/// The source and target types have the same byte order.
template <typename T, std::enable_if_t<std::is_same_v<T, BitInt256> || std::is_same_v<T, BitUInt256>, int> = 0>
constexpr const auto & fromBitInt256(const T & n)
{
    using Signed = std::conditional_t<std::is_same_v<T, BitInt256>, signed, unsigned>;
    return *reinterpret_cast<const wide::integer<256, Signed> *>(&n);
}
#endif

template <typename T>
struct IsWideInteger
{
    static constexpr bool value = false;
};

template <size_t Bits, typename Signed>
struct IsWideInteger<wide::integer<Bits, Signed>>
{
    static constexpr bool value = true;
};

template <typename T>
static constexpr bool ArithmeticConcept() noexcept
{
    return std::is_arithmetic_v<T> || IsWideInteger<T>::value;
}

template <typename T>
static constexpr bool IntegralConcept() noexcept
{
    return std::is_integral_v<T> || IsWideInteger<T>::value;
}

template <typename T>
class IsTupleLike
{
    template <typename U>
    static auto check(U * p) -> decltype(std::tuple_size<U>::value, int());
    template <typename>
    static void check(...);

public:
    static constexpr const bool value = !std::is_void_v<decltype(check<T>(nullptr))>;
};

}

namespace std
{

// numeric limits
template <size_t Bits, typename Signed>
class numeric_limits<wide::integer<Bits, Signed>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = is_same_v<Signed, signed>;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = true;
    static constexpr bool has_denorm_loss = false;
    static constexpr std::float_round_style round_style = std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;
    static constexpr int digits = Bits - (is_same_v<Signed, signed> ? 1 : 0);
    static constexpr int digits10 = digits * 0.30103 /*std::log10(2)*/;
    static constexpr int max_digits10 = 0;
    static constexpr int radix = 2;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    static constexpr bool traps = true;
    static constexpr bool tinyness_before = false;

    static constexpr wide::integer<Bits, Signed> min() noexcept
    {
        if constexpr (is_same_v<Signed, signed>)
        {
            using T = wide::integer<Bits, signed>;
            T res{};
            res.items[T::_impl::big(0)] = std::numeric_limits<typename wide::integer<Bits, Signed>::signed_base_type>::min();
            return res;
        }
        return wide::integer<Bits, Signed>(0);
    }

    static constexpr wide::integer<Bits, Signed> max() noexcept
    {
        using T = wide::integer<Bits, Signed>;
        T res{};
        res.items[T::_impl::big(0)] = is_same_v<Signed, signed>
            ? std::numeric_limits<typename wide::integer<Bits, Signed>::signed_base_type>::max()
            : std::numeric_limits<typename wide::integer<Bits, Signed>::base_type>::max();
        for (unsigned i = 1; i < wide::integer<Bits, Signed>::_impl::item_count; ++i)
        {
            res.items[T::_impl::big(i)] = std::numeric_limits<typename wide::integer<Bits, Signed>::base_type>::max();
        }
        return res;
    }

    static constexpr wide::integer<Bits, Signed> lowest() noexcept { return min(); }
    static constexpr wide::integer<Bits, Signed> epsilon() noexcept { return 0; }
    static constexpr wide::integer<Bits, Signed> round_error() noexcept { return 0; }
    static constexpr wide::integer<Bits, Signed> infinity() noexcept { return 0; }
    static constexpr wide::integer<Bits, Signed> quiet_NaN() noexcept { return 0; }
    static constexpr wide::integer<Bits, Signed> signaling_NaN() noexcept { return 0; }
    static constexpr wide::integer<Bits, Signed> denorm_min() noexcept { return 0; }
};

// type traits
template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
struct common_type<wide::integer<Bits, Signed>, wide::integer<Bits2, Signed2>>
{
    using type = std::conditional_t < Bits == Bits2,
          wide::integer<Bits, std::conditional_t<(std::is_same_v<Signed, Signed2> && std::is_same_v<Signed2, signed>), signed, unsigned>>,
          std::conditional_t<Bits2<Bits, wide::integer<Bits, Signed>, wide::integer<Bits2, Signed2>>>;
};

template <size_t Bits, typename Signed, typename Arithmetic>
struct common_type<wide::integer<Bits, Signed>, Arithmetic>
{
    static_assert(wide::ArithmeticConcept<Arithmetic>());

    using type = std::conditional_t < std::is_floating_point_v<Arithmetic>, Arithmetic,
          std::conditional_t<
              sizeof(Arithmetic) * 8 < Bits,
              wide::integer<Bits, Signed>,
              std::conditional_t<Bits<
                  sizeof(Arithmetic) * 8,
                  Arithmetic,
                  std::conditional_t<
                      Bits == sizeof(Arithmetic) * 8 && (std::is_same_v<Signed, signed> || std::is_signed_v<Arithmetic>),
                      Arithmetic,
                      wide::integer<Bits, Signed>>>>>;
};

template <typename Arithmetic, size_t Bits, typename Signed>
struct common_type<Arithmetic, wide::integer<Bits, Signed>> : common_type<wide::integer<Bits, Signed>, Arithmetic>
{
};

}

#pragma clang attribute push(__attribute__((no_sanitize("undefined"))), apply_to = function)
namespace wide
{

template <size_t Bits, typename Signed>
struct integer<Bits, Signed>::_impl
{
    static constexpr size_t _bits = Bits;
    static constexpr const unsigned byte_count = Bits / 8;
    static constexpr const unsigned item_count = byte_count / sizeof(base_type);
    static constexpr const unsigned base_bits = sizeof(base_type) * 8;

    /// Use Clang's built-in 256-bit integer to improve performance if possible.
    ///
    /// Not implemented for 128 bit types because performance benefits are negligible as of 2025:
    /// https://github.com/ClickHouse/ClickHouse/issues/70502
    static constexpr bool use_BitInt256 = supportsBitInt256() && Bits == 256;

    static_assert(Bits % base_bits == 0);

    /// Simple iteration in both directions
    static constexpr unsigned little(unsigned idx)
    {
        if constexpr (std::endian::native == std::endian::little)
            return idx;
        else
            return item_count - 1 - idx;
    }
    static constexpr unsigned big(unsigned idx)
    {
        if constexpr (std::endian::native == std::endian::little)
            return item_count - 1 - idx;
        else
            return idx;
    }
    static constexpr unsigned any(unsigned idx) { return idx; }

    template <class T>
    constexpr static bool is_negative(const T & n) noexcept
    {
        if constexpr (std::is_signed_v<T>)
            return n < 0;
        else
            return false;
    }

    template <size_t B, class T>
    constexpr static bool is_negative(const integer<B, T> & n) noexcept
    {
        if constexpr (std::is_same_v<T, signed>)
            return static_cast<signed_base_type>(n.items[integer<B, T>::_impl::big(0)]) < 0;
        else
            return false;
    }

    template <typename T>
    constexpr static auto make_positive(const T & n) noexcept
    {
        if constexpr (std::is_signed_v<T>)
            return n < 0 ? -n : n;
        else
            return n;
    }

    template <size_t B, class S>
    constexpr static integer<B, S> make_positive(const integer<B, S> & n) noexcept
    {
        return is_negative(n) ? integer<B, S>(operator_unary_minus(n)) : n;
    }

    template <typename T>
    constexpr static auto to_Integral(T f) noexcept
    {
        /// NOTE: this can be called with DB::Decimal, and in this case, result
        /// will be wrong
        if constexpr (std::is_signed_v<T>)
            return static_cast<int64_t>(f);
        else
            return static_cast<uint64_t>(f);
    }

    template <typename Integral>
    constexpr static void wide_integer_from_builtin(integer<Bits, Signed> & self, Integral rhs) noexcept
    {
        static_assert(sizeof(Integral) <= sizeof(base_type));

        self.items[little(0)] = _impl::to_Integral(rhs);

        if constexpr (std::is_signed_v<Integral>)
        {
            if (rhs < 0)
            {
                for (unsigned i = 1; i < item_count; ++i)
                    self.items[little(i)] = -1;
                return;
            }
        }

        for (unsigned i = 1; i < item_count; ++i)
            self.items[little(i)] = 0;
    }

    template <typename TupleLike, size_t i = 0>
    constexpr static void wide_integer_from_tuple_like(integer<Bits, Signed> & self, const TupleLike & tuple) noexcept
    {
        if constexpr (i < item_count)
        {
            if constexpr (i < std::tuple_size_v<TupleLike>)
                self.items[i] = std::get<i>(tuple);
            else
                self.items[i] = 0;
            wide_integer_from_tuple_like<TupleLike, i + 1>(self, tuple);
        }
    }

    template <typename CityHashUInt128 = CityHash_v1_0_2::uint128>
    constexpr static void wide_integer_from_cityhash_uint128(integer<Bits, Signed> & self, const CityHashUInt128 & value) noexcept
    {
        static_assert(sizeof(item_count) >= 2);

        if constexpr (std::endian::native == std::endian::little)
            wide_integer_from_tuple_like(self, std::make_pair(value.low64, value.high64));
        else
            wide_integer_from_tuple_like(self, std::make_pair(value.high64, value.low64));
    }

    /**
     * N.B. t is constructed from double, so max(t) = max(double) ~ 2^310
     * the recursive call happens when t / 2^64 > 2^64, so there won't be more than 5 of them.
     *
     * t = a1 * max_int + b1,   a1 > max_int, b1 < max_int
     * a1 = a2 * max_int + b2,  a2 > max_int, b2 < max_int
     * a_(n - 1) = a_n * max_int + b2, a_n <= max_int <- base case.
     */
    template <class T>
    constexpr static void set_multiplier(integer<Bits, Signed> & self, T t) noexcept
    {
        constexpr uint64_t max_int = std::numeric_limits<uint64_t>::max();
        static_assert(std::is_same_v<T, double> || std::is_same_v<T, FromDoubleIntermediateType>);
        /// Implementation specific behaviour on overflow (if we don't check here, stack overflow will triggered in bigint_cast).
#if (LDBL_MANT_DIG == 64)
        if (!std::isfinite(t))
        {
            self = 0;
            return;
        }
#else
        if constexpr (std::is_same_v<T, double>)
        {
            if (!std::isfinite(t))
            {
                self = 0;
                return;
            }
        }
        else
        {
            if (!boost::math::isfinite(t))
            {
                self = 0;
                return;
            }
        }
#endif

        const T alpha = t / static_cast<T>(max_int);

        /** Here we have to use strict comparison.
          * The max_int is 2^64 - 1.
          * When cast to a floating point type, it will be rounded to the closest representable number,
          * which is 2^64.
          * But 2^64 is not representable in uint64_t,
          * so the maximum representable number will be strictly less.
          */
        if (alpha < static_cast<T>(max_int))
            self = static_cast<uint64_t>(alpha);
        else // max(double) / 2^64 will surely contain less than 52 precision bits, so speed up computations.
            set_multiplier<double>(self, static_cast<double>(alpha));

        self *= max_int;
        self += static_cast<uint64_t>(t - floor(alpha) * static_cast<T>(max_int)); // += b_i
    }

    CONSTEXPR_FROM_DOUBLE static void wide_integer_from_builtin(integer<Bits, Signed> & self, double rhs) noexcept
    {
        constexpr int64_t max_int = std::numeric_limits<int64_t>::max();
        constexpr int64_t min_int = std::numeric_limits<int64_t>::lowest();

        /// There are values in int64 that have more than 53 significant bits (in terms of double
        /// representation). Such values, being promoted to double, are rounded up or down. If they are rounded up,
        /// the result may not fit in 64 bits.
        /// The example of such a number is 9.22337e+18.
        /// As to_Integral does a static_cast to int64_t, it may result in UB.
        /// The necessary check here is that FromDoubleIntermediateType has enough significant (mantissa) bits to store the
        /// int64_t max value precisely.

        if (rhs > static_cast<FromDoubleIntermediateType>(min_int) && rhs < static_cast<FromDoubleIntermediateType>(max_int))
        {
            self = static_cast<int64_t>(rhs);
            return;
        }

        const FromDoubleIntermediateType rhs_long_double
            = (static_cast<FromDoubleIntermediateType>(rhs) < 0) ? -static_cast<FromDoubleIntermediateType>(rhs) : rhs;

        set_multiplier(self, rhs_long_double);

        if (rhs < 0)
            self = -self;
    }

    template <size_t Bits2, typename Signed2>
    constexpr static void wide_integer_from_wide_integer(integer<Bits, Signed> & self, const integer<Bits2, Signed2> & rhs) noexcept
    {
        constexpr const unsigned min_bits = (Bits < Bits2) ? Bits : Bits2;
        constexpr const unsigned to_copy = min_bits / base_bits;

        for (unsigned i = 0; i < to_copy; ++i)
            self.items[little(i)] = rhs.items[integer<Bits2, Signed2>::_impl::little(i)];

        if constexpr (Bits > Bits2)
        {
            if constexpr (std::is_signed_v<Signed2>)
            {
                if (rhs < 0)
                {
                    for (unsigned i = to_copy; i < item_count; ++i)
                        self.items[little(i)] = -1;
                    return;
                }
            }

            for (unsigned i = to_copy; i < item_count; ++i)
                self.items[little(i)] = 0;
        }
    }

    template <typename T>
    constexpr static bool should_keep_size()
    {
        return sizeof(T) <= byte_count;
    }

    constexpr static integer<Bits, Signed> shift_left(const integer<Bits, Signed> & rhs, unsigned n) noexcept
    {
        integer<Bits, Signed> lhs;
        unsigned items_shift = n / base_bits;

        if (unsigned bit_shift = n % base_bits)
        {
            unsigned overflow_shift = base_bits - bit_shift;

            lhs.items[big(0)] = rhs.items[big(items_shift)] << bit_shift;
            for (unsigned i = 1; i < item_count - items_shift; ++i)
            {
                lhs.items[big(i - 1)] |= rhs.items[big(items_shift + i)] >> overflow_shift;
                lhs.items[big(i)] = rhs.items[big(items_shift + i)] << bit_shift;
            }
        }
        else
        {
            for (unsigned i = 0; i < item_count - items_shift; ++i)
                lhs.items[big(i)] = rhs.items[big(items_shift + i)];
        }

        for (unsigned i = 0; i < items_shift; ++i)
            lhs.items[little(i)] = 0;
        return lhs;
    }

    constexpr static integer<Bits, Signed> shift_right(const integer<Bits, Signed> & rhs, unsigned n) noexcept
    {
        integer<Bits, Signed> lhs;
        unsigned items_shift = n / base_bits;
        unsigned bit_shift = n % base_bits;

        if (bit_shift)
        {
            unsigned overflow_shift = base_bits - bit_shift;

            lhs.items[little(0)] = rhs.items[little(items_shift)] >> bit_shift;
            for (unsigned i = 1; i < item_count - items_shift; ++i)
            {
                lhs.items[little(i - 1)] |= rhs.items[little(items_shift + i)] << overflow_shift;
                lhs.items[little(i)] = rhs.items[little(items_shift + i)] >> bit_shift;
            }
        }
        else
        {
            for (unsigned i = 0; i < item_count - items_shift; ++i)
                lhs.items[little(i)] = rhs.items[little(items_shift + i)];
        }

        if (is_negative(rhs))
        {
            if (bit_shift)
                lhs.items[big(items_shift)] |= std::numeric_limits<base_type>::max() << (base_bits - bit_shift);

            for (unsigned i = 0; i < items_shift; ++i)
                lhs.items[big(i)] = std::numeric_limits<base_type>::max();
        }
        else
        {
            for (unsigned i = 0; i < items_shift; ++i)
                lhs.items[big(i)] = 0;
        }

        return lhs;
    }

private:
    template <typename T>
    constexpr static base_type get_item(const T & x, unsigned idx)
    {
        if constexpr (IsWideInteger<T>::value)
        {
            if (idx < T::_impl::item_count)
                return x.items[idx];
            return 0;
        }
        else
        {
            if constexpr (sizeof(T) <= sizeof(base_type))
            {
                if (little(0) == idx)
                    return static_cast<base_type>(x);
            }
            else if (idx * sizeof(base_type) < sizeof(T))
                return x >> (idx * base_bits); // & std::numeric_limits<base_type>::max()
            return 0;
        }
    }

    template <typename T>
    constexpr static integer<Bits, Signed> minus(const integer<Bits, Signed> & lhs, T rhs)
    {
        constexpr const unsigned rhs_items = (sizeof(T) > sizeof(base_type)) ? (sizeof(T) / sizeof(base_type)) : 1;
        constexpr const unsigned op_items = (item_count < rhs_items) ? item_count : rhs_items;

        integer<Bits, Signed> res(lhs);
        bool underflows[item_count] = {};

        for (unsigned i = 0; i < op_items; ++i)
        {
            base_type rhs_item = get_item(rhs, little(i));
            base_type & res_item = res.items[little(i)];

            underflows[i] = res_item < rhs_item;
            res_item -= rhs_item;
        }

        for (unsigned i = 1; i < item_count; ++i)
        {
            if (underflows[i - 1])
            {
                base_type & res_item = res.items[little(i)];
                if (res_item == 0)
                    underflows[i] = true;
                --res_item;
            }
        }

        return res;
    }

    template <typename T>
    constexpr static integer<Bits, Signed> plus(const integer<Bits, Signed> & lhs, T rhs)
    {
        constexpr const unsigned rhs_items = (sizeof(T) > sizeof(base_type)) ? (sizeof(T) / sizeof(base_type)) : 1;
        constexpr const unsigned op_items = (item_count < rhs_items) ? item_count : rhs_items;

        integer<Bits, Signed> res(lhs);
        bool overflows[item_count] = {};

        for (unsigned i = 0; i < op_items; ++i)
        {
            base_type rhs_item = get_item(rhs, little(i));
            base_type & res_item = res.items[little(i)];

            res_item += rhs_item;
            overflows[i] = res_item < rhs_item;
        }

        for (unsigned i = 1; i < item_count; ++i)
        {
            if (overflows[i - 1])
            {
                base_type & res_item = res.items[little(i)];
                ++res_item;
                if (res_item == 0)
                    overflows[i] = true;
            }
        }

        return res;
    }

    template <typename T>
    constexpr static integer<Bits, Signed> multiply(const integer<Bits, Signed> & lhs, const T & rhs)
    {
        if constexpr (Bits == 256 && sizeof(base_type) == 8)
        {
            /// @sa https://github.com/abseil/abseil-cpp/blob/master/absl/numeric/int128.h
            using HalfType = unsigned __int128;

            HalfType a01 = (HalfType(lhs.items[little(1)]) << 64) + lhs.items[little(0)];
            HalfType a23 = (HalfType(lhs.items[little(3)]) << 64) + lhs.items[little(2)];
            HalfType a0 = lhs.items[little(0)];
            HalfType a1 = lhs.items[little(1)];

            HalfType b01 = rhs;
            uint64_t b0 = b01;
            uint64_t b1 = 0;
            HalfType b23 = 0;
            if constexpr (sizeof(T) > 8)
                b1 = b01 >> 64;
            if constexpr (sizeof(T) > 16)
                b23 = (HalfType(rhs.items[little(3)]) << 64) + rhs.items[little(2)];

            HalfType r23 = a23 * b01 + a01 * b23 + a1 * b1;
            HalfType r01 = a0 * b0;
            HalfType r12 = (r01 >> 64) + (r23 << 64);
            HalfType r12_x = a1 * b0;

            integer<Bits, Signed> res;
            res.items[little(0)] = r01;
            res.items[little(3)] = r23 >> 64;

            if constexpr (sizeof(T) > 8)
            {
                HalfType r12_y = a0 * b1;
                r12_x += r12_y;
                if (r12_x < r12_y)
                    ++res.items[little(3)];
            }

            r12 += r12_x;
            if (r12 < r12_x)
                ++res.items[little(3)];

            res.items[little(1)] = r12;
            res.items[little(2)] = r12 >> 64;
            return res;
        }
        else if constexpr (Bits == 128 && sizeof(base_type) == 8)
        {
            using CompilerUInt128 = unsigned __int128;
            CompilerUInt128 a = (CompilerUInt128(lhs.items[little(1)]) << 64)
                + lhs.items[little(0)]; // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
            CompilerUInt128 b = (CompilerUInt128(rhs.items[little(1)]) << 64)
                + rhs.items[little(0)]; // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
            CompilerUInt128 c = a * b;
            integer<Bits, Signed> res;
            res.items[little(0)] = c;
            res.items[little(1)] = c >> 64;
            return res;
        }
        else
        {
            integer<Bits, Signed> res{};
#if 1
            integer<Bits, Signed> lhs2 = plus(lhs, shift_left(lhs, 1));
            integer<Bits, Signed> lhs3 = plus(lhs2, shift_left(lhs, 2));
#endif
            for (unsigned i = 0; i < item_count; ++i)
            {
                base_type rhs_item = get_item(rhs, little(i));
                unsigned pos = i * base_bits;

                while (rhs_item)
                {
#if 1 /// optimization
                    if ((rhs_item & 0x7) == 0x7)
                    {
                        res = plus(res, shift_left(lhs3, pos));
                        rhs_item >>= 3;
                        pos += 3;
                        continue;
                    }

                    if ((rhs_item & 0x3) == 0x3)
                    {
                        res = plus(res, shift_left(lhs2, pos));
                        rhs_item >>= 2;
                        pos += 2;
                        continue;
                    }
#endif
                    if (rhs_item & 1)
                        res = plus(res, shift_left(lhs, pos));

                    rhs_item >>= 1;
                    ++pos;
                }
            }

            return res;
        }
    }

public:
    constexpr static integer<Bits, Signed> operator_unary_tilda(const integer<Bits, Signed> & lhs) noexcept
    {
        integer<Bits, Signed> res;

        for (unsigned i = 0; i < item_count; ++i)
            res.items[any(i)] = ~lhs.items[any(i)];
        return res;
    }

    constexpr static integer<Bits, Signed>
    operator_unary_minus(const integer<Bits, Signed> & lhs) noexcept(std::is_same_v<Signed, unsigned>)
    {
        return plus(operator_unary_tilda(lhs), 1);
    }

    template <typename T>
    constexpr static auto operator_plus(const integer<Bits, Signed> & lhs, const T & rhs) noexcept(std::is_same_v<Signed, unsigned>)
    {
        if constexpr (should_keep_size<T>())
        {
            if constexpr (use_BitInt256)
            {
                if constexpr (!std::is_same_v<T, integer<Bits, Signed>>)
                {
                    auto new_rhs = static_cast<integer<Bits, Signed>>(rhs);
                    return fromBitInt256(toBitInt256(lhs) + toBitInt256(new_rhs));
                }
                else
                    return fromBitInt256(toBitInt256(lhs) + toBitInt256(rhs));
            }
            else
            {
                if (is_negative(rhs))
                    return minus(lhs, -rhs);
                else
                    return plus(lhs, rhs);
            }
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, integer<T::_impl::_bits, Signed>>::_impl::operator_plus(
                integer<T::_impl::_bits, Signed>(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static auto operator_minus(const integer<Bits, Signed> & lhs, const T & rhs) noexcept(std::is_same_v<Signed, unsigned>)
    {
        if constexpr (should_keep_size<T>())
        {
            if constexpr (use_BitInt256)
            {
                if constexpr (!std::is_same_v<T, integer<Bits, Signed>>)
                {
                    auto new_rhs = static_cast<integer<Bits, Signed>>(rhs);
                    return fromBitInt256(toBitInt256(lhs) - toBitInt256(new_rhs));
                }
                else
                    return fromBitInt256(toBitInt256(lhs) - toBitInt256(rhs));
            }
            else
            {
                if (is_negative(rhs))
                    return plus(lhs, -rhs);
                else
                    return minus(lhs, rhs);
            }
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, integer<T::_impl::_bits, Signed>>::_impl::operator_minus(
                integer<T::_impl::_bits, Signed>(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static auto operator_star(const integer<Bits, Signed> & lhs, const T & rhs)
    {
        if constexpr (should_keep_size<T>())
        {
            if constexpr (use_BitInt256)
            {
                if constexpr (!std::is_same_v<T, integer<Bits, Signed>>)
                {
                    auto new_rhs = static_cast<integer<Bits, Signed>>(rhs);
                    return fromBitInt256(toBitInt256(lhs) * toBitInt256(new_rhs));
                }
                else
                    return fromBitInt256(toBitInt256(lhs) * toBitInt256(rhs));
            }
            else
            {
                integer<Bits, Signed> res;

                if constexpr (std::is_signed_v<Signed>)
                {
                    res = multiply((is_negative(lhs) ? make_positive(lhs) : lhs), (is_negative(rhs) ? make_positive(rhs) : rhs));
                }
                else
                {
                    res = multiply(lhs, (is_negative(rhs) ? make_positive(rhs) : rhs));
                }

                if (std::is_same_v<Signed, signed> && is_negative(lhs) != is_negative(rhs))
                    res = operator_unary_minus(res);

                return res;
            }
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, T>::_impl::operator_star(T(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static bool operator_greater(const integer<Bits, Signed> & lhs, const T & rhs) noexcept
    {
        if constexpr (should_keep_size<T>())
        {
            if (std::numeric_limits<T>::is_signed && (is_negative(lhs) != is_negative(rhs)))
                return is_negative(rhs);

            integer<Bits, Signed> t = rhs;
            for (unsigned i = 0; i < item_count; ++i)
            {
                base_type rhs_item = get_item(t, big(i));

                if (lhs.items[big(i)] != rhs_item)
                    return lhs.items[big(i)] > rhs_item;
            }

            return false;
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, T>::_impl::operator_greater(T(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static bool operator_less(const integer<Bits, Signed> & lhs, const T & rhs) noexcept
    {
        if constexpr (should_keep_size<T>())
        {
            if (std::numeric_limits<T>::is_signed && (is_negative(lhs) != is_negative(rhs)))
                return is_negative(lhs);

            integer<Bits, Signed> t = rhs;
            for (unsigned i = 0; i < item_count; ++i)
            {
                base_type rhs_item = get_item(t, big(i));

                if (lhs.items[big(i)] != rhs_item)
                    return lhs.items[big(i)] < rhs_item;
            }

            return false;
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, T>::_impl::operator_less(T(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static bool operator_eq(const integer<Bits, Signed> & lhs, const T & rhs) noexcept
    {
        if constexpr (should_keep_size<T>())
        {
            integer<Bits, Signed> t = rhs;
            for (unsigned i = 0; i < item_count; ++i)
            {
                base_type rhs_item = get_item(t, any(i));

                if (lhs.items[any(i)] != rhs_item)
                    return false;
            }

            return true;
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, T>::_impl::operator_eq(T(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static auto operator_pipe(const integer<Bits, Signed> & lhs, const T & rhs) noexcept
    {
        if constexpr (should_keep_size<T>())
        {
            integer<Bits, Signed> res;

            for (unsigned i = 0; i < item_count; ++i)
                res.items[little(i)] = lhs.items[little(i)] | get_item(rhs, little(i));
            return res;
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, T>::_impl::operator_pipe(T(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static auto operator_amp(const integer<Bits, Signed> & lhs, const T & rhs) noexcept
    {
        if constexpr (should_keep_size<T>())
        {
            integer<Bits, Signed> res;

            for (unsigned i = 0; i < item_count; ++i)
                res.items[little(i)] = lhs.items[little(i)] & get_item(rhs, little(i));
            return res;
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, T>::_impl::operator_amp(T(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static bool is_zero(const T & x)
    {
        bool is_zero = true;
        for (auto item : x.items)
        {
            if (item != 0)
            {
                is_zero = false;
                break;
            }
        }
        return is_zero;
    }

    /// returns quotient as result and remainder in numerator.
    template <size_t Bits2>
    constexpr static integer<Bits2, unsigned> divide(integer<Bits2, unsigned> & numerator, integer<Bits2, unsigned> denominator)
    {
        static_assert(std::is_unsigned_v<Signed>);

        if (is_zero(denominator))
            throwError("Division by zero");

        if constexpr (Bits == 128 && sizeof(base_type) == 8)
        {
            using CompilerUInt128 = unsigned __int128;

            CompilerUInt128 a = (CompilerUInt128(numerator.items[little(1)]) << 64)
                + numerator.items[little(0)]; // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
            CompilerUInt128 b = (CompilerUInt128(denominator.items[little(1)]) << 64)
                + denominator.items[little(0)]; // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
            CompilerUInt128 c = a / b; // NOLINT

            integer<Bits, Signed> res;
            res.items[little(0)] = c;
            res.items[little(1)] = c >> 64;

            CompilerUInt128 remainder = a - b * c;
            numerator.items[little(0)] = remainder;
            numerator.items[little(1)] = remainder >> 64;

            return res;
        }

        integer<Bits2, unsigned> x = 1;
        integer<Bits2, unsigned> quotient = 0;

        while (!operator_greater(denominator, numerator) && is_zero(operator_amp(shift_right(denominator, Bits2 - 1), 1)))
        {
            x = shift_left(x, 1);
            denominator = shift_left(denominator, 1);
        }

        while (!is_zero(x))
        {
            if (!operator_greater(denominator, numerator))
            {
                numerator = operator_minus(numerator, denominator);
                quotient = operator_pipe(quotient, x);
            }

            x = shift_right(x, 1);
            denominator = shift_right(denominator, 1);
        }

        return quotient;
    }

    template <typename T>
    constexpr static auto operator_slash(const integer<Bits, Signed> & lhs, const T & rhs)
    {
        if constexpr (should_keep_size<T>())
        {
            if constexpr (use_BitInt256)
            {
                if constexpr (!std::is_same_v<T, integer<Bits, Signed>>)
                {
                    auto new_rhs = static_cast<integer<Bits, Signed>>(rhs);
                    return fromBitInt256(toBitInt256(lhs) / toBitInt256(new_rhs));
                }
                else
                    return fromBitInt256(toBitInt256(lhs) / toBitInt256(rhs));
            }
            else
            {
                integer<Bits, unsigned> numerator = make_positive(lhs);
                integer<Bits, unsigned> denominator = make_positive(integer<Bits, Signed>(rhs));
                integer<Bits, unsigned> quotient = integer<Bits, unsigned>::_impl::divide(numerator, std::move(denominator));

                if (std::is_same_v<Signed, signed> && is_negative(rhs) != is_negative(lhs))
                    quotient = operator_unary_minus(quotient);
                return quotient;
            }
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, integer<T::_impl::_bits, Signed>>::operator_slash(T(lhs), rhs);
        }
    }

    template <typename T>
    constexpr static auto operator_percent(const integer<Bits, Signed> & lhs, const T & rhs)
    {
        if constexpr (should_keep_size<T>())
        {
            if constexpr (use_BitInt256)
            {
                if constexpr (!std::is_same_v<T, integer<Bits, signed>>)
                {
                    auto new_rhs = static_cast<integer<Bits, Signed>>(rhs);
                    return fromBitInt256(toBitInt256(lhs) % toBitInt256(new_rhs));
                }
                else
                    return fromBitInt256(toBitInt256(lhs) % toBitInt256(rhs));
            }
            else
            {
                integer<Bits, unsigned> remainder = make_positive(lhs);
                integer<Bits, unsigned> denominator = make_positive(integer<Bits, Signed>(rhs));
                integer<Bits, unsigned>::_impl::divide(remainder, std::move(denominator));

                if (std::is_same_v<Signed, signed> && is_negative(lhs))
                    remainder = operator_unary_minus(remainder);
                return remainder;
            }
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return std::common_type_t<integer<Bits, Signed>, integer<T::_impl::_bits, Signed>>::operator_percent(T(lhs), rhs);
        }
    }

    // ^
    template <typename T>
    constexpr static auto operator_circumflex(const integer<Bits, Signed> & lhs, const T & rhs) noexcept
    {
        if constexpr (should_keep_size<T>())
        {
            integer<Bits, Signed> t(rhs);
            integer<Bits, Signed> res = lhs;

            for (unsigned i = 0; i < item_count; ++i)
                res.items[any(i)] ^= t.items[any(i)];
            return res;
        }
        else
        {
            static_assert(IsWideInteger<T>::value);
            return T::operator_circumflex(T(lhs), rhs);
        }
    }

    constexpr static integer<Bits, Signed> from_str(const char * c)
    {
        integer<Bits, Signed> res = 0;

        bool is_neg = std::is_same_v<Signed, signed> && *c == '-';
        if (is_neg)
            ++c;

        if (*c == '0' && (*(c + 1) == 'x' || *(c + 1) == 'X'))
        { // hex
            ++c;
            ++c;
            while (*c)
            {
                if (*c >= '0' && *c <= '9')
                {
                    res = multiply(res, 16U);
                    res = plus(res, *c - '0');
                    ++c;
                }
                else if (*c >= 'a' && *c <= 'f')
                {
                    res = multiply(res, 16U);
                    res = plus(res, *c - 'a' + 10U);
                    ++c;
                }
                else if (*c >= 'A' && *c <= 'F')
                { // tolower must be used, but it is not constexpr
                    res = multiply(res, 16U);
                    res = plus(res, *c - 'A' + 10U);
                    ++c;
                }
                else
                    throwError("Invalid char from");
            }
        }
        else
        { // dec
            while (*c)
            {
                if (*c < '0' || *c > '9')
                    throwError("Invalid char from");

                res = multiply(res, 10U);
                res = plus(res, *c - '0');
                ++c;
            }
        }

        if (is_neg)
            res = operator_unary_minus(res);

        return res;
    }
};

// Members

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed>::integer(T rhs) noexcept
    : items{}
{
    if constexpr (IsWideInteger<T>::value)
        _impl::wide_integer_from_wide_integer(*this, rhs);
    else if constexpr (IsTupleLike<T>::value)
        _impl::wide_integer_from_tuple_like(*this, rhs);
    else if constexpr (std::is_same_v<std::remove_cvref_t<T>, CityHash_v1_0_2::uint128>)
        _impl::wide_integer_from_cityhash_uint128(*this, rhs);
    else
        _impl::wide_integer_from_builtin(*this, rhs);
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed>::integer(std::initializer_list<T> il) noexcept
    : items{}
{
    if (il.size() == 1)
    {
        if constexpr (IsWideInteger<T>::value)
            _impl::wide_integer_from_wide_integer(*this, *il.begin());
        else if constexpr (IsTupleLike<T>::value)
            _impl::wide_integer_from_tuple_like(*this, *il.begin());
        else if constexpr (std::is_same_v<std::remove_cvref_t<T>, CityHash_v1_0_2::uint128>)
            _impl::wide_integer_from_cityhash_uint128(*this, *il.begin());
        else
            _impl::wide_integer_from_builtin(*this, *il.begin());
    }
    else if (il.size() == 0)
    {
        _impl::wide_integer_from_builtin(*this, 0);
    }
    else
    {
        auto it = il.begin();
        for (unsigned i = 0; i < _impl::item_count; ++i)
        {
            if (it < il.end())
            {
                items[_impl::little(i)] = *it;
                ++it;
            }
            else
                items[_impl::little(i)] = 0;
        }
    }
}

template <size_t Bits, typename Signed>
template <size_t Bits2, typename Signed2>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator=(const integer<Bits2, Signed2> & rhs) noexcept
{
    _impl::wide_integer_from_wide_integer(*this, rhs);
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator=(T rhs) noexcept
{
    if constexpr (IsTupleLike<T>::value)
        _impl::wide_integer_from_tuple_like(*this, rhs);
    else if constexpr (std::is_same_v<std::remove_cvref_t<T>, CityHash_v1_0_2::uint128>)
        _impl::wide_integer_from_cityhash_uint128(*this, rhs);
    else
        _impl::wide_integer_from_builtin(*this, rhs);
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator*=(const T & rhs)
{
    *this = *this * rhs;
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator/=(const T & rhs)
{
    *this = *this / rhs;
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator+=(const T & rhs) noexcept(std::is_same_v<Signed, unsigned>)
{
    *this = *this + rhs;
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator-=(const T & rhs) noexcept(std::is_same_v<Signed, unsigned>)
{
    *this = *this - rhs;
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator%=(const T & rhs)
{
    *this = *this % rhs;
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator&=(const T & rhs) noexcept
{
    *this = *this & rhs;
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator|=(const T & rhs) noexcept
{
    *this = *this | rhs;
    return *this;
}

template <size_t Bits, typename Signed>
template <typename T>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator^=(const T & rhs) noexcept
{
    *this = *this ^ rhs;
    return *this;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator<<=(int n) noexcept
{
    if (static_cast<size_t>(n) >= Bits)
        *this = 0;
    else if (n > 0)
        *this = _impl::shift_left(*this, n);
    return *this;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator>>=(int n) noexcept
{
    if (static_cast<size_t>(n) >= Bits)
    {
        if (_impl::is_negative(*this))
            *this = -1;
        else
            *this = 0;
    }
    else if (n > 0)
        *this = _impl::shift_right(*this, n);
    return *this;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator++() noexcept(std::is_same_v<Signed, unsigned>)
{
    *this = _impl::operator_plus(*this, 1);
    return *this;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> integer<Bits, Signed>::operator++(int) noexcept(std::is_same_v<Signed, unsigned>)
{
    auto tmp = *this;
    *this = _impl::operator_plus(*this, 1);
    return tmp;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> & integer<Bits, Signed>::operator--() noexcept(std::is_same_v<Signed, unsigned>)
{
    *this = _impl::operator_minus(*this, 1);
    return *this;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> integer<Bits, Signed>::operator--(int) noexcept(std::is_same_v<Signed, unsigned>)
{
    auto tmp = *this;
    *this = _impl::operator_minus(*this, 1);
    return tmp;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed>::operator bool() const noexcept
{
    return !_impl::operator_eq(*this, 0);
}

template <size_t Bits, typename Signed>
template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int>>
constexpr integer<Bits, Signed>::operator T() const noexcept
{
    static_assert(std::numeric_limits<T>::is_integer);

    /// NOTE: memcpy will suffice, but unfortunately, this function is constexpr.

    using UnsignedT = std::make_unsigned_t<T>;

    UnsignedT res{};
    for (unsigned i = 0; i < _impl::item_count && i < (sizeof(T) + sizeof(base_type) - 1) / sizeof(base_type); ++i)
        res += UnsignedT(items[_impl::little(i)])
            << (sizeof(base_type) * 8 * i); // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)

    return res;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed>::operator long double() const noexcept
{
    if (_impl::operator_eq(*this, 0))
        return 0;

    integer<Bits, Signed> tmp = *this;
    if (_impl::is_negative(*this))
        tmp = -tmp;

    long double res = 0;
    for (unsigned i = 0; i < _impl::item_count; ++i)
    {
        long double t = res;
        res *= static_cast<long double>(std::numeric_limits<base_type>::max());
        res += t;
        res += tmp.items[_impl::big(i)];
    }

    if (_impl::is_negative(*this))
        res = -res;

    return res;
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed>::operator double() const noexcept
{
    return static_cast<double>(static_cast<long double>(*this));
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed>::operator float() const noexcept
{
    return static_cast<float>(static_cast<long double>(*this));
}

// Unary operators
template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator~(const integer<Bits, Signed> & lhs) noexcept
{
    return integer<Bits, Signed>::_impl::operator_unary_tilda(lhs);
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator-(const integer<Bits, Signed> & lhs) noexcept(std::is_same_v<Signed, unsigned>)
{
    return integer<Bits, Signed>::_impl::operator_unary_minus(lhs);
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator+(const integer<Bits, Signed> & lhs) noexcept(std::is_same_v<Signed, unsigned>)
{
    return lhs;
}

#define CT(x) \
    std::common_type_t<std::decay_t<decltype(rhs)>, std::decay_t<decltype(lhs)>> \
    { \
        x \
    }

// Binary operators
template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator*(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_star(lhs, rhs);
}

template <typename Arithmetic, typename Arithmetic2, class>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator*(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) * CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator/(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_slash(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator/(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) / CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator+(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_plus(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator+(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) + CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator-(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_minus(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator-(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) - CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator%(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_percent(lhs, rhs);
}
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator%(const Integral & lhs, const Integral2 & rhs)
{
    return CT(lhs) % CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator&(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_amp(lhs, rhs);
}
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator&(const Integral & lhs, const Integral2 & rhs)
{
    return CT(lhs) & CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator|(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_pipe(lhs, rhs);
}
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator|(const Integral & lhs, const Integral2 & rhs)
{
    return CT(lhs) | CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>> constexpr
operator^(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_circumflex(lhs, rhs);
}
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator^(const Integral & lhs, const Integral2 & rhs)
{
    return CT(lhs) ^ CT(rhs);
}

template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator<<(const integer<Bits, Signed> & lhs, int n) noexcept
{
    if (static_cast<size_t>(n) >= Bits)
        return integer<Bits, Signed>(0);
    if (n <= 0)
        return lhs;
    return integer<Bits, Signed>::_impl::shift_left(lhs, n);
}
template <size_t Bits, typename Signed>
constexpr integer<Bits, Signed> operator>>(const integer<Bits, Signed> & lhs, int n) noexcept
{
    if (static_cast<size_t>(n) >= Bits)
        return integer<Bits, Signed>(0);
    if (n <= 0)
        return lhs;
    return integer<Bits, Signed>::_impl::shift_right(lhs, n);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator<(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_less(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
constexpr bool operator<(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) < CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator>(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_greater(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
constexpr bool operator>(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) > CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator<=(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_less(lhs, rhs)
        || std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_eq(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
constexpr bool operator<=(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) <= CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator>=(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_greater(lhs, rhs)
        || std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_eq(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
constexpr bool operator>=(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) >= CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator==(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_eq(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
constexpr bool operator==(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) == CT(rhs);
}

template <size_t Bits, typename Signed, size_t Bits2, typename Signed2>
constexpr bool operator!=(const integer<Bits, Signed> & lhs, const integer<Bits2, Signed2> & rhs)
{
    return !std::common_type_t<integer<Bits, Signed>, integer<Bits2, Signed2>>::_impl::operator_eq(lhs, rhs);
}
template <typename Arithmetic, typename Arithmetic2, class>
constexpr bool operator!=(const Arithmetic & lhs, const Arithmetic2 & rhs)
{
    return CT(lhs) != CT(rhs);
}

#undef CT

}
#pragma clang attribute pop

namespace std
{

template <size_t Bits, typename Signed>
struct hash<wide::integer<Bits, Signed>>
{
    std::size_t operator()(const wide::integer<Bits, Signed> & lhs) const
    {
        static_assert(Bits % (sizeof(size_t) * 8) == 0);

        const auto * ptr = reinterpret_cast<const size_t *>(lhs.items);
        unsigned count = Bits / (sizeof(size_t) * 8);

        size_t res = 0;
        for (unsigned i = 0; i < count; ++i)
            res ^= ptr[i];
        return res;
    }
};

}

// NOLINTEND(*)
namespace wide
{
template <size_t Bits, typename Signed>
class integer;
}

using Int128 = wide::integer<128, signed>;
using UInt128 = wide::integer<128, unsigned>;
using Int256 = wide::integer<256, signed>;
using UInt256 = wide::integer<256, unsigned>;

namespace wide
{

template <size_t Bits, typename Signed>
std::string to_string(const integer<Bits, Signed> & n);

extern template std::string to_string(const Int128 & n);
extern template std::string to_string(const UInt128 & n);
extern template std::string to_string(const Int256 & n);
extern template std::string to_string(const UInt256 & n);
}

template <size_t Bits, typename Signed>
std::ostream & operator<<(std::ostream & out, const wide::integer<Bits, Signed> & value);

extern std::ostream & operator<<(std::ostream & out, const Int128 & value);
extern std::ostream & operator<<(std::ostream & out, const UInt128 & value);
extern std::ostream & operator<<(std::ostream & out, const Int256 & value);
extern std::ostream & operator<<(std::ostream & out, const UInt256 & value);

/// See https://fmt.dev/latest/api.html#formatting-user-defined-types
template <size_t Bits, typename Signed>
struct fmt::formatter<wide::integer<Bits, Signed>>
{
    constexpr auto parse(format_parse_context & ctx)
    {
        const auto * it = ctx.begin();
        const auto * end = ctx.end();

        /// Only support {}.
        if (it != end && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template <typename FormatContext>
    auto format(const wide::integer<Bits, Signed> & value, FormatContext & ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", to_string(value));
    }
};

extern template struct fmt::formatter<Int128>;
extern template struct fmt::formatter<UInt128>;
extern template struct fmt::formatter<Int256>;
extern template struct fmt::formatter<UInt256>;
namespace wide
{
template <size_t Bits, typename Signed>

inline std::string to_string(const integer<Bits, Signed> & n)
{
    std::string res;
    if (integer<Bits, Signed>::_impl::operator_eq(n, 0U))
        return "0";

    integer<Bits, unsigned> t;
    bool is_neg = integer<Bits, Signed>::_impl::is_negative(n);
    if (is_neg)
        t = integer<Bits, Signed>::_impl::operator_unary_minus(n);
    else
        t = n;

    while (!integer<Bits, unsigned>::_impl::operator_eq(t, 0U))
    {
        res.insert(res.begin(), '0' + char(integer<Bits, unsigned>::_impl::operator_percent(t, 10U)));
        t = integer<Bits, unsigned>::_impl::operator_slash(t, 10U);
    }

    if (is_neg)
        res.insert(res.begin(), '-');
    return res;
}

template std::string to_string(const integer<128, signed> & n);
template std::string to_string(const integer<128, unsigned> & n);
template std::string to_string(const integer<256, signed> & n);
template std::string to_string(const integer<256, unsigned> & n);

}

template <size_t Bits, typename Signed>
std::ostream & operator<<(std::ostream & out, const wide::integer<Bits, Signed> & value)
{
    return out << to_string(value);
}

std::ostream & operator<<(std::ostream & out, const wide::integer<128, signed> & value);
std::ostream & operator<<(std::ostream & out, const wide::integer<128, unsigned> & value);
std::ostream & operator<<(std::ostream & out, const wide::integer<256, signed> & value);
std::ostream & operator<<(std::ostream & out, const wide::integer<256, unsigned> & value);

template struct fmt::formatter<wide::integer<128, signed>>;
template struct fmt::formatter<wide::integer<128, unsigned>>;
template struct fmt::formatter<wide::integer<256, signed>>;
template struct fmt::formatter<wide::integer<256, unsigned>>;
