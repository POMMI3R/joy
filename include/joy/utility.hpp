#ifndef JOY_UTILITY_HPP
#define JOY_UTILITY_HPP

#include <functional>
#include <utility>

namespace joy {
    template< typename F >
    using fn = std::function< F >;

    template< typename F, typename... As, typename... Bs >
    auto tuple_apply(F f, std::tuple< As... > tuple, Bs... additional) -> auto {
        return [&]< auto... p >(std::index_sequence< p... >) -> auto {
            return f(additional..., std::get< p >(tuple)...);
        } (std::make_index_sequence< sizeof...(As) >());
    }

    template< typename... As >
    auto tuple_call(std::tuple< As... > tuple) -> auto {
        return [&]< auto... p >(std::index_sequence< p... >) -> auto {
            return std::tuple(std::get< p >(tuple)()...);
        } (std::make_index_sequence< sizeof...(As) >());
    }
}

#endif