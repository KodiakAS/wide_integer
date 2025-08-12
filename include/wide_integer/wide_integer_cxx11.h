#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <type_traits>

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

namespace detail
{
template <size_t Bits>
struct storage_count
{
    static_assert(Bits % 64 == 0, "Bits must be multiple of 64");
    static constexpr size_t value = Bits / 64;
};
}

template <size_t Bits, typename Signed>
class integer
{
public:
    static constexpr size_t limbs = detail::storage_count<Bits>::value;
    using limb_type = uint64_t;

    constexpr integer() noexcept = default;

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    integer(T v) noexcept
    {
        assign(v);
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    integer & operator=(T v) noexcept
    {
        assign(v);
        return *this;
    }

    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    operator T() const noexcept
    {
        unsigned __int128 value = 0;
        for (size_t i = 0; i < limbs && i < (sizeof(T) + 63) / 64; ++i)
            value |= static_cast<unsigned __int128>(data_[i]) << (i * 64);

        if (std::is_signed<T>::value)
            return static_cast<T>(static_cast<__int128>(value));
        else
            return static_cast<T>(value);
    }

    integer & operator+=(const integer & rhs) noexcept
    {
        unsigned __int128 carry = 0;
        for (size_t i = 0; i < limbs; ++i)
        {
            unsigned __int128 sum = static_cast<unsigned __int128>(data_[i]) + rhs.data_[i] + carry;
            data_[i] = static_cast<limb_type>(sum);
            carry = sum >> 64;
        }
        return *this;
    }

    integer & operator-=(const integer & rhs) noexcept
    {
        unsigned __int128 borrow = 0;
        for (size_t i = 0; i < limbs; ++i)
        {
            unsigned __int128 sub = static_cast<unsigned __int128>(data_[i]) - rhs.data_[i] - borrow;
            data_[i] = static_cast<limb_type>(sub);
            borrow = (sub >> 127) & 1;
        }
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

    friend integer operator+(integer lhs, const integer & rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    friend integer operator-(integer lhs, const integer & rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    friend integer operator&(integer lhs, const integer & rhs) noexcept
    {
        lhs &= rhs;
        return lhs;
    }

    friend integer operator|(integer lhs, const integer & rhs) noexcept
    {
        lhs |= rhs;
        return lhs;
    }

    friend integer operator^(integer lhs, const integer & rhs) noexcept
    {
        lhs ^= rhs;
        return lhs;
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

    friend bool operator==(const integer & lhs, const integer & rhs) noexcept
    {
        for (size_t i = 0; i < limbs; ++i)
            if (lhs.data_[i] != rhs.data_[i])
                return false;
        return true;
    }

    friend bool operator!=(const integer & lhs, const integer & rhs) noexcept { return !(lhs == rhs); }

    friend bool operator<(const integer & lhs, const integer & rhs) noexcept
    {
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

    std::array<limb_type, limbs> data_{};
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

} // namespace wide
