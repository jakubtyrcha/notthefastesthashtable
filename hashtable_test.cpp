//
// Created by Jakub Tyrcha on 29/11/2017.
//

#include "hashtable.h"
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE( "Hashtable internals support insertion, overwrites and removals", "[hashtable_internals]" ) {

    hash_table_internals h;
    h.initialize(17);
    {
        u32 k = 1;
        u32 v = 100;
        auto [ok, index, collision] = h.try_insert(k, v);
        REQUIRE(h.values_[h.find(1)] == 100);
        REQUIRE(h.find(1) == index);
        REQUIRE(!collision);
    }
    {
        u32 k = 1;
        u32 v = 101;
        auto [ok, index, collision] = h.try_insert(k, v);
        h.values_[index] = 101;
        REQUIRE(h.values_[h.find(1)] == 101);
        REQUIRE(h.find(1) == index);
        REQUIRE(collision);
    }
    REQUIRE(h.size() == 1);

    for(u32 i = 0; i < 17; i++)
    {
        u32 k = i;
        u32 v = 100 + i;
        auto [ok, index, collision] = h.try_insert(k, v);
        REQUIRE(h.find(i) == index);
    }
    for(u32 i = 0; i < 17; i++)
    {
        i32 iter = h.find(i);
        REQUIRE(h.values_[iter] == 100 + i);
    }
    REQUIRE(h.size() == 17);
    for(u32 i=0; i<7; i++)
    {
        bool removed = h.remove(i);
        REQUIRE(removed);
    }
    REQUIRE(h.size() == 10);
}

TEST_CASE( "Hashtable internals supports resize and iteration", "[hashtable_internals]" ) {
    hash_table_internals h;
    h.initialize(17);

    for(u32 i = 0; i < 17; i++)
    {
        u32 k = i;
        u32 v = 100 + i;
        auto [ok, index, overwritten] = h.try_insert(k, v);
    }

    h.resize_and_rehash(31);

    i32 count = 0;
    i32 index = h.find_next(-1);
    while(index != -1) {
        count += index != -1;
        index = h.find_next(index);
    }

    REQUIRE(count == 17);

    for(u32 i = 0; i < 10; i++)
    {
        u32 k = 100 + i;
        u32 v = 200 + i;
        auto [ok, index, overwritten] = h.try_insert(k, v);
        REQUIRE(h.find(100+i) == index);
    }

    REQUIRE(h.size() == 27);

    h.resize_and_rehash(103);

    count = 0;
    index = h.find_next(-1);
    while(index != -1) {
        count += index != -1;
        index = h.find_next(index);
    }

    REQUIRE(count == 27);
}

TEST_CASE( "Hashtable supports default initialization and multiple insertions with rehashes", "[hashtable]" ) {
    hashtable h;
    for(u32 i = 0; i< 1000; i++) {
        auto [iter, ok] = h.insert(std::make_pair(i, 1000 + i));
        REQUIRE(iter.key() == i);
        REQUIRE(iter.value() == 1000 + i);
    }
    REQUIRE(h.size() == 1000);

    for(u32 i = 0; i< 1000; i++) {
        auto iter = h.find(i);
        REQUIRE(!(iter == h.end()));
        REQUIRE(iter.key() == i);
        REQUIRE(iter.value() == 1000 + i);
    }
}

TEST_CASE( "Hashtable supports insertions mixed with erasures, failing insertions return iter to colliding key", "[hashtable]" ) {
    hashtable h;
    for(u32 i = 0; i< 20; i++) {
        auto [iter, ok] = h.insert(std::make_pair(i, 1000 + i));
        REQUIRE(iter.key() == i);
        REQUIRE(iter.value() == 1000 + i);
    }
    REQUIRE(h.size() == 20);

    for(u32 i = 0; i< 20; i+=2) {
        bool erased = h.erase(i);
        REQUIRE(erased);
        auto iter = h.find(i);
        REQUIRE(iter == h.end());
    }
    REQUIRE(h.size() == 10);

    for(u32 i = 0; i< 20; i++) {
        auto [iter, ok] = h.insert(std::make_pair(i, 2000 + i));
        if(!ok)
        {
            iter.value() = 2000 + i;
        }
        REQUIRE(iter.key() == i);
        REQUIRE(iter.value() == 2000 + i);
    }
    REQUIRE(h.size() == 20);

    for(u32 i = 1; i< 20; i+=2) {
        bool erased = h.erase(i);
        REQUIRE(erased);
        auto iter = h.find(i);
        REQUIRE(iter == h.end());
    }
    REQUIRE(h.size() == 10);

    for(u32 i = 0; i< 20; i++) {
        auto [iter, ok] = h.insert(std::make_pair(i, 3000 + i));
        if(!ok)
        {
            iter.value() = 3000 + i;
        }
        REQUIRE(iter.key() == i);
        REQUIRE(iter.value() == 3000 + i);
    }
    REQUIRE(h.size() == 20);
}

TEST_CASE( "Hashtable supports large num of elements", "[hashtable]" ) {
    hashtable h;
    for(u32 i = 0; i< 1000000; i++) {
        auto [iter, ok] = h.insert(std::make_pair(i, 1000 + i));
        REQUIRE(iter.key() == i);
        REQUIRE(iter.value() == 1000 + i);
    }
    REQUIRE(h.size() == 1000000);
}