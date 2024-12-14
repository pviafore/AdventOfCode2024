#include <generator>
#include <ranges>
namespace algo {

    template<typename Range>
    using RangeValueType = typename std::remove_reference<decltype(std::declval<Range>().begin().operator*())>::type;
    template<typename Range>
    std::generator<std::pair<RangeValueType<Range>, RangeValueType<Range>>>  unique_pairs(Range&& r){
        for(auto iter=r.begin(); iter != r.end(); ++iter){
            for(auto iter2 = iter; iter2 != r.end(); ++iter2){
                if(iter == iter2) continue;

                co_yield std::pair(*iter, *iter2);
            }
        }
    }
}