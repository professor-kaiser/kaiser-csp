#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"

#include "../../src/csp/core/expressions/LinearExpression.hpp"
#include "../../src/csp/core/expressions/ScaleExpression.hpp"
#include "../../src/csp/core/expressions/SumExpression.hpp"
#include "../../src/csp/core/expressions/operation.hpp"
#include "../../src/csp/core/structure/operation.hpp"
#include <memory>
#include <vector>
#include <stack>
#include <array>
#include <numeric>
#include <functional>
#include <chrono>
#include <iostream>

using namespace kaiser::csp::core::expression;
using namespace kaiser::csp::core::structure;

// g++ -std=c++20 expression.test.cpp ../../src/csp/core/structure/*.cpp ../../src/csp/core/expressions/*.cpp -o expression

auto& reg = ExpressionRegistry::instance();

TEST_CASE("[LinearExpression]")
{
    auto domains = make_union({});
    auto var = make_expression<LinearExpression>(domains, 0);

    auto& expr = reg.at(var->id);
    
    int result0 = eval(var->flatten(), std::array {10});
    int result1 = eval(expr->flatten(), std::array {10});

    CHECK(result0 == 10);
    CHECK(result1 == 10);
}

TEST_CASE("[ScaleExpression] : operator *")
{
    auto domains = make_union({});
    auto a = make_expression<LinearExpression>(domains, 0);

    auto expr0 = a * 2;
    auto expr1 = 2 * a;

    int result0 = eval(expr0->flatten(), std::array {102});
    int result1 = eval(expr1->flatten(), std::array {90});

    CHECK(result0 == 204);
    CHECK(result1 == 180);
}

TEST_CASE("[ScaleExpression] : operator * & registry")
{
    auto domains = make_continue(0, 2);
    auto a = make_expression<LinearExpression>(domains, 0);

    auto expr0 = a * 2;
    auto expr1 = reg.at(expr0->id);

    expr0->domains = std::move(make_continue(0, 3));

    int result0 = eval(expr0->flatten(), std::array {102});
    int result1 = eval(expr1->flatten(), std::array {90});

    CHECK(result0 == 204);
    CHECK(result1 == 180);
    CHECK(expr0->domains->flatten() == std::set {0, 1, 2, 3});
    CHECK(expr1->domains->flatten() == std::set {0, 1, 2, 3});
}

TEST_CASE("[SumExpression] : operator +")
{
    auto domains = make_union({});
    auto a = make_expression<LinearExpression>(domains, 0);
    auto b = make_expression<LinearExpression>(domains, 1);

    auto expr = a + b;
    auto sum0 = a + 10;
    auto sum1 = a + 20;

    int result = eval(expr->flatten(), std::array {10, 23});
    int res_sum0 = eval(sum0->flatten(), std::array {15});
    int res_sum1 = eval(sum1->flatten(), std::array {15});

    CHECK(res_sum0 == 25);
    CHECK(res_sum1 == 35);
    CHECK(result == 33);
}

TEST_CASE("[Domains propagation]")
{
    auto var = [](IntervalPtr domains, int idx, int coeff = 1)
    {
        return make_expression<LinearExpression>(domains, idx, coeff);
    };

    auto X = var(make_continue(-10, 0), 0);

    auto expr = 2*X + 5;

    expr->propagate_domains(make_continue(0, 0));

    CHECK(X->domains->min() == -3);
    CHECK(X->domains->max() == -2);
}

TEST_CASE("[Test]")
{
    auto var = [](IntervalPtr domains, int idx, int coeff = 1)
    {
        return make_expression<LinearExpression>(domains, idx, coeff);
    };

    auto S = var(make_continue(1, 9), 0);
    auto E = var(make_continue(0, 9), 1);
    auto N = var(make_continue(0, 9), 2);
    auto D = var(make_continue(0, 9), 3);
    auto M = var(make_continue(1, 9), 4);
    auto O = var(make_continue(0, 9), 5);
    auto R = var(make_continue(0, 9), 6);
    auto Y = var(make_continue(0, 9), 7);

    std::vector<Expression> variables = {S, E, N, D, M, O, R, Y};

    auto SEND  = 1000*S + 100*E + 10*N + D;
    auto MORE  = 1000*M + 100*O + 10*R + E;
    auto MONEY = 10000*M + 1000*O + 100*N + 10*E + Y;

    auto constraint = SEND + MORE - MONEY;
    auto flat = constraint->flatten();

    constraint->propagate_domains(make_continue(0, 0));

    std::vector<int> solutions;
    size_t n_solutions = 0;

    std::array<int, 8> data{};
    std::array<std::set<int>, 8> flattened {};
    bool used[10] = {};

    for (size_t i = 0; i < variables.size(); ++i)
    {
        flattened[i] = variables[i]->domains->flatten();
    }

    std::function<void(int)> bt = [&](int depth)
    {
        if (depth == 8)
        {
            if (eval(flat, data) == 0)
            {
                solutions.insert(solutions.end(), data.begin(), data.end());
                n_solutions++;
            }
            return;
        }

        for (const auto& d : flattened[depth])
        {
            if (used[d]) continue;
            data[depth] = d;
            used[d]     = true;
            bt(depth + 1);
            used[d]     = false;
        }
    };

    auto start_time = std::chrono::high_resolution_clock::now();
    bt(0);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution : " << elapsed.count() << " ms\n";

    // auto result = eval(flat, std::array{9, 5, 6, 7, 1, 0, 8, 2});
    // CHECK(result == 0);
    // CHECK(n_solutions == 1);
}