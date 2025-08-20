#pragma once

#include <cmath>
#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>
#include <fmt/format.h>

namespace wide
{

template <size_t Bits, typename Signed>
class integer;

using Int128 = integer<128, signed>;
using UInt128 = integer<128, unsigned>;
using Int256 = integer<256, signed>;
using UInt256 = integer<256, unsigned>;

template <size_t Bits, typename Signed>
std::string to_string(const integer<Bits, Signed> & value);

template <size_t Bits, typename Signed>
std::ostream & operator<<(std::ostream & out, const integer<Bits, Signed> & value);

namespace detail
{
template <size_t Bits>
struct storage_count
{
    static_assert(Bits % 64 == 0, "Bits must be multiple of 64");
    static constexpr size_t value = Bits / 64;
};

template <size_t... I>
struct index_sequence
{
};

template <size_t N, size_t... I>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, I...>
{
};

template <size_t... I>
struct make_index_sequence<0, I...>
{
    using type = index_sequence<I...>;
};

template <size_t I>
struct limbs_equal
{
    template <typename Int>
    static constexpr bool eval(const Int & lhs, const Int & rhs) noexcept
    {
        return lhs.data_[I] == rhs.data_[I] && limbs_equal<I - 1>::eval(lhs, rhs);
    }
};

template <>
struct limbs_equal<0>
{
    template <typename Int>
    static constexpr bool eval(const Int & lhs, const Int & rhs) noexcept
    {
        return lhs.data_[0] == rhs.data_[0];
    }
};

template <size_t I>
struct add_limbs
{
    template <typename Int>
    static void eval(Int & lhs, const Int & rhs, unsigned __int128 & carry) noexcept
    {
        add_limbs<I - 1>::eval(lhs, rhs, carry);
        unsigned __int128 sum = static_cast<unsigned __int128>(lhs.data_[I]) + rhs.data_[I] + carry;
        lhs.data_[I] = static_cast<typename Int::limb_type>(sum);
        carry = sum >> 64;
    }
};

template <>
struct add_limbs<0>
{
    template <typename Int>
    static void eval(Int & lhs, const Int & rhs, unsigned __int128 & carry) noexcept
    {
        unsigned __int128 sum = static_cast<unsigned __int128>(lhs.data_[0]) + rhs.data_[0] + carry;
        lhs.data_[0] = static_cast<typename Int::limb_type>(sum);
        carry = sum >> 64;
    }
};

template <size_t I>
struct sub_limbs
{
    template <typename Int>
    static void eval(Int & lhs, const Int & rhs, unsigned __int128 & borrow) noexcept
    {
        sub_limbs<I - 1>::eval(lhs, rhs, borrow);
        unsigned __int128 diff = static_cast<unsigned __int128>(lhs.data_[I]) - rhs.data_[I] - borrow;
        lhs.data_[I] = static_cast<typename Int::limb_type>(diff);
        borrow = (diff >> 127) & 1;
    }
};

template <>
struct sub_limbs<0>
{
    template <typename Int>
    static void eval(Int & lhs, const Int & rhs, unsigned __int128 & borrow) noexcept
    {
        unsigned __int128 diff = static_cast<unsigned __int128>(lhs.data_[0]) - rhs.data_[0] - borrow;
        lhs.data_[0] = static_cast<typename Int::limb_type>(diff);
        borrow = (diff >> 127) & 1;
    }
};
}

template <size_t Bits, typename Signed>
class integer
{
public:
    static constexpr size_t limbs = detail::storage_count<Bits>::value;
    using limb_type = uint64_t;
    template <size_t>
    friend struct detail::limbs_equal;
    template <size_t>
    friend struct detail::add_limbs;
    template <size_t>
    friend struct detail::sub_limbs;

    constexpr integer() noexcept = default;

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    constexpr integer(T v) noexcept
        : integer(v, typename detail::make_index_sequence<limbs>::type())
    {
    }

private:
    template <typename T, size_t... I>
    constexpr integer(T v, detail::index_sequence<I...>) noexcept
        : data_{limb_from<T, I>(v)...}
    {
    }

    template <typename T, size_t I>
    static constexpr limb_type limb_from(T v) noexcept
    {
        return I < (sizeof(T) * 8 + 63) / 64
            ? static_cast<limb_type>((std::is_signed<T>::value ? static_cast<__int128>(v) : static_cast<unsigned __int128>(v)) >> (I * 64))
            : (std::is_signed<T>::value && v < 0 ? ~0ULL : 0ULL);
    }

public:
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    integer(T v) noexcept
    {
        assign_float(v);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    integer & operator=(T v) noexcept
    {
        assign(v);
        return *this;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    integer & operator=(T v) noexcept
    {
        assign_float(v);
        return *this;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    operator T() const noexcept
    {
        unsigned __int128 value = 0;
        for (size_t i = 0; i < limbs && i < (sizeof(T) + sizeof(limb_type) - 1) / sizeof(limb_type); ++i)
            value |= static_cast<unsigned __int128>(data_[i]) << (i * 64);

        if (std::is_signed<T>::value)
            return static_cast<T>(static_cast<__int128>(value));
        else
            return static_cast<T>(value);
    }

    operator __int128() const noexcept
    {
        unsigned __int128 value = 0;
        for (size_t i = 0; i < limbs && i < (sizeof(__int128) + sizeof(limb_type) - 1) / sizeof(limb_type); ++i)
            value |= static_cast<unsigned __int128>(data_[i]) << (i * 64);
        return static_cast<__int128>(value);
    }

    operator unsigned __int128() const noexcept
    {
        unsigned __int128 value = 0;
        for (size_t i = 0; i < limbs && i < (sizeof(unsigned __int128) + sizeof(limb_type) - 1) / sizeof(limb_type); ++i)
            value |= static_cast<unsigned __int128>(data_[i]) << (i * 64);
        return value;
    }

    explicit operator long double() const noexcept
    {
        if (is_zero())
            return 0;
        bool neg = std::is_same<Signed, signed>::value && (data_[limbs - 1] >> 63);
        integer tmp = neg ? -*this : *this;
        long double res = 0;
        for (size_t i = limbs; i-- > 0;)
        {
            res *= std::ldexp(1.0L, 64);
            res += static_cast<long double>(tmp.data_[i]);
        }
        return neg ? -res : res;
    }

    explicit operator double() const noexcept { return static_cast<double>(static_cast<long double>(*this)); }

    explicit operator float() const noexcept { return static_cast<float>(static_cast<long double>(*this)); }

    integer & operator+=(const integer & rhs) noexcept
    {
        unsigned __int128 carry = 0;
        detail::add_limbs<limbs - 1>::eval(*this, rhs, carry);
        return *this;
    }

    integer & operator-=(const integer & rhs) noexcept
    {
        unsigned __int128 borrow = 0;
        detail::sub_limbs<limbs - 1>::eval(*this, rhs, borrow);
        return *this;
    }

    integer & operator&=(const integer & rhs) noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
            data_[i] &= rhs.data_[i];
        return *this;
    }

    integer & operator|=(const integer & rhs) noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
            data_[i] |= rhs.data_[i];
        return *this;
    }

    integer & operator^=(const integer & rhs) noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
            data_[i] ^= rhs.data_[i];
        return *this;
    }

    integer & operator*=(const integer & rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    integer & operator/=(const integer & rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }

    integer & operator%=(const integer & rhs) noexcept
    {
        *this = *this % rhs;
        return *this;
    }

    integer & operator<<=(int n) noexcept
    {
        if (n <= 0)
            return *this;
        size_t limb_shift = static_cast<size_t>(n) / 64;
        int bit_shift = n % 64;
        if (limb_shift)
        {
            for (size_t i = limbs; i-- > limb_shift;)
                data_[i] = data_[i - limb_shift];
            for (size_t i = 0; i < limb_shift; ++i)
                data_[i] = 0;
        }
        if (bit_shift)
        {
            for (size_t i = limbs; i-- > 0;)
            {
                unsigned __int128 part = static_cast<unsigned __int128>(data_[i]) << bit_shift;
                if (i)
                    part |= data_[i - 1] >> (64 - bit_shift);
                data_[i] = static_cast<limb_type>(part);
            }
        }
        return *this;
    }

    integer & operator>>=(int n) noexcept
    {
        if (n <= 0)
            return *this;
        size_t limb_shift = static_cast<size_t>(n) / 64;
        int bit_shift = n % 64;
        if (limb_shift)
        {
            for (size_t i = 0; i < limbs - limb_shift; ++i)
                data_[i] = data_[i + limb_shift];
            for (size_t i = limbs - limb_shift; i < limbs; ++i)
                data_[i] = 0;
        }
        if (bit_shift)
        {
            for (size_t i = 0; i < limbs; ++i)
            {
                unsigned __int128 part = static_cast<unsigned __int128>(data_[i]) >> bit_shift;
                if (i + 1 < limbs)
                    part |= static_cast<unsigned __int128>(data_[i + 1]) << (64 - bit_shift);
                data_[i] = static_cast<limb_type>(part);
            }
        }
        return *this;
    }

    integer & operator++() noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
        {
            if (++data_[i])
                break;
        }
        return *this;
    }

    integer operator++(int) noexcept
    {
        integer tmp = *this;
        ++(*this);
        return tmp;
    }

    integer & operator--() noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
        {
            limb_type old = data_[i];
            --data_[i];
            if (old)
                break;
        }
        return *this;
    }

    integer operator--(int) noexcept
    {
        integer tmp = *this;
        --(*this);
        return tmp;
    }

    explicit operator bool() const noexcept { return !is_zero(); }

    friend integer operator+(integer lhs, const integer & rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator+(integer lhs, T rhs) noexcept
    {
        lhs += integer(rhs);
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator+(T lhs, integer rhs) noexcept
    {
        rhs += integer(lhs);
        return rhs;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator+(integer lhs, T rhs) noexcept
    {
        lhs += integer(rhs);
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator+(T lhs, integer rhs) noexcept
    {
        rhs += integer(lhs);
        return rhs;
    }

    friend integer operator-(integer lhs, const integer & rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator-(integer lhs, T rhs) noexcept
    {
        lhs -= integer(rhs);
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator-(T lhs, integer rhs) noexcept
    {
        return integer(lhs) - rhs;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator-(integer lhs, T rhs) noexcept
    {
        lhs -= integer(rhs);
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator-(T lhs, integer rhs) noexcept
    {
        return integer(lhs) - rhs;
    }

    friend integer operator&(integer lhs, const integer & rhs) noexcept
    {
        lhs &= rhs;
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator&(integer lhs, T rhs) noexcept
    {
        lhs &= integer(rhs);
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator&(T lhs, integer rhs) noexcept
    {
        rhs &= integer(lhs);
        return rhs;
    }

    friend integer operator|(integer lhs, const integer & rhs) noexcept
    {
        lhs |= rhs;
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator|(integer lhs, T rhs) noexcept
    {
        lhs |= integer(rhs);
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator|(T lhs, integer rhs) noexcept
    {
        rhs |= integer(lhs);
        return rhs;
    }

    friend integer operator^(integer lhs, const integer & rhs) noexcept
    {
        lhs ^= rhs;
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator^(integer lhs, T rhs) noexcept
    {
        lhs ^= integer(rhs);
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator^(T lhs, integer rhs) noexcept
    {
        rhs ^= integer(lhs);
        return rhs;
    }

    friend integer operator<<(integer lhs, int n) noexcept
    {
        lhs <<= n;
        return lhs;
    }

    friend integer operator>>(integer lhs, int n) noexcept
    {
        lhs >>= n;
        return lhs;
    }

    friend integer operator*(const integer & lhs, const integer & rhs) noexcept
    {
        if (limbs == 4 && sizeof(limb_type) == 8)
        {
            using HalfType = unsigned __int128;
            HalfType a01 = (HalfType(lhs.data_[1]) << 64) + lhs.data_[0];
            HalfType a23 = (HalfType(lhs.data_[3]) << 64) + lhs.data_[2];
            HalfType b01 = (HalfType(rhs.data_[1]) << 64) + rhs.data_[0];
            HalfType b23 = (HalfType(rhs.data_[3]) << 64) + rhs.data_[2];

            HalfType r23 = a23 * b01 + a01 * b23 + HalfType(lhs.data_[1]) * rhs.data_[1];
            HalfType r01 = HalfType(lhs.data_[0]) * rhs.data_[0];
            HalfType r12 = (r01 >> 64) + (r23 << 64);
            HalfType r12_x = HalfType(lhs.data_[1]) * rhs.data_[0];

            integer result;
            result.data_[0] = static_cast<limb_type>(r01);
            result.data_[3] = static_cast<limb_type>(r23 >> 64);

            HalfType r12_y = HalfType(lhs.data_[0]) * rhs.data_[1];
            r12_x += r12_y;
            if (r12_x < r12_y)
                ++result.data_[3];

            r12 += r12_x;
            if (r12 < r12_x)
                ++result.data_[3];

            result.data_[1] = static_cast<limb_type>(r12);
            result.data_[2] = static_cast<limb_type>(r12 >> 64);
            return result;
        }
        else
        {
            integer result;
            for (size_t i = 0; i < limbs; ++i)
            {
                unsigned __int128 carry = 0;
                for (size_t j = 0; j + i < limbs; ++j)
                {
                    unsigned __int128 cur = result.data_[i + j];
                    cur += static_cast<unsigned __int128>(lhs.data_[i]) * rhs.data_[j];
                    cur += carry;
                    result.data_[i + j] = static_cast<limb_type>(cur);
                    carry = cur >> 64;
                }
            }
            return result;
        }
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator*(integer lhs, T rhs) noexcept
    {
        return lhs * integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator*(T lhs, integer rhs) noexcept
    {
        return integer(lhs) * rhs;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator*(integer lhs, T rhs) noexcept
    {
        return lhs * integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator*(T lhs, integer rhs) noexcept
    {
        return integer(lhs) * rhs;
    }

    friend integer operator/(integer lhs, const integer & rhs) noexcept
    {
        integer result;
        integer divisor = rhs;
        integer current(1);
        while (divisor <= lhs && !(divisor.data_[limbs - 1] & (1ULL << 63)))
        {
            divisor <<= 1;
            current <<= 1;
        }
        while (!current.is_zero())
        {
            if (!(lhs < divisor))
            {
                lhs -= divisor;
                result |= current;
            }
            divisor >>= 1;
            current >>= 1;
        }
        return result;
    }

    friend integer operator%(integer lhs, const integer & rhs) noexcept
    {
        integer q = lhs / rhs;
        q *= rhs;
        lhs -= q;
        return lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator/(integer lhs, T rhs) noexcept
    {
        return lhs / integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator/(T lhs, integer rhs) noexcept
    {
        return integer(lhs) / rhs;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator/(integer lhs, T rhs) noexcept
    {
        return lhs / integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator/(T lhs, integer rhs) noexcept
    {
        return integer(lhs) / rhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator%(integer lhs, T rhs) noexcept
    {
        return lhs % integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend integer operator%(T lhs, integer rhs) noexcept
    {
        return integer(lhs) % rhs;
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator%(integer lhs, T rhs) noexcept
    {
        return lhs % integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend integer operator%(T lhs, integer rhs) noexcept
    {
        return integer(lhs) % rhs;
    }

    friend constexpr bool operator==(const integer & lhs, const integer & rhs) noexcept
    {
        return detail::limbs_equal<limbs - 1>::eval(lhs, rhs);
    }

    friend constexpr bool operator!=(const integer & lhs, const integer & rhs) noexcept { return !(lhs == rhs); }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend constexpr bool operator==(const integer & lhs, T rhs) noexcept
    {
        return lhs == integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend constexpr bool operator==(T lhs, const integer & rhs) noexcept
    {
        return integer(lhs) == rhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend constexpr bool operator!=(const integer & lhs, T rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend constexpr bool operator!=(T lhs, const integer & rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator==(const integer & lhs, T rhs) noexcept
    {
        return static_cast<long double>(lhs) == static_cast<long double>(rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator==(T lhs, const integer & rhs) noexcept
    {
        return static_cast<long double>(lhs) == static_cast<long double>(rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator!=(const integer & lhs, T rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator!=(T lhs, const integer & rhs) noexcept
    {
        return !(lhs == rhs);
    }

    friend bool operator<(const integer & lhs, const integer & rhs) noexcept
    {
        if (std::is_same<Signed, signed>::value)
        {
            bool lhs_neg = lhs.data_[limbs - 1] >> 63;
            bool rhs_neg = rhs.data_[limbs - 1] >> 63;
            if (lhs_neg != rhs_neg)
                return lhs_neg;
        }
        for (size_t i = limbs; i-- > 0;)
        {
            if (lhs.data_[i] != rhs.data_[i])
                return lhs.data_[i] < rhs.data_[i];
        }
        return false;
    }

    friend bool operator>(const integer & lhs, const integer & rhs) noexcept { return rhs < lhs; }

    friend bool operator<=(const integer & lhs, const integer & rhs) noexcept { return !(rhs < lhs); }

    friend bool operator>=(const integer & lhs, const integer & rhs) noexcept { return !(lhs < rhs); }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator<(const integer & lhs, T rhs) noexcept
    {
        return lhs < integer(rhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator<(T lhs, const integer & rhs) noexcept
    {
        return integer(lhs) < rhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator>(const integer & lhs, T rhs) noexcept
    {
        return integer(rhs) < lhs;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator>(T lhs, const integer & rhs) noexcept
    {
        return rhs < integer(lhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator<=(const integer & lhs, T rhs) noexcept
    {
        return !(integer(rhs) < lhs);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator<=(T lhs, const integer & rhs) noexcept
    {
        return !(rhs < integer(lhs));
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator>=(const integer & lhs, T rhs) noexcept
    {
        return !(lhs < integer(rhs));
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    friend bool operator>=(T lhs, const integer & rhs) noexcept
    {
        return !(integer(lhs) < rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator<(const integer & lhs, T rhs) noexcept
    {
        return static_cast<long double>(lhs) < static_cast<long double>(rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator<(T lhs, const integer & rhs) noexcept
    {
        return static_cast<long double>(lhs) < static_cast<long double>(rhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator>(const integer & lhs, T rhs) noexcept
    {
        return static_cast<long double>(rhs) < static_cast<long double>(lhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator>(T lhs, const integer & rhs) noexcept
    {
        return static_cast<long double>(rhs) < static_cast<long double>(lhs);
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator<=(const integer & lhs, T rhs) noexcept
    {
        return !(static_cast<long double>(rhs) < static_cast<long double>(lhs));
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator<=(T lhs, const integer & rhs) noexcept
    {
        return !(static_cast<long double>(rhs) < static_cast<long double>(lhs));
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator>=(const integer & lhs, T rhs) noexcept
    {
        return !(static_cast<long double>(lhs) < static_cast<long double>(rhs));
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    friend bool operator>=(T lhs, const integer & rhs) noexcept
    {
        return !(static_cast<long double>(lhs) < static_cast<long double>(rhs));
    }

    friend integer operator~(integer v) noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
            v.data_[i] = ~v.data_[i];
        return v;
    }

    friend integer operator-(const integer & v) noexcept
    {
        integer res = ~v;
        limb_type carry = 1;
        for (size_t i = 0; i < limbs; ++i)
        {
            unsigned __int128 sum = static_cast<unsigned __int128>(res.data_[i]) + carry;
            res.data_[i] = static_cast<limb_type>(sum);
            carry = sum >> 64;
            if (!carry)
                break;
        }
        return res;
    }

    friend integer operator+(const integer & v) noexcept { return v; }

    friend std::string to_string<>(const integer & v);

private:
    template <typename T>
    void assign(T v) noexcept
    {
        using ST = typename std::conditional<std::is_signed<T>::value, __int128_t, unsigned __int128>::type;
        ST val = static_cast<ST>(v);
        for (size_t i = 0; i < limbs; ++i)
        {
            data_[i] = static_cast<limb_type>(static_cast<unsigned __int128>(val));
            if (std::is_signed<T>::value)
                val >>= 64;
            else
                val = static_cast<unsigned __int128>(val) >> 64;
        }
        if (std::is_signed<T>::value && v < 0)
        {
            for (size_t i = (sizeof(T) * 8 + 63) / 64; i < limbs; ++i)
                data_[i] = ~0ULL;
        }
    }

    template <typename T>
    void assign_float(T v) noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
            data_[i] = 0;
        if (v == 0)
            return;
        bool neg = v < 0;
        if (neg)
            v = -v;
        long double val = static_cast<long double>(v);
        long double intpart;
        std::modf(val, &intpart);
        val = intpart;
        long double base = std::ldexp(1.0L, 64);
        for (size_t i = 0; i < limbs && val > 0; ++i)
        {
            long double rem = std::fmod(val, base);
            data_[i] = static_cast<limb_type>(rem);
            val = std::floor(val / base);
        }
        if (neg)
            *this = -*this;
    }

    bool is_zero() const noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
            if (data_[i] != 0)
                return false;
        return true;
    }

    limb_type div_mod_small(limb_type div, integer & quotient) const noexcept
    {
        quotient = integer();
        unsigned __int128 rem = 0;
        for (size_t i = limbs; i-- > 0;)
        {
            rem = (rem << 64) | data_[i];
            quotient.data_[i] = static_cast<limb_type>(rem / div);
            rem %= div;
        }
        return static_cast<limb_type>(rem);
    }

    limb_type data_[limbs] = {};
};

template <size_t Bits, typename Signed>
inline std::string to_string(const integer<Bits, Signed> & v)
{
    integer<Bits, Signed> tmp = v;
    bool neg = false;
    if (std::is_same<Signed, signed>::value && (v.data_[integer<Bits, Signed>::limbs - 1] >> 63))
    {
        tmp = -v;
        neg = true;
    }
    if (tmp.is_zero())
        return "0";
    std::string res;
    while (!tmp.is_zero())
    {
        integer<Bits, Signed> q;
        typename integer<Bits, Signed>::limb_type rem = tmp.div_mod_small(10, q);
        res.insert(res.begin(), static_cast<char>('0' + rem));
        tmp = q;
    }
    if (neg)
        res.insert(res.begin(), '-');
    return res;
}

template <size_t Bits, typename Signed>
inline std::ostream & operator<<(std::ostream & out, const integer<Bits, Signed> & value)
{
    return out << to_string(value);
}

} // namespace wide

namespace fmt
{
template <size_t Bits, typename Signed>
struct formatter<wide::integer<Bits, Signed>>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext & ctx) -> typename ParseContext::iterator
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const wide::integer<Bits, Signed> & value, FormatContext & ctx) const -> typename FormatContext::iterator
    {
        return fmt::format_to(ctx.out(), "{}", wide::to_string(value));
    }
};
} // namespace fmt
