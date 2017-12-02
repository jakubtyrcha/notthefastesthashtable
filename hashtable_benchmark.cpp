//
// Created by Jakub Tyrcha on 01/12/2017.
//

#define NONIUS_RUNNER
#include <nonius/nonius_single.h++>

#include <unordered_map>
#include "hashtable.h"
#include "flat_hash_map.hpp"

constexpr u32 insertions_num = 250000;
constexpr u32 lookups_num = 500000;

#if 1

NONIUS_BENCHMARK("insert unordered_map", []{
    std::unordered_map<u32, u32> h;
    h.reserve(insertions_num);
    for(i32 i=0; i<insertions_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }
})

NONIUS_BENCHMARK("insert hashtable", []{
    hashtable h;
    h.reserve(insertions_num);
    for(i32 i=0; i<insertions_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }
})

NONIUS_BENCHMARK("insert flat_hash_map", []{
    ska::flat_hash_map<u32,u32> h;
    h.reserve(insertions_num);
    for(i32 i=0; i<insertions_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }
})

#endif

#if 1

NONIUS_BENCHMARK("lookup existing unordered_map", [](nonius::chronometer meter) {
    std::unordered_map<u32, u32> h;
    h.reserve(lookups_num);
    for(i32 i=0; i<lookups_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }

    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=0; i<lookups_num; ++i) {
            ctr += ! (h.find(i) == h.end()) ;
        }
        assert(ctr == lookups_num);
        return ctr;
    });
});

NONIUS_BENCHMARK("lookup existing hashtable", [](nonius::chronometer meter) {
    hashtable h;
    h.reserve(lookups_num);
    for(i32 i=0; i<lookups_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }
    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=0; i<lookups_num; ++i) {
            ctr += ! (h.find(i) == h.end()) ;
        }
        assert(ctr == lookups_num);
        return ctr;
    });
});

NONIUS_BENCHMARK("lookup existing flat_hash_map", [](nonius::chronometer meter) {
    ska::flat_hash_map<u32,u32> h;
    h.reserve(lookups_num);
    for(i32 i=0; i<lookups_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }
    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=0; i<lookups_num; ++i) {
            ctr += ! (h.find(i) == h.end()) ;
        }
        assert(ctr == lookups_num);
        return ctr;
    });
});

#endif

#if 1

NONIUS_BENCHMARK("lookup missing unordered_map", [](nonius::chronometer meter) {
    std::unordered_map<u32, u32> h;
    h.reserve(lookups_num);
    for(i32 i=0; i<lookups_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }

    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=lookups_num; i<2 * lookups_num; ++i) {
            ctr += ! (h.find(i) == h.end()) ;
        }
        assert(ctr == 0);
        return ctr;
    });
});

NONIUS_BENCHMARK("lookup missing hashtable", [](nonius::chronometer meter) {
    hashtable h;
    h.reserve(lookups_num);
    for(i32 i=0; i<lookups_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }
    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=lookups_num; i<2 * lookups_num; ++i) {
            ctr += ! (h.find(i) == h.end()) ;
        }
        assert(ctr == 0);
        return ctr;
    });
});

NONIUS_BENCHMARK("lookup missing flat_hash_map", [](nonius::chronometer meter) {
    ska::flat_hash_map<u32,u32> h;
    h.reserve(lookups_num);
    for(i32 i=0; i<lookups_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }
    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=lookups_num; i<2 * lookups_num; ++i) {
            ctr += ! (h.find(i) == h.end()) ;
        }
        assert(ctr == 0);
        return ctr;
    });
});

#endif
