#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/csp/core/Problem.hpp"
#include "../src/csp/solver/BacktrackingSolver.hpp"
#include "../src/csp/core/constraint/AccumulateConstraint.hpp"
#include "../src/csp/core/constraint/ConstraintComposite.hpp"
#include "../src/csp/core/constraint/DistinctConstraint.hpp"
#include "../src/csp/core/constraint/MappedValueConstraint.hpp"
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace kaiser::csp::core;
using namespace kaiser::csp::solver;

TEST_CASE("CRYPTO-ARITHME")
{
    /**
     * @brief Utility function to generate a contiguous integer range [begin, end).
     */
    auto range = [](int begin, int end)
    {
        std::vector<int> v(end - begin);
        std::iota(v.begin(), v.end(), begin);
        return v;
    };

    /**
     * Variable indexing:
     *  s:0, e:1, n:2, d:3, m:4, o:5, r:6, y:7
     */
    size_t n_variables = 8;

    /// Container holding all CSP variables
    std::shared_ptr<Variable> variables = std::make_shared<Variable>();

    /**
     * Domain definition:
     * Each variable takes a value in [0, 9]
     */
    std::vector<int> domains = range(0, 10);

    /// Assign the same domain to all variables
    for (size_t i = 0; i < n_variables; i++)
        variables->add(domains);

    /**
     * @brief Accumulation mapping used for arithmetic constraint.
     *
     * This defines how transformed values are combined.
     * Here, index 2 corresponds to MONEY = SEND + MORE.
     */
    std::map<int, std::vector<int>> sum = {
        {2, {0, 1}}  // money = send + more
    };

    /**
     * @brief Constraint enforcing non-zero leading digits.
     *
     * Leading letters (S and M) must not be equal to 0.
     */
    std::map<int, std::vector<int>> first_letters = {
        {0, {0}},  // S != 0
        {4, {0}}   // M != 0
    };

    /**
     * @brief Transforms raw variable assignments into semantic values.
     *
     * This function is applied before evaluating the AccumulateConstraint.
     * It converts digit assignments into actual numbers:
     *
     *  SEND  = 1000*S + 100*E + 10*N + D
     *  MORE  = 1000*M + 100*O + 10*R + E
     *  MONEY = 10000*M + 1000*O + 100*N + 10*E + Y
     *
     * @param data Raw assignment vector (size must be >= 8)
     * @return A vector containing {SEND, MORE, MONEY}
     */
    auto transformer = [](const std::vector<int>& data)
    {
        if (data.size() < 8)
            return std::vector<int> {};

        int s = data[0], e = data[1], n = data[2], d = data[3];
        int m = data[4], o = data[5], r = data[6], y = data[7];

        int send  = 1000*s + 100*e + 10*n + d;
        int more  = 1000*m + 100*o + 10*r + e;
        int money = 10000*m + 1000*o + 100*n + 10*e + y;

        return std::vector<int> {send, more, money};
    };

    /**
     * @brief Composite constraint aggregating all problem constraints.
     *
     * Includes:
     *  - AllDifferent constraint on all variables
     *  - Non-zero constraint on leading digits
     *  - Arithmetic constraint: SEND + MORE = MONEY
     */
    std::shared_ptr<Constraint> constraints =
        std::make_shared<ConstraintComposite>()
            ->add(std::make_shared<DistinctConstraint>(range(0, 8)))
            ->add(std::make_shared<MappedValueConstraint>(
                first_letters,
                MappedValueConstraint::Operation::NEQ))
            ->add(std::make_shared<AccumulateConstraint>(
                sum,
                [](int a, int b) { return a + b; },
                0,
                transformer   // transformation is applied before constraint evaluation
            ));

    /// Problem definition
    Problem problem {variables, constraints};

    /// Solver instantiation (backtracking)
    BacktrackingSolver solver(problem);

    /// Solve the CSP
    solver.solve();

    /// Retrieve flattened solutions
    std::vector<int> solutions = solver.get_solutions();

    CHECK(solutions == std::vector<int>{9, 5, 6, 7, 1, 0, 8, 2});
}