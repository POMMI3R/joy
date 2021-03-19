# Joy

Simple C++ PBT Library of Joy. EZ.

## Features
* Test function with automatically generated input generators
    * Integral types (`int`, `char`, ...)
    * Floating point types (`float`, `double`, ...)
    * `std::vector< T >` if `T` implements `joy::AutoGen< T >::operator()`
    * ... And any custom types by implementing `joy::AutoGen< T >::operator()`

* Test function with custom generators

## Examples

### Automatically generated input generators (Add numbers)
```cpp
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <joy/all.hpp>

auto add_wrong(int x, int y) -> int {
    return x + y + !(rand() % 10);
}

auto main() -> int {
    auto tester = joy::fn([](int result, int x, int y) -> bool {
        return x + y == result;
    });

    auto test_result = joy::test(
        1024,
        joy::fn(add_wrong),
        tester
    );

    if (test_result) {
        auto [input, output] = *test_result;
        auto [x, y] = input;
        std::cout << "[Failed]\n";
        std::cout << "Input : " << x << " + " << y << '\n';
        std::cout << "Output: " << output << '\n';
    }
    else {
        std::cout << "[Passed]\n";
    }

    return 0;
}
```

### Automatically generated input generators (Sort numbers)

1. Implment `joy::AutoGen< MyChar >::operator()`
2. `joy::AutoGen< std::vector< MyChar > >::operator()` is automatically implemented
3. ... And inputs are automatically generated

```cpp
#include <iostream>
#include <algorithm>
#include <compare>
#include <cstdlib>
#include <joy/all.hpp>

struct MyChar {
    char data;

    auto operator<=>(const MyChar&) const = default;

    friend auto operator<<(std::ostream& lhs, MyChar rhs) -> auto& {
        lhs << rhs.data;
        return lhs;
    }
};

template<>
struct joy::AutoGen< MyChar > {
    auto operator()() const -> MyChar {
        static auto dist = std::uniform_int_distribution('A', 'Z');
        return MyChar { dist(joy::mt) };
    }
};

template< typename T >
auto operator<<(std::ostream& lhs, std::vector< T > rhs) -> auto& {
    lhs << '[';
    for (int i = 0; i < (int)rhs.size() - 1; ++i)
        lhs << rhs[ i ] << ", ";
    lhs << rhs.back() << ']';
    return lhs;
}

auto sort_wrong(std::vector< MyChar > v) -> std::vector< MyChar > {
    std::sort(v.begin(), v.end());
    if (v.size() > 1 && (rand() % 10))
        std::swap(v[ 0 ], v[ 1 ]);
    return v;
}

auto main() -> int {
    auto tester = joy::fn([](std::vector< MyChar > sorted, std::vector< MyChar > original) -> bool {
        for (int i = 0; i < (int)sorted.size() - 1; ++i)
            if (sorted[ i ] > sorted[ i + 1 ])
                return false;
        return true;
    });

    auto test_result = joy::test(
        1024,
        joy::fn(sort_wrong),
        tester
    );

    if (test_result) {
        auto [inputt, output] = *test_result;
        auto [input] = inputt;
        std::cout << "[Failed]\n";
        std::cout << "Input : " << input  << '\n';
        std::cout << "Output: " << output << '\n';
    }
    else {
        std::cout << "[Passed]\n";
    }

    return 0;
}
```

### Custom generators (Add numbers)
```cpp
#include <iostream>
#include <cstdlib>
#include <joy/all.hpp>

auto add_wrong(int x, int y, int z) -> int {
    return x + y + z + !(rand() % 10);
}

auto main() -> int {
    auto tester = joy::fn([](int result, int x, int y, int z) -> bool {
        return x + y + z == result;
    });

    auto gen1 = joy::fn([]() -> int {
        static auto dist = std::uniform_int_distribution(0, 9);
        return dist(joy::mt);
    });

    auto gen2 = joy::fn([]() -> int {
        static auto dist = std::uniform_int_distribution(10, 99);
        return dist(joy::mt);
    });

    auto test_result = joy::test(
        1024,
        joy::fn(add_wrong),
        tester,
        std::tuple(gen1, gen2, joy::autogen< int >())
    );

    if (test_result) {
        auto [input, output] = *test_result;
        auto [x, y, z] = input;
        std::cout << "[Failed]\n";
        std::cout << "Input : " << x << " + " << y << " + " << z << '\n';
        std::cout << "Output: " << output << '\n';
    }
    else {
        std::cout << "[Passed]\n";
    }

    return 0;
}
```