#include <chrono>
#include <fmt/core.h>

#ifdef USE_CXX11_HEADER
#    include <wide_integer/wide_integer_cxx11.h>
#else
#    include <wide_integer/wide_integer.h>
#endif

int main()
{
    using WInt = wide::integer<256, unsigned>;
    WInt result = 0;
    WInt a = 123456789;
    WInt b = 987654321;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i)
    {
        result += a * b;
        result -= a;
    }
    auto end = std::chrono::high_resolution_clock::now();
    fmt::print("{}\n", wide::to_string(result));
    fmt::print("{}\n", std::chrono::duration<double, std::milli>(end - start).count());
    return 0;
}
