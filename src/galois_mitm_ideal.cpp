
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include <cstdint>

typedef unsigned __int128 uint128_t;

extern "C" {
    inline uint128_t pack_state(const std::vector<int>& state, int num_oracles) {
        uint128_t key = 0;
        for(int i = 0; i < num_oracles; i++) {
            key = (key << 28) | (uint128_t)(state[i] & 0x0FFFFFFF);
        }
        return key;
    }

    void generate_half1(int level, int end_level, int num_oracles, int* qvals, int* basis_mod,
                        std::vector<int>& current_sums, std::vector<uint128_t>& keys) {
        if (level == end_level) {
            keys.push_back(pack_state(current_sums, num_oracles));
            return;
        }
        std::vector<int> original_sums = current_sums;
        for (int val = -1; val <= 1; val++) {
            if (val != 0) {
                for (int p_idx = 0; p_idx < num_oracles; p_idx++) {
                    int q = qvals[p_idx];
                    long long add = (long long)val * basis_mod[level * num_oracles + p_idx];
                    current_sums[p_idx] = (int)(((long long)original_sums[p_idx] + add) % q);
                    if (current_sums[p_idx] < 0) current_sums[p_idx] += q;
                }
            } else {
                current_sums = original_sums;
            }
            generate_half1(level + 1, end_level, num_oracles, qvals, basis_mod, current_sums, keys);
        }
        current_sums = original_sums;
    }

    void generate_half2_and_count(int level, int end_level, int num_oracles, int* qvals, int* basis_mod, int* target_mod,
                                  std::vector<int>& current_sums, const std::vector<uint128_t>& keys1, uint64_t& local_matches) {
        if (level == end_level) {
            std::vector<int> target_half1(num_oracles, 0);
            for(int i = 0; i < num_oracles; i++) {
                target_half1[i] = (target_mod[i] - current_sums[i]) % qvals[i];
                if (target_half1[i] < 0) target_half1[i] += qvals[i];
            }
            uint128_t target_key = pack_state(target_half1, num_oracles);
            auto bounds = std::equal_range(keys1.begin(), keys1.end(), target_key);
            local_matches += std::distance(bounds.first, bounds.second);
            return;
        }
        std::vector<int> original_sums = current_sums;
        for (int val = -1; val <= 1; val++) {
            if (val != 0) {
                for (int p_idx = 0; p_idx < num_oracles; p_idx++) {
                    int q = qvals[p_idx];
                    long long add = (long long)val * basis_mod[level * num_oracles + p_idx];
                    current_sums[p_idx] = (int)(((long long)original_sums[p_idx] + add) % q);
                    if (current_sums[p_idx] < 0) current_sums[p_idx] += q;
                }
            } else {
                current_sums = original_sums;
            }
            generate_half2_and_count(level + 1, end_level, num_oracles, qvals, basis_mod, target_mod, current_sums, keys1, local_matches);
        }
        current_sums = original_sums;
    }

    uint64_t run_mitm_galois_ideal(int d, int num_oracles, int* qvals, int* basis_mod, int* target_mod) {
        int d1 = d / 2;
        int d2 = d;
        std::vector<uint128_t> keys1;
        keys1.reserve((size_t)std::pow(3, d1));
        std::vector<int> initial_sums(num_oracles, 0);
        generate_half1(0, d1, num_oracles, qvals, basis_mod, initial_sums, keys1);
        std::sort(keys1.begin(), keys1.end());
        uint64_t total_matches = 0;
        #pragma omp parallel for reduction(+:total_matches)
        for (int val = -1; val <= 1; val++) {
            std::vector<int> current_sums(num_oracles, 0);
            if (val != 0) {
                for (int p_idx = 0; p_idx < num_oracles; p_idx++) {
                    int q = qvals[p_idx];
                    long long add = (long long)val * basis_mod[d1 * num_oracles + p_idx];
                    current_sums[p_idx] = (int)(add % q);
                    if (current_sums[p_idx] < 0) current_sums[p_idx] += q;
                }
            }
            uint64_t local_matches = 0;
            generate_half2_and_count(d1 + 1, d2, num_oracles, qvals, basis_mod, target_mod, current_sums, keys1, local_matches);
            total_matches += local_matches;
        }
        return total_matches;
    }
}
