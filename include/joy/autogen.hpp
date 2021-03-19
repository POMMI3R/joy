#ifndef JOY_AUTOGEN_HPP
#define JOY_AUTOGEN_HPP

#include <concepts>
#include <random>
#include <limits>
#include <joy/utility.hpp>

namespace joy {
    inline auto mt = std::mt19937(std::random_device {} ());

    template< typename T >
    struct AutoGen {};

    template< typename T >
    auto autogen() -> fn< auto() -> T > {
        return fn(AutoGen< T > {});
    }

    template< std::integral T >
    struct AutoGen< T > {
        auto operator()() const -> T {
            static auto dist = std::uniform_int_distribution(std::numeric_limits< T >::min(), std::numeric_limits< T >::max());
            return dist(mt);
        }
    };

    template< std::floating_point T >
    struct AutoGen< T > {
        auto operator()() const -> T {
            static auto dist = std::uniform_real_distribution(std::numeric_limits< T >::min(), std::numeric_limits< T >::max());
            return dist(mt);
        }
    };

    template< typename T >
    struct AutoGen< std::vector< T > > {
        auto operator()() const -> std::vector< T > {
            static auto len_dist = std::uniform_int_distribution(0, 128);
            auto len = len_dist(mt);
            auto v = std::vector< T >(len);
            std::generate(v.begin(), v.end(), autogen< T >());
            return v;
        }
    };
}

#endif