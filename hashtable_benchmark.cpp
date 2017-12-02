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

template<typename THashtable>
void benchmark_insertion(nonius::chronometer meter) {
    THashtable h;
    h.reserve(insertions_num);
    meter.measure([&h] {
        int ctr = 0;
        for (i32 i = 0; i < insertions_num; ++i) {
            ctr += h.insert(std::make_pair(i, 100 + i)).first == h.end() ? 0 : 1;
        }
        return ctr;
    });
}

#if 0

NONIUS_BENCHMARK("insert unordered_map", benchmark_insertion<std::unordered_map<u32,u32>>)
NONIUS_BENCHMARK("insert hashtable", benchmark_insertion<hashtable>)
NONIUS_BENCHMARK("insert flat_hash_map", benchmark_insertion<ska::flat_hash_map<u32,u32>>)

#endif

template<typename THashtable>
void benchmark_lookup_existing(nonius::chronometer meter) {
    THashtable h;
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
}

#if 0

NONIUS_BENCHMARK("lookup existing unordered_map", benchmark_lookup_existing<std::unordered_map<u32,u32>>);
NONIUS_BENCHMARK("lookup existing hashtable", benchmark_lookup_existing<hashtable>);
NONIUS_BENCHMARK("lookup existing flat_hash_map", benchmark_lookup_existing<ska::flat_hash_map<u32,u32>>);

#endif


template<typename THashtable>
void benchmark_lookup_missing(nonius::chronometer meter) {
    THashtable h;
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
}

#if 0

NONIUS_BENCHMARK("lookup missing unordered_map", benchmark_lookup_missing<std::unordered_map<u32,u32>>);
NONIUS_BENCHMARK("lookup missing hashtable", benchmark_lookup_missing<hashtable>);
NONIUS_BENCHMARK("lookup missing flat_hash_map", benchmark_lookup_missing<ska::flat_hash_map<u32,u32>>);

#endif

template<typename THashtable>
void benchmark_deletion(nonius::chronometer meter) {
    THashtable h;
    h.reserve(insertions_num);
    for(i32 i=0; i<insertions_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }

    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=0; i<insertions_num; i+=2) {
            ctr += h.erase(i);
        }
        //assert(ctr == insertions_num / 2);
        return ctr;
    });
}

#if 0

NONIUS_BENCHMARK("deletions unordered_map", benchmark_deletion<std::unordered_map<u32,u32>>);
NONIUS_BENCHMARK("deletions hashtable", benchmark_deletion<hashtable>);
NONIUS_BENCHMARK("deletions flat_hash_map", benchmark_deletion<ska::flat_hash_map<u32,u32>>);

#endif

template<typename THashtable>
void benchmark_lookup_existing_after_deletion(nonius::chronometer meter) {
    THashtable h;
    h.reserve(lookups_num);
    for(i32 i=0; i<lookups_num; ++i) {
        h.insert(std::make_pair(i, 100 + i));
    }

    for(i32 i=0; i<lookups_num; i+=2) {
        h.erase(i);
    }

    meter.measure([&h] {
        int ctr = 0;
        for(i32 i=1; i<lookups_num; i+=2) {
            ctr += !(h.find(i) == h.end());
        }
        return ctr;
    });
}

#if 1

NONIUS_BENCHMARK("unordered_map", benchmark_lookup_existing_after_deletion<std::unordered_map<u32,u32>>);
NONIUS_BENCHMARK("hashtable", benchmark_lookup_existing_after_deletion<hashtable>);
NONIUS_BENCHMARK("flat_hash_map", benchmark_lookup_existing_after_deletion<ska::flat_hash_map<u32,u32>>);

#endif