//
// Created by Jakub Tyrcha on 13/11/2017.
//

#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#include <iostream>
#include <cstdlib>
#include <tuple>
#include <memory>
#include <assert.h>
#include <limits>

using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i32 = int32_t;

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept {
    return N;
}

constexpr u32 prime_hashtable_sizes []=
        { 2, 5, 11, 17, 37, 67, 131, 257, 521, 1031,
          2053, 3079, 4099, 6151, 8209, 12289, 16411, 24593, 32771, 49157,
          65537, 98317, 131101, 196613, 262147, 393241, 524309, 786433, 1048583, 1572869,
          2097169, 3145739, 4194319, 6291469, 8388617, 12582917, 16777259, 25165843, 33554467, 50331653,
          67108879, 100663319, 134217757, 201326611, 268435459, 402653189, 536870923, 805306457, 1073741827, 1610612741,
        };

class prime_hashing_policy {
public:
    u32 hashtable_size(size_t elements) const noexcept {
        return *std::lower_bound(prime_hashtable_sizes, prime_hashtable_sizes + countof(prime_hashtable_sizes), elements);
    }
    u32 hashtable_index(u32 hash, u32 size) const noexcept {
        switch(size)
        {   case 2: return hash % 2;
            case 5: return hash % 5;
            case 11: return hash % 11;
            case 17: return hash % 17;
            case 37: return hash % 37;
            case 67: return hash % 67;
            case 131: return hash % 131;
            case 257: return hash % 257;
            case 521: return hash % 521;
            case 1031: return hash % 1031;
            case 2053: return hash % 2053;
            case 3079: return hash % 3079;
            case 4099: return hash % 4099;
            case 6151: return hash % 6151;
            case 8209: return hash % 8209;
            case 12289: return hash % 12289;
            case 16411: return hash % 16411;
            case 24593: return hash % 24593;
            case 32771: return hash % 32771;
            case 49157: return hash % 49157;
            case 65537: return hash % 65537;
            case 98317: return hash % 98317;
            case 131101: return hash % 131101;
            case 196613: return hash % 196613;
            case 262147: return hash % 262147;
            case 393241: return hash % 393241;
            case 524309: return hash % 524309;
            case 786433: return hash % 786433;
            case 1048583: return hash % 1048583;
            case 1572869: return hash % 1572869;
            case 2097169: return hash % 2097169;
            case 3145739: return hash % 3145739;
            case 4194319: return hash % 4194319;
            case 6291469: return hash % 6291469;
            case 8388617: return hash % 8388617;
            case 12582917: return hash % 12582917;
            case 16777259: return hash % 16777259;
            case 25165843: return hash % 25165843;
            case 33554467: return hash % 33554467;
            case 50331653: return hash % 50331653;
            case 67108879: return hash % 67108879;
            case 100663319: return hash % 100663319;
            case 134217757: return hash % 134217757;
            case 201326611: return hash % 201326611;
            case 268435459: return hash % 268435459;
            case 402653189: return hash % 402653189;
            case 536870923: return hash % 536870923;
            case 805306457: return hash % 805306457;
            case 1073741827: return hash % 1073741827;
            case 1610612741: return hash % 1610612741;
        };
        return -1;
    }
};

using key_type = u32;
using mapped_type = u32;

constexpr u8 max_offset_limit = 12;
constexpr float max_load_factor = 0.9f;

struct hash_table_internals : private prime_hashing_policy {
    static constexpr u8 SLOT_IS_EMPTY = 0;
    struct key_slot_t {
        key_type key;
        u8 encoded_isempty_offset; // 0 = empty, count - 1 = robin_hood_offset

        bool is_empty() const noexcept { return encoded_isempty_offset == SLOT_IS_EMPTY; }
        u8 read_offset() const noexcept { return encoded_isempty_offset - 1; }
    };
    std::unique_ptr<key_slot_t[]> keys_;
    std::unique_ptr<mapped_type[]> values_;
    u32 tablesize_;
    u32 entries_;

    void initialize(size_t size);
// if fails might reorder the hashtable and leaves key and value set to element that needs to be reinserted (might be different than the initial one!)
// was insertion successful, index to original item, boolean of overwrite if key matches prev entry
    std::tuple<bool, i32, bool> try_insert(key_type & key, mapped_type & value);
//std::tuple<i32, bool> try_insert(hash_table_internals * this_, key_type & key, mapped_type & value);
    bool remove(const key_type & key);
    i32 find(const key_type & key);
    i32 find_next(i32 index);
    void resize_and_rehash(size_t new_size);

    u32 get_table_index(u32 hash) const noexcept;
    u32 get_index_offseted(u32 index, u32 offset) const noexcept;
    size_t size() const noexcept;
    u8 max_offset() const noexcept;
};

u32 hash_table_internals::get_table_index(u32 hash) const noexcept {
    //return u32 ( (u64 ( hash ) * tablesize_) >> 32ull );
    return hashtable_index(hash, tablesize_);
}

u32 hash_table_internals::get_index_offseted(u32 index, u32 offset) const noexcept {
    u32 v = index + offset;
    return v < tablesize_ ? v : (v - tablesize_);
}

size_t hash_table_internals::size() const noexcept {
    return size_t (entries_);
}

u8 hash_table_internals::max_offset() const noexcept {
    return (u8)std::min(tablesize_ - 1, (u32)max_offset_limit);
}

void hash_table_internals::resize_and_rehash(size_t new_size) {
    hash_table_internals new_hashtable;
    new_hashtable.initialize(hashtable_size(new_size));

    if(size() == 0) {
        *this = std::move(new_hashtable);
        return;
    }

    i32 index = find_next(-1);
    while(index != -1) {
        key_type & k = keys_[index].key;
        mapped_type & v = values_[index];
        auto [success, index_, overwritten] = new_hashtable.try_insert(k, v);

        while(!success) {
            new_hashtable.resize_and_rehash(hashtable_size(new_size + 1));
            auto [success_re, index_, overwritten] = new_hashtable.try_insert(k, v);
            success = success_re;
        }

        index = find_next(index);
    }

    *this = std::move(new_hashtable);
}

void hash_table_internals::initialize(size_t size) {
    u32 buffer_size ( size );
    tablesize_ = buffer_size;
    entries_ = 0;
    if(buffer_size > 0) {
        keys_ = std::make_unique<hash_table_internals::key_slot_t[]>(buffer_size);
        values_ = std::make_unique<mapped_type[]>(buffer_size);
    }

    for(u32 i = 0; i<buffer_size; i++) {
        // init key to default value?
        // init value to default value?
        keys_[i].encoded_isempty_offset = hash_table_internals::SLOT_IS_EMPTY;
    }
}

std::tuple<bool, i32, bool> hash_table_internals::try_insert(key_type & key, mapped_type & value) {
    if(tablesize_ == 0) {
        return std::make_tuple(false, -1, false);
    }

    u32 hash = (u32)std::hash<key_type>{}(key);
    u32 index = get_table_index(hash);
    i32 original_index = -1;

    u8 offset = 0;
    const u8 max_offset_ = max_offset();

    while(offset <= max_offset_) {
        if(keys_[index].is_empty()) {
            keys_[index].key = key;
            keys_[index].encoded_isempty_offset = offset + 1;
            values_[index] = value;
            original_index = (original_index == -1) ? index : original_index;
            entries_++;
            return std::make_tuple(true, original_index, false);
        }
        if(keys_[index].key == key) {
            return std::make_tuple(true, index, true);
        }
        if(keys_[index].read_offset() < offset) {
            std::swap(keys_[index].key, key);
            std::swap(values_[index], value);
            original_index = (original_index == -1) ? index : original_index;
            u8 swapped_offset = keys_[index].read_offset();
            keys_[index].encoded_isempty_offset = offset + 1;
            offset = swapped_offset;
            hash = (u32)std::hash<key_type>{}(key);
        }

        offset++;
        index = get_index_offseted(index, 1);
    }

    return std::make_tuple(false, original_index, false);
};

bool hash_table_internals::remove(const key_type & key) {
    i32 index = find(key);
    if(index == -1) {
        return false;
    }
    keys_[index].encoded_isempty_offset = hash_table_internals::SLOT_IS_EMPTY;
    entries_--;
    u32 next_index = get_index_offseted(index, 1);
    while(!keys_[next_index].is_empty() && keys_[next_index].read_offset() > 0) {
        keys_[index].key = keys_[next_index].key;
        keys_[index].encoded_isempty_offset = keys_[next_index].encoded_isempty_offset - 1;
        keys_[next_index].encoded_isempty_offset = hash_table_internals::SLOT_IS_EMPTY;
        values_[index] = values_[next_index];

        index = next_index;
        next_index = get_index_offseted(index, 1);
    }
    return true;
}

i32 hash_table_internals::find(const key_type & key) {
    if(tablesize_ == 0) {
        return -1;
    }

    u32 hash = (u32)std::hash<key_type>{}(key);
    u32 index = get_table_index(hash);
    u8 offset = 0;
    const u8 max_offset_ = max_offset();

    while(offset <= max_offset_) {
        if(keys_[index].is_empty()) {
            return -1;
        }

        if(keys_[index].key == key) {
            return i32(index);
        }

        offset++;
        index = get_index_offseted(index, 1);
    }

    return -1;
}

i32 hash_table_internals::find_next(i32 index) {
    const i32 end ( tablesize_ );
    index++;
    for(; index < end; index++) {
        if(!keys_[index].is_empty()) {
            return index;
        }
    }
    return -1;
}

class hashtable : private prime_hashing_policy {
public:
    using key_type = u32;
    using mapped_type = u32;
    using value_type = std::pair<key_type, mapped_type>;
    using reference = mapped_type&;

    static constexpr i32 end_iter_index = std::numeric_limits<i32>::max();

    class iterator {
    public:
        const key_type & key() const;
        mapped_type & value() const;

        iterator& operator++();
        reference operator*() const;
        bool operator == (const iterator & rhs) const noexcept;
    private:
        friend class hashtable;

        iterator(hashtable * parent, i32 index) : hashtable_(parent), index_(index) {}

        hashtable * hashtable_;
        i32 index_;
    };

    class const_iterator {
    public:
        const key_type & key();
        const mapped_type & value();
    };

    hashtable() noexcept;

    iterator end() noexcept;

    std::pair<iterator,bool> insert ( const value_type & va );

    //iterator erase ( const_iterator position );
    size_t erase ( const key_type& k );

    iterator find ( const key_type& k );
    //const_iterator find ( const key_type& k ) const;

    void reserve(size_t size);

    size_t size() const noexcept {
        return internals_.size();
    }
private:
    void resize_and_rehash(size_t new_size);

    hash_table_internals internals_;
};

const hashtable::key_type & hashtable::iterator::key() const {
    return hashtable_->internals_.keys_[index_].key;
}

hashtable::reference hashtable::iterator::value() const {
    return hashtable_->internals_.values_[index_];
}

hashtable::iterator& hashtable::iterator::operator++() {
    index_ = hashtable_->internals_.find_next(index_);
    return *this;
}

hashtable::reference hashtable::iterator::operator*() const {
    return value();
}

bool hashtable::iterator::operator == (const hashtable::iterator & rhs) const noexcept {
    return hashtable_ == rhs.hashtable_ && index_ == rhs.index_;
}

hashtable::iterator hashtable::end() noexcept {
    return iterator {  this, end_iter_index };
}

hashtable::hashtable() noexcept {
    internals_.initialize(0);
}

void hashtable::resize_and_rehash(size_t new_size) {
    internals_.resize_and_rehash(new_size);
}

std::pair<hashtable::iterator,bool> hashtable::insert( const value_type & va ) {
    key_type k {va.first};
    mapped_type v {va.second};

    if(internals_.size() >= internals_.tablesize_ * max_load_factor) {
        resize_and_rehash(hashtable_size(internals_.tablesize_ + 1));
    }

    auto [success, index, collision] = internals_.try_insert(k, v);
    while(!success) {
        if(index != -1) {
            std::swap(internals_.keys_[index].key, k);
            std::swap(internals_.values_[index], v);
        }
        resize_and_rehash(hashtable_size(internals_.tablesize_ + 1));
        {
            auto [success_re, index_re, collision] = internals_.try_insert(k, v);
            success = success_re;
            index = index_re;
        }
    }

    return std::make_pair(iterator{this, index}, (index != -1) && !collision);
}

size_t hashtable::erase( const key_type & key) {
    return internals_.remove(key) ? 1 : 0;
}

hashtable::iterator hashtable::find( const key_type& k ) {
    i32 index = internals_.find(k);
    if(index != -1) {
        return iterator { this, index };
    }
    return end();
}

void hashtable::reserve(size_t size) {
    resize_and_rehash(size);
}

#endif //HASHTABLE_HASHTABLE_H
