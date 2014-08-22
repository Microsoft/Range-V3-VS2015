//  Copyright Neil Groves 2009.
//  Copyright Eric Niebler 2013
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef RANGES_V3_ALGORITHM_REVERSE_HPP
#define RANGES_V3_ALGORITHM_REVERSE_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/swap.hpp>

namespace ranges
{
    inline namespace v3
    {
        struct reverse_fn
        {
        private:
            template<typename I>
            static void impl(I begin, I end, concepts::BidirectionalIterator*)
            {
                while(begin != end)
                {
                    if(begin == --end)
                        break;
                    ranges::swap(*begin, *end);
                    ++begin;
                }
            }

            template<typename I>
            static void impl(I begin, I end, concepts::RandomAccessIterator*)
            {
                if(begin != end)
                    for(; begin < --end; ++begin)
                        ranges::swap(*begin, *end);
            }

        public:
            template<typename I, typename S,
                CONCEPT_REQUIRES_(BidirectionalIterator<I, S>() && Permutable<I>())>
            I operator()(I begin, S end_) const
            {
                I end = next_to(begin, end_);
                reverse_fn::impl(begin, end, iterator_concept<I>{});
                return end;
            }

            template<typename Rng, typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(ConvertibleToBidirectionalRange<Rng>() && Permutable<I>())>
            I operator()(Rng & rng) const
            {
                return (*this)(begin(rng), end(rng));
            }
        };

        RANGES_CONSTEXPR reverse_fn reverse{};

    } // namespace v3
} // namespace ranges

#endif // include guard
