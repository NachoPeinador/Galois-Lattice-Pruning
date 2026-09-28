
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include <cstdint>

extern "C" {
    // Bit-packing up to 6 modular projections into a single 64-bit integer
    inline uint64_t pack_state(const std::vector<int>& state, int num_oracles) {
        uint64_t key = 0;
        for(int i = 0; i < num_oracles; i++) {
            key = (key << 10) | state[i];
        }
        return key;
    }

    // FORWARD PHASE: Generate and store the first half of the lattice
    void generate_half1(int level, int end_level, int num_oracles, int* primes, int* basis_mod,
                       std::vector<int>& current_sums, std::vector<uint64_t>& keys) {
        if (level == end_level) {
            keys.push_back(pack_state(current_sums, num_oracles));
            return;
        }
        
        std::vector<int> original_sums = current_sums;
        for (int val = -1; val <= 1; val++) {
            if (val != 0) {
                for (int p_idx = 0; p_idx < num_oracles; p_idx++) {
                    int p = primes[p_idx];
                    current_sums[p_idx] = (original_sums[p_idx] + val * basis_mod[level * num_oracles + p_idx]) % p;
                    if (current_sums[p_idx] < 0) current_sums[p_idx] += p;
                }
            } else {
                current_sums = original_sums;
            }
            generate_half1(level + 1, end_level, num_oracles, primes, basis_mod, current_sums, keys);
        }
        current_sums = original_sums; // Backtrack
    }

    // BACKWARD PHASE: Generate the second half and search for collisions
    void generate_half2_and_count(int level, int end_level, int num_oracles, int* primes, int* basis_mod, int* target_mod,
                                  std::vector<int>& current_sums, const std::vector<uint64_t>& keys1, uint64_t& local_matches) {
        if (level == end_level) {
            std::vector<int> target_half1(num_oracles, 0);
            for(int i = 0; i < num_oracles; i++) {
                target_half1[i] = (target_mod[i] - current_sums[i]) % primes[i];
                if (target_half1[i] < 0) target_half1[i] += primes[i];
            }
            uint64_t target_key = pack_state(target_half1, num_oracles);
            
            // Massive binary search
            auto bounds = std::equal_range(keys1.begin(), keys1.end(), target_key);
            local_matches += std::distance(bounds.first, bounds.second);
            return;
        }
        
        std::vector<int> original_sums = current_sums;
        for (int val = -1; val <= 1; val++) {
            if (val != 0) {
                for (int p_idx = 0; p_idx < num_oracles; p_idx++) {
                    int p = primes[p_idx];
                    current_sums[p_idx] = (original_sums[p_idx] + val * basis_mod[level * num_oracles + p_idx]) % p;
                    if (current_sums[p_idx] < 0) current_sums[p_idx] += p;
                }
            } else {
                current_sums = original_sums;
            }
            generate_half2_and_count(level + 1, end_level, num_oracles, primes, basis_mod, target_mod, current_sums, keys1, local_matches);
        }
        current_sums = original_sums; // Backtrack
    }

    // ORCHESTRATOR
    uint64_t run_mitm_galois(int d, int num_oracles, int* primes, int* basis_mod, int* target_mod) {
        int d1 = d / 2;
        int d2 = d;
        
        std::vector<uint64_t> keys1;
        uint64_t reserve_size = std::pow(3, d1);
        keys1.reserve(reserve_size); 
        
        std::vector<int> initial_sums(num_oracles, 0);
        
        // 1. Forward Phase
        generate_half1(0, d1, num_oracles, primes, basis_mod, initial_sums, keys1);
        
        // 2. Sorting O(N log N)
        std::sort(keys1.begin(), keys1.end());
        
        uint64_t total_matches = 0;
        
        // 3. Backward Phase (Parallelized via OpenMP)
        #pragma omp parallel for reduction(+:total_matches)
        for (int val = -1; val <= 1; val++) {
            std::vector<int> current_sums(num_oracles, 0);
            if (val != 0) {
                for (int p_idx = 0; p_idx < num_oracles; p_idx++) {
                    int p = primes[p_idx];
                    current_sums[p_idx] = (val * basis_mod[d1 * num_oracles + p_idx]) % p;
                    if (current_sums[p_idx] < 0) current_sums[p_idx] += p;
                }
            }
            
            uint64_t local_matches = 0;
            generate_half2_and_count(d1 + 1, d2, num_oracles, primes, basis_mod, target_mod, current_sums, keys1, local_matches);
            total_matches += local_matches;
        }
        
        return total_matches; 
    }
}
