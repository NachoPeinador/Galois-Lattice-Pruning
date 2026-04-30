# Galois Invariants in Cyclotomic Lattice Enumeration

[![Paper Status](https://img.shields.io/badge/Paper-Under%20Review-blue.svg)](https://link-to-your-paper-or-zenodo-preprint.com)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C++-OpenMP-00599C.svg)]()
[![Python](https://img.shields.io/badge/Python-3.x-3776AB.svg)]()

Official code repository and supplementary material for the paper **"Galois Invariants in Cyclotomic Lattice Enumeration: Deterministic Collapse of the SVP Search Space via Modular Projection Pruning"** (Submitted to *SEMA Journal*, Springer Nature).

## 📌 Overview

The security of Post-Quantum Cryptography (PQC), specifically Module-Lattice-Based Key-Encapsulation Mechanisms like **ML-KEM**, relies on the assumed exponential hardness of the Shortest Vector Problem (SVP). Current security models assume that the injected Learning With Errors (LWE) noise is ergodic and uniformly distributed in the phase space. 

This repository provides an empirical framework that proves the **non-ergodicity** of this noise. By exploiting the ideal structure in cyclotomic rings $\mathbb{Z}[x]/\langle x^n + 1 \rangle$, we apply simultaneous arithmetic constraints (Galois Pruning) to intercept algebraically inconsistent branches. 

Through a highly optimized **Meet-in-the-Middle (MitM)** architecture written in C++ with OpenMP, we demonstrate that the SVP search space collapses deterministically, following the law of Oracle Independence:

$$|\Omega_{conf}| \approx \frac{|\Omega|}{\prod_{i=1}^k p_i}$$

## 🚀 Key Experimental Results

Our MitM engine evaluates the total number of surviving candidate vectors in dimensions up to $d=32$ under ternary noise $\{-1, 0, 1\}$. The intersection of just 5 modular oracles elides **>99.99999%** of the original search space, proving a formal violation of the Eigenstate Thermalization Hypothesis (ETH) in structured lattices.

| Dim (d) | Total Leaves ($3^d$) | Oracles (k) | Survivors (Empirical C++) | Theoretical Expectation |
|---------|------------------------|-------------|---------------------------|-------------------------|
| 20      | 3,486,784,401          | 1           | 35,945,565                | 35,946,230              |
| 20      | 3,486,784,401          | 3           | 3,452                     | 3,455                   |
| 20      | 3,486,784,401          | 5           | 0                         | 0                       |
| 32      | 1,853,020,188,851,841  | 1           | 19,103,300,900,235        | 19,103,300,915,998      |
| 32      | 1,853,020,188,851,841  | 3           | 1,836,316,436             | 1,836,326,147           |
| 32      | 1,853,020,188,851,841  | 5           | 157,517                   | 157,448                 |

## 📂 Repository Structure

* `SVP_Galois_Pruning_Framework.ipynb`: The main interactive Google Colab notebook containing the mathematical didactics, the C++ MitM engine compilation, and the execution pipeline.
* `/src/`: (Optional) Directory containing the raw C++ source files (`galois_mitm.cpp`) for local compilation.

## 🛠️ Requirements and Usage

The code is designed to run seamlessly on **Google Colab**, requiring no local setup. It automatically compiles the C++ shared library using `g++` with `-fopenmp` and `-O3` flags for maximum performance, and links it to Python via `ctypes`.

If you prefer to run it locally on a Linux environment, ensure you have:
* `g++` (with OpenMP support)
* `Python 3.x`
* `NumPy`

### Quick Start
1. Open `SVP_Galois_Pruning_Framework.ipynb` in Google Colab.
2. Run all cells sequentially.
3. The C++ engine will compile, and the experiment will execute, replicating the exact dimensional collapse table shown above.

---

## ⚖️ Licensing

This repository operates under a **Dual License** model to protect the non-commercial nature of the research while encouraging open academic collaboration:

1. **Code & Software (`Notebooks/`, `/src/` and scripts):**
   Released under the [PolyForm Noncommercial License 1.0.0](https://polyformproject.org/licenses/noncommercial/1.0.0). 
   *You are free to use, modify, and share the code for academic, personal, or educational purposes. Any commercial use, monetization, or integration into proprietary paid software is strictly prohibited.*

2. **Manuscripts & Visual Assets (`Papers/` and `Images/`):**
   Released under the [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).
   *You are free to share and adapt the theoretical text and graphics for non-commercial purposes, provided you give appropriate credit and distribute your contributions under the exact same license.*

---

## 📝 Citation

<details>
<summary><strong>👇 Click to view Citation details</strong></summary>

If this Galois pruning mechanism, the Meet-in-the-Middle (MitM) C++ architecture, or the Law of Oracle Independence ($|\Omega_{conf}| \approx \frac{|\Omega|}{\prod p_i}$) assists in your research, please cite the corresponding manuscript:

**BibTeX:**

```bibtex
@article{peinador2026galois,
  author = {Peinador Sala, Jos{\'e} Ignacio},
  title = {Galois Invariants in Cyclotomic Lattice Enumeration: Deterministic Collapse of the SVP Search Space via Modular Projection Pruning},
  journal = {SEMA Journal},
  year = {2026},
  note = {Under Review},
  url = {[https://github.com/NachoPeinador/Galois-Lattice-Pruning](https://github.com/NachoPeinador/Galois-Lattice-Pruning)}
}

</details>

---
