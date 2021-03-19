#ifndef JOY_TESTER_HPP
#define JOY_TESTER_HPP

#include <optional>
#include <joy/autogen.hpp>
#include <joy/utility.hpp>

namespace joy {
    template< typename R, typename... As >
    auto test(std::size_t N, fn< auto(As...) -> R > f, fn< auto(R, As...) -> bool > tester, std::tuple< fn< auto() -> As >...> gen = std::tuple(autogen< As >() ...)) -> std::optional< std::pair< std::tuple< As... >, R > > {
        for (std::size_t i = 0; i < N; ++i) {
            auto input  = tuple_call(gen);
            auto output = tuple_apply(f, input);
            auto result = tuple_apply(tester, input, output);
            if (!result) return std::pair { input, output };
        }

        return {};
    }
}

#endif