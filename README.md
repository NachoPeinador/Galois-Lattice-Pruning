# Galois Invariants in Cyclotomic Lattice Enumeration

[![C++](https://img.shields.io/badge/C++-OpenMP-00599C.svg)]()
[![Python](https://img.shields.io/badge/Python-3.x-3776AB.svg)]()
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.19920083.svg)](https://doi.org/10.5281/zenodo.19920083)
[![ORCID](https://img.shields.io/badge/ORCID-0009--0008--1822--3452-A6CE39?style=flat&logo=orcid&logoColor=white)](https://orcid.org/0009-0008-1822-3452)
[![X](https://img.shields.io/badge/X-%40todos__lumpen-000000?style=flat&logo=x&logoColor=white)](https://twitter.com/todos_lumpen)
[![Papers](https://img.shields.io/badge/Paper-Read_PDF-B31B1B?style=flat&logo=latex&logoColor=white)](https://github.com/NachoPeinador/Galois-Lattice-Pruning/blob/main/Paper/Galois_Invariants_in_Cyclotomic_Lattice_Enumeration.pdf)

Official code repository and supplementary material for the paper **"Galois Invariants in Cyclotomic Lattice Enumeration: An Efficient Meet-in-the-Middle Attack on Ring-LWE under Partial Key Exposure"** (Submitted to *Journal of Cryptographic Engineering*).

---

## 🎯 TL;DR – The Essentials

### 🔬 **Theoretical & Computational Breakthroughs**

* 🛡️ **Galois Pruning:** Arithmetic superselection —projecting ternary secret vectors onto the residue fields of prime ideals in ℤ[x]/(xⁿ+1)— eliminates algebraically inconsistent branches before they can proliferate.
* 📐 **Law of Oracle Independence:** The number of survivors collapses as |Ω_conf| = 3ᵈ / ∏ qᵢ, where qᵢ = pᵢ^{fᵢ} are the **true ideal norms**. Empirically validated up to d=32 with k=3 leaked ideals (>99.999999991% node elision).
* 🔗 **Galois Group Reduction:** Conjugate prime ideals are tied by Frobenius automorphisms; learning one residue automatically reveals all g = n/f conjugates. This effectively halves the number of independent side‑channel oracles.
* 🧮 **Extrapolation to ML‑KEM:** In the NIST standard ring ℚ(ζ₅₁₂) (n=256), two leaked inert‑prime residues (p=3 and p=5) already make the secret unique.
* 📏 **Area‑Law Entropy:** The configuration entropy of survivors does **not** grow with the volume 3ᵈ but saturates to a constant once ∏ qᵢ > 3ᵈ, a hallmark of non‑ergodic phases.
* 🔀 **Adaptive Two‑Phase Search:** A prioritised search strategy, inspired by quantum modular algorithms, is validated on a fully enumerated d=8 instance.
* 🎲 **Marginal Uniformity:** Without leakage, individual residues are statistically indistinguishable from uniform (Wasserstein distance tests). The attack **requires** the side channel.

### ⚡ **Core Experimental Result (d=32, ℚ(ζ₆₄))**

| k | ∏ qᵢ | Survivors (emp.) | Survivors (th.) | Pruning |
|---|-------|------------------|-----------------|---------|
| 1 | 4.30×10⁷ | 43 043 215 | 43 046 721 | 99.9999977% |
| 2 | 6.94×10¹¹ | 2 696 | 2 668 | 99.9999999999% |
| 3 | 1.34×10¹⁴ | **13** | **13** | 100.0000000000% |

*Empirical counts from the C++/OpenMP MitM engine (128‑bit packed keys) match the theoretical prediction with striking accuracy.*

<p align="center">
  <img src="Images/search_space_collapse.png" alt="Deterministic Search Space Collapse via Galois Pruning" width="100%">
  <br>
  <em>Figure 1. Deterministic collapse of the LWE search space in dimension d=32. The empirical results (red points) obtained via the C++ MitM engine perfectly align with the theoretical Law of Oracle Independence (dashed line).</em>
</p>

### 💡 **Key Insight**

> Ring‑LWE security does **not** rest solely on the generic hardness of SVP. The arithmetic structure of cyclotomic rings —specifically, the inert prime ideals and their Galois symmetries— acts as a deterministic superselection rule. A small amount of modular projection leakage (obtainable, e.g., through a physical side channel) is sufficient to collapse the search space by dozens of orders of magnitude. This calls for a serious re‑evaluation of side‑channel resistance in lattice‑based PQC implementations.

---

## 📌 Overview

Post‑quantum standards such as **ML‑KEM** (FIPS 203) assume that the LWE error term behaves as an ergodic, uniformly distributed noise, making the underlying SVP exponentially hard. In this work we show that **partial key exposure** —the exact modular residue of the secret polynomial modulo a few prime ideals— can be combined with the Galois structure of cyclotomic rings to **deterministically** prune the search tree.

Our threat model assumes a physical side channel (power, EM, timing) has leaked the residues $\pi_{\mathfrak{p}_i}(s)$. The attacker then runs a highly optimised Meet‑in‑the‑Middle engine that:

1. Packs up to 4 residues into a single 128‑bit integer,
2. Sorts $3^{d/2}$ forward keys in $O(N\log N)$,
3. Counts matches with `std::equal_range` in the backward phase,
4. Recovers the secret by testing the few survivors against the public key.

The engine is written in **C++ with OpenMP** and requires only a commodity cloud instance (2 vCPUs, 12 GB RAM).

---

## 📂 Repository Structure

| Path | Description |
|------|-------------|
| `Paper/` | LaTeX source and compiled PDF of the manuscript submitted to the *Journal of Cryptographic Engineering*. |
| `Notebooks/` | The main interactive Colab notebook (`SVP_Galois_Pruning_Framework.ipynb`) that compiles the MitM engine, executes every experiment reported in the paper, and provides guided markdown explanations. |
| `src/` | Standalone C++ source files (`galois_mitm_ideal.cpp`) for local compilation outside Colab. |
| `Images/` | Figures and diagnostic plots (search‑space collapse, Galois orbits, ML‑KEM extrapolation). |
| `README.md` | This file. |

---

## 🛠️ Quick Start

[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/NachoPeinador/Galois-Lattice-Pruning/blob/main/Notebooks/Galois_Pruning_Attack_on_Ring_LWE.ipynb)

1. Click the **Open in Colab** badge above.
2. Run all cells sequentially (`Runtime → Run all`).
3. The notebook compiles the C++ engine and executes every experiment reported in the paper. All random seeds are fixed for reproducibility.

**Local Linux requirements (optional):** `g++` with OpenMP, Python 3, NumPy, SciPy.

---

## ⚖️ Licensing

This repository operates under a **Dual License** model:

| Asset | License |
|-------|---------|
| **Code & Software** (`Notebooks/`, `src/`) | [PolyForm Noncommercial 1.0.0](https://polyformproject.org/licenses/noncommercial/1.0.0) |
| **Manuscript & Visual Assets** (`Paper/`, `Images/`) | [CC BY‑NC‑SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) |

*Academic, educational, and personal use is encouraged. Commercial use of the software requires a separate agreement.*

---

## 📝 Citation

If you use this framework or the theoretical derivations in your research, please cite:

**BibTeX:**
```bibtex
@article{peinador2026galois,
  author  = {Peinador Sala, José Ignacio},
  title   = {Galois Invariants in Cyclotomic Lattice Enumeration:
             An Efficient Meet-in-the-Middle Attack on Ring-LWE
             under Partial Key Exposure},
  journal = {Zenodo},
  year    = {2026},
  note    = {Submitted to Journal of Cryptographic Engineering},
  url     = {https://doi.org/10.5281/zenodo.19920083}
}
```

**APA:**
> Peinador Sala, J. I. (2026). *Galois Invariants in Cyclotomic Lattice Enumeration: An Efficient Meet‑in‑the‑Middle Attack on Ring‑LWE under Partial Key Exposure* (Version v2). Zenodo. https://doi.org/10.5281/zenodo.19920083

---

## 🔭 Philosophical Context

> *“Nothing in nature is random... A thing appears random only through the incompleteness of our knowledge.”* — **Baruch Spinoza**

For years, the injected LWE noise has been treated as an impenetrable fog —a perfect stochastic thermalization over the phase space. This geometric axiom built the insurmountable exponential walls of modern post‑quantum cryptography.

This work looks at the cyclotomic lattice through the rigid architecture of **Galois fields**. Modular projections are not passive features; they are deterministic superselection rules that confine the “random noise” to a vanishingly small algebraic sub‑manifold. The noise is not truly ergodic —it is tightly bound by arithmetic laws.

The engine and the theoretical framework were crafted outside traditional academic silos. They stand as a reminder that supposedly unbreakable cryptographic standards can be critically challenged by combining mathematical curiosity, elegant computational architectures, and the courage to question foundational axioms.

> *“Do not try and bend the spoon, that’s impossible. Instead, only try to realize the truth... There is no spoon.”* — **Spoon Boy (The Matrix)**

---

<div align="center">
<b>Last Update:</b> May 2026 | <b>Status:</b> Submitted to <i>Journal of Cryptographic Engineering</i> | Built with ⚙️ & 🐍
</div>
