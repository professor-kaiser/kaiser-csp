#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../doctest.h"

#include "../../../src/csp/core/expression/integer/LinearExpression.hpp"
#include "../../../src/csp/core/expression/integer/ScaleExpression.hpp"
#include "../../../src/csp/core/expression/integer/SumExpression.hpp"
#include "../../../src/csp/core/expression/operation.hpp"
#include "../../../src/csp/core/expression/structure.hpp"
#include "../../../src/csp/core/arc/arc.hpp"
#include <memory>
#include <vector>
#include <stack>
#include <array>
#include <numeric>
#include <functional>
#include <chrono>
#include <iostream>

using namespace kaiser::csp::core::expression;
using namespace kaiser::csp::core::arc;
using namespace kaiser::csp::core::expression::structure;

TEST_CASE("[LinearExpression]")
{
    auto var = std::make_shared<LinearExpression<int>>(Bounds<int>{}, 0);

    int result = eval<int>(var, std::array {1, 2, 3});
}

TEST_CASE("[ScaleExpression] : operator *")
{
    auto a = std::make_shared<LinearExpression<int>>(Bounds<int>{}, 0);

    auto expr0 = a * 2;
    auto expr1 = 2 * a;

    int result0 = eval(expr0, std::array {102});
    int result1 = eval(expr1, std::array {90});

    CHECK(result0 == 204);
    CHECK(result1 == 180);
}

TEST_CASE("[SumExpression] : operator +")
{
    auto a = std::make_shared<LinearExpression<int>>(Bounds<int>{}, 0);
    auto b = std::make_shared<LinearExpression<int>>(Bounds<int>{}, 1);

    auto expr = a + b;
    auto sum0 = a + 10;
    auto sum1 = a + 20;

    int result = eval<int>(expr, std::array {10, 23});
    int res_sum0 = eval(sum0, std::array {15});
    int res_sum1 = eval(sum1, std::array {15});

    CHECK(res_sum0 == 25);
    CHECK(res_sum1 == 35);
    CHECK(result == 33);
}

TEST_CASE("[AC3]")
{
    auto var = [](Bounds<int> domains, int idx, int coeff = 1)
    {
        return std::make_shared<LinearExpression<int>>(domains, idx, coeff);
    };

    auto A = var({{0, 2}}, 0);
    auto B = var({{0, 2}}, 1);

    auto a_vec = to_vector(A->domains);
    auto b_vec = to_vector(B->domains);

    auto start_time = std::chrono::high_resolution_clock::now(); 

    auto constraint = [](int a, int b) {
        return a > b;
    };

    revise<int>(a_vec, b_vec, constraint);

    revise<int>(b_vec, a_vec, [&](int b, int a) {
        return constraint(a, b);
    });

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution : " << elapsed.count() << " ms\n";

    CHECK(a_vec == std::vector {1, 2});
    CHECK(b_vec == std::vector {0, 1});
}

TEST_CASE("[Domains propagation]")
{
    auto var = [](Bounds<int> domains, int idx, int coeff = 1)
    {
        return std::make_shared<LinearExpression<int>>(domains, idx, coeff);
    };

    auto X = var({{-10, 0}}, 0);

    auto expr = 2*X + 5;

    expr->propagate_domains({{0, 0}});

    std::cout << "Domains of X after propagation: " << std::endl;
    for (const auto& bound : X->domains)
        std::cout << "bound: " << bound.low << ", " << bound.high << std::endl;
}

/*
TEST_CASE("[Test]")
{
    auto var = [](Bounds<int> domains, int idx, int coeff = 1)
    {
        return std::make_shared<LinearExpression<int>>(domains, idx, coeff);
    };

    auto S = var({{1, 9}}, 0);
    auto E = var({{0, 9}}, 1);
    auto N = var({{0, 9}}, 2);
    auto D = var({{0, 9}}, 3);
    auto M = var({{1, 9}}, 4);
    auto O = var({{0, 9}}, 5);
    auto R = var({{0, 9}}, 6);
    auto Y = var({{0, 9}}, 7);

    std::vector<std::shared_ptr<ExpressionBase<int>>> variables = {S, E, N, D, M, O, R, Y};

    auto SEND  = 1000*S + 100*E + 10*N + D;
    auto MORE  = 1000*M + 100*O + 10*R + E;
    auto MONEY = 10000*M + 1000*O + 100*N + 10*E + Y;

    auto constraint = SEND + MORE - MONEY;
    auto flat = constraint->flatten();
    std::vector<std::pair<int,int>> terms(flat.coeffs.begin(), flat.coeffs.end());
    const int flat_const = flat.constant;

    std::vector<int> solutions;
    size_t n_solutions = 0;

    std::array<int, 8> data{};
    bool used[10] = {};

    std::function<void(int)> bt = [&](int depth)
    {
        if (depth == 8)
        {
            // int sum = flat_const;
            // for (const auto& [idx, coeff] : terms)
            //     sum += coeff * data[idx];

            if (eval(terms, flat_const, data) == 0)
            {
                solutions.insert(solutions.end(), data.begin(), data.end());
                n_solutions++;
            }
            return;
        }

        for (const auto& bound : variables[depth]->domains)
        {
            for (int d = bound.low; d <= bound.high; ++d)
            {
                if (used[d]) continue;
                data[depth] = d;
                used[d]     = true;
                bt(depth + 1);
                used[d]     = false;
            }
        }
    };

    auto start_time = std::chrono::high_resolution_clock::now();
    bt(0);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution : " << elapsed.count() << " ms\n";

    auto result = eval<int>(constraint, std::array{9, 5, 6, 7, 1, 0, 8, 2});
    CHECK(result == 0);
    CHECK(n_solutions == 1);
}
*/
