# Quick Start — kaiser CSP Library

A minimal guide to solving constraint satisfaction problems with `kaiser::csp`.

---

## Overview

This example solves the classic **SEND + MORE = MONEY** cryptarithmetic puzzle, where each letter maps to a unique digit (0–9) and the arithmetic equation holds.

---

## 1. Include Headers

```cpp
#include "kaiser/csp/core/Problem.hpp"
#include "kaiser/csp/solver/BacktrackingSolver.hpp"
#include "kaiser/csp/core/constraint/AccumulateConstraint.hpp"
#include "kaiser/csp/core/constraint/ConstraintComposite.hpp"
#include "kaiser/csp/core/constraint/DistinctConstraint.hpp"
#include "kaiser/csp/core/constraint/MappedValueConstraint.hpp"
```

---

## 2. Define Variables & Domains

Each letter (`S E N D M O R Y`) is a variable with domain `[0, 9]`.

```cpp
using namespace kaiser::csp::core;
using namespace kaiser::csp::solver;

// Variable indices: s=0, e=1, n=2, d=3, m=4, o=5, r=6, y=7
size_t n_variables = 8;
std::shared_ptr<Variable> variables = std::make_shared<Variable>();

auto range = [](int begin, int end) {
    std::vector<int> v(end - begin);
    std::iota(v.begin(), v.end(), begin);
    return v;
};

for (size_t i = 0; i < n_variables; i++)
    variables->add(range(0, 10));  // domain [0, 9]
```

---

## 3. Define Constraints

### 3a. All-Different

All letters must map to distinct digits.

```cpp
auto distinct = std::make_shared<DistinctConstraint>(range(0, 8));
```

### 3b. Non-Zero Leading Digits

`S` (index 0) and `M` (index 4) must not be zero.

```cpp
std::map<int, std::vector<int>> first_letters = {
    {0, {0}},  // S != 0
    {4, {0}}   // M != 0
};
auto no_leading_zero = std::make_shared<MappedValueConstraint>(
    first_letters,
    MappedValueConstraint::Operation::NEQ
);
```

### 3c. Arithmetic Equation (SEND + MORE = MONEY)

Define a transformer that converts raw digit assignments into word values, then constrain their sum.

```cpp
// Transformer: digit vector → {SEND, MORE, MONEY}
auto transformer = [](const std::vector<int>& data) -> std::vector<int> {
    if (data.size() < 8) return {};
    int s=data[0], e=data[1], n=data[2], d=data[3];
    int m=data[4], o=data[5], r=data[6], y=data[7];
    return {
        1000*s + 100*e + 10*n + d,          // SEND
        1000*m + 100*o + 10*r + e,          // MORE
        10000*m + 1000*o + 100*n + 10*e + y // MONEY
    };
};

// sum[2] = sum[0] + sum[1]  →  MONEY = SEND + MORE
std::map<int, std::vector<int>> sum = { {2, {0, 1}} };

auto arithmetic = std::make_shared<AccumulateConstraint>(
    sum,
    [](int a, int b) { return a + b; },
    0,
    transformer
);
```

---

## 4. Compose & Solve

```cpp
std::shared_ptr<Constraint> constraints =
    std::make_shared<ConstraintComposite>()
        ->add(distinct)
        ->add(no_leading_zero)
        ->add(arithmetic);

Problem problem { variables, constraints };
BacktrackingSolver solver(problem);
solver.solve();
```

---

## 5. Read Solutions

Solutions are stored as a flat vector. Each block of `variables->size()` values is one solution.

```cpp
std::vector<int> solutions = solver.get_solutions();
size_t vars = variables->size();  // 8

std::cout << "Solutions found: " << solutions.size() / vars << "\n";

for (size_t i = 0; i < solutions.size(); i += vars) {
    // s=solutions[i+0], e=i+1, n=i+2, d=i+3,
    // m=i+4, o=i+5, r=i+6, y=i+7
    for (size_t j = 0; j < vars; j++)
        std::cout << solutions[i + j] << " ";
    std::cout << "\n";
}
```

---

## Key Concepts

| Component | Role |
|---|---|
| `Variable` | Holds all variables and their domains |
| `DistinctConstraint` | Enforces all-different on given indices |
| `MappedValueConstraint` | Bans or requires specific values for variables |
| `AccumulateConstraint` | Arithmetic constraint with optional transformer |
| `ConstraintComposite` | Combines multiple constraints via logical AND |
| `BacktrackingSolver` | Solves by systematic backtracking |

---

## Variable Index Reference (this example)

| Index | Letter |
|---|---|
| 0 | S |
| 1 | E |
| 2 | N |
| 3 | D |
| 4 | M |
| 5 | O |
| 6 | R |
| 7 | Y |