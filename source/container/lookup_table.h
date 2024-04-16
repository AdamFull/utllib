#pragma once

#include <standart_library.h>

// Based on https://joelfilho.com/blog/2020/compile_time_lookup_tables_in_cpp/

namespace utl
{
    // C++20 impl
    template<std::size_t Length, typename Generator>
    constexpr auto lut(Generator&& f)
    {
        using content_type = decltype(f(std::size_t{ 0 }));
        std::array<content_type, Length> arr;

        using namespace std::ranges;
        auto content = views::iota(std::size_t{ 0 }, Length) // Generate a sequence
            | views::transform(std::forward<Generator>(f)); // Transform using our generator
        copy(content, arr.begin());

        return arr;
    }

    // C++17 impl
    //template<std::size_t Length, typename Generator>
    //constexpr auto lut(Generator&& f)
    //{
    //    using content_type = decltype(f(std::size_t{ 0 }));
    //    std::array<content_type, Length> arr{};
    //
    //    for (std::size_t i = 0; i < Length; i++)
    //        arr[i] = f(i);
    //
    //    return arr;
    //}

    // C++14 impl 
    //template<std::size_t Length, typename Generator, std::size_t... Indexes>
    //constexpr auto lut_impl(Generator&& f, std::index_sequence<Indexes...>)
    //{
    //    using content_type = decltype(f(std::size_t{ 0 }));
    //    return std::array<content_type, Length> {{ f(Indexes)... }};
    //}
    //
    //template<std::size_t Length, typename Generator>
    //constexpr auto lut(Generator&& f)
    //{
    //    return lut_impl<Length>(std::forward<Generator>(f), std::make_index_sequence<Length>{});
    //}

    // C++11 impl
    //template<std::size_t Length, typename T, typename Generator, typename... Values>
    //constexpr auto lut_impl(Generator&& f, Values... values)
    //    -> typename std::enable_if<sizeof...(Values) == (Length - 1), std::array<T, Length>>::type
    //{
    //    return { values..., std::forward<Generator>(f)(sizeof...(Values)) };
    //}
    //
    //template<std::size_t Length, typename T, typename Generator, typename... Values>
    //constexpr auto lut_impl(Generator&& f, Values... values)
    //    -> typename std::enable_if < sizeof...(Values) < (Length - 1), std::array<T, Length> > ::type
    //{
    //    return lut_impl < Length, T, Generator, Values..., decltype(f(std::size_t{0})) >
    //                                                    (std::forward<Generator>(f),
    //                                                     values...,
    //                                                     f(sizeof...(Values)));
    //}
    //
    //    template<std::size_t Length, typename Generator>
    //constexpr auto lut(Generator&& f) -> std::array<decltype(f(std::size_t{ 0 })), Length > {
    //    // We call the implementation
    //    return lut_impl < Length, // The size
    //        decltype(f(std::size_t{ 0 })) // The return type
    //    > (std::forward<Generator>(f));
    //}
}