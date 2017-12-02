//
// Created by Jakub Tyrcha on 13/11/2017.
//

#include "utilities.h"
#include <vector>
#include <numeric>
#include <boost/hana/functional/compose.hpp>
namespace hana = boost::hana;

bool is_prime(size_t i) {
    for(size_t j = 2; j*j <= i; ++j) {
        if((i % j) == 0) {
            return false;
        }
    }
    return true;
}

size_t next_prime(size_t v) {
    while(!is_prime(v++)) {}
    return --v;
}

std::vector<size_t> generate_prime_size() {
    std::vector<size_t> sizes;
    sizes.resize(50);
    std::iota(sizes.begin(), sizes.end(), 0);
    std::transform(sizes.begin(), sizes.end(), sizes.begin(),
                   hana::compose([](size_t v){
                       return next_prime(v);
                   },[](size_t v){
                       constexpr size_t exp_b = 10;
                       if(v < exp_b) {
                           return 2 << v;
                       }
                       else {
                           return (2 << (exp_b + (v - exp_b) / 2))
                                  + ((v % 2) ? 2 << (exp_b + (v - exp_b) / 2 - 1) : 0);
                       }
                   })
    );
    return sizes;
}

void print_prime_tables() {
    auto sizes = generate_prime_size();

    printf("{ ");
    size_t i=1;
    for(auto s : sizes) {
        printf("%lu, ", s); if(((i++) % 10) == 0) printf("\n");
    }
    printf("};");

    printf("\n");

    printf("{ ");
    for(auto s : sizes) {
        printf("case %lu: return hash %% %lu;\n", s, s);
    }
    printf("};");
}
