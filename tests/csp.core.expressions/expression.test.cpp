#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"

#include "../../src/csp/core/expressions/LinearExpression.hpp"
#include "../../src/csp/core/expressions/ScaleExpression.hpp"
#include "../../src/csp/core/expressions/SumExpression.hpp"
#include "../../src/csp/core/expressions/operation.hpp"
#include "../../src/csp/core/structure/operation.hpp"
#include "../../src/csp/core/constraint/constraint.hpp"
#include "../../src/csp/core/arc/arc.hpp"
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
using namespace kaiser::csp::core::constraint;
using namespace kaiser::csp::core::arc;

// g++ -std=c++20 expression.test.cpp ../../src/csp/core/structure/*.cpp ../../src/csp/core/expressions/*.cpp -o expression

/*
g++ -std=c++20 \
expression.test.cpp \
../../src/csp/core/structure/*.cpp \
../../src/csp/core/expressions/*.cpp \
../../src/csp/core/constraint/*.cpp \
-o expression
*/

auto& expr_reg = ExpressionRegistry::instance();
auto& cons_reg = ConstraintRegistry::instance();
auto& neig_reg = NeighborsRegistry::instance();
auto& doms_reg = DomainsRegistry::instance();
auto& flat_doms_reg = FlatDomainsRegistry::instance();

auto var = [](std::string ctx, IntervalPtr domains, int idx, int coeff = 1)
{
    return make_expression<LinearExpression>(std::move(ctx), domains, idx, coeff);
};

// TEST_CASE("[LinearExpression]")
// {
//     auto domains = make_union({});
//     auto var = make_expression<LinearExpression>(domains, 0);

//     auto& expr = expr_reg.at(var->id);
    
//     int result0 = eval(var->flatten(), std::array {10});
//     int result1 = eval(expr->flatten(), std::array {10});

//     CHECK(result0 == 10);
//     CHECK(result1 == 10);
// }

// TEST_CASE("[ScaleExpression] : operator *")
// {
//     auto domains = make_union({});
//     auto a = make_expression<LinearExpression>(domains, 0);

//     auto expr0 = a * 2;
//     auto expr1 = 2 * a;

//     int result0 = eval(expr0->flatten(), std::array {102});
//     int result1 = eval(expr1->flatten(), std::array {90});

//     CHECK(result0 == 204);
//     CHECK(result1 == 180);
// }

// TEST_CASE("[ScaleExpression] : operator * & registry")
// {
//     auto domains = make_continuous(0, 2);
//     auto a = make_expression<LinearExpression>(domains, 0);

//     auto expr0 = a * 2;
//     auto expr1 = expr_reg.at(expr0->id);

//     expr0->domains = std::move(make_continuous(0, 3));

//     int result0 = eval(expr0->flatten(), std::array {102});
//     int result1 = eval(expr1->flatten(), std::array {90});

//     CHECK(result0 == 204);
//     CHECK(result1 == 180);
//     CHECK(expr0->domains->flatten() == std::set {0, 1, 2, 3});
//     CHECK(expr1->domains->flatten() == std::set {0, 1, 2, 3});
// }

// TEST_CASE("[SumExpression] : operator +")
// {
//     auto domains = make_union({});
//     auto a = make_expression<LinearExpression>(domains, 0);
//     auto b = make_expression<LinearExpression>(domains, 1);

//     auto expr = a + b;
//     auto sum0 = a + 10;
//     auto sum1 = a + 20;

//     int result = eval(expr->flatten(), std::array {10, 23});
//     int res_sum0 = eval(sum0->flatten(), std::array {15});
//     int res_sum1 = eval(sum1->flatten(), std::array {15});

//     CHECK(res_sum0 == 25);
//     CHECK(res_sum1 == 35);
//     CHECK(result == 33);
// }

// TEST_CASE("[Domains propagation]")
// {
//     auto var = [](IntervalPtr domains, int idx, int coeff = 1)
//     {
//         return make_expression<LinearExpression>(domains, idx, coeff);
//     };

//     auto X = var(context, make_continuous(-10, 0), 0);

//     auto expr = 2*X + 5;

//     expr->propagate_domains(make_continuous(0, 0));

//     CHECK(X->domains->min() == -3);
//     CHECK(X->domains->max() == -2);
// }

// TEST_CASE("[Test]")
// {
//     std::string context {"SENDMONEY"};
//     auto var = [&](IntervalPtr domains, int idx, int coeff = 1)
//     {
//         return make_expression<LinearExpression>(context, domains, idx, coeff);
//     };

//     auto S = var(context, make_continuous(1, 9), 0);
//     auto E = var(context, make_continuous(0, 9), 1);
//     auto N = var(context, make_continuous(0, 9), 2);
//     auto D = var(context, make_continuous(0, 9), 3);
//     auto M = var(context, make_continuous(1, 9), 4);
//     auto O = var(context, make_continuous(0, 9), 5);
//     auto R = var(context, make_continuous(0, 9), 6);
//     auto Y = var(context, make_continuous(0, 9), 7);

//     std::vector<Expression> variables = {S, E, N, D, M, O, R, Y};

//     auto SEND  = 1000*S + 100*E + 10*N + D;
//     auto MORE  = 1000*M + 100*O + 10*R + E;
//     auto MONEY = 10000*M + 1000*O + 100*N + 10*E + Y;

//     auto constraint = SEND + MORE - MONEY;
//     auto flat = constraint->flatten();

//     constraint->propagate_domains(make_continuous(0, 0));

//     std::vector<int> solutions;
//     size_t n_solutions = 0;

//     std::array<int, 8> data{};
//     std::array<std::set<int>, 8> flattened {};
//     bool used[10] = {};

//     for (size_t i = 0; i < variables.size(); ++i)
//     {
//         flattened[i] = variables[i]->domains->flatten();
//     }

//     std::function<void(int)> bt = [&](int depth)
//     {
//         if (depth == 8)
//         {
//             if (eval(flat, data) == 0)
//             {
//                 solutions.insert(solutions.end(), data.begin(), data.end());
//                 n_solutions++;
//             }
//             return;
//         }

//         for (const auto& d : flattened[depth])
//         {
//             if (used[d]) continue;
//             data[depth] = d;
//             used[d]     = true;
//             bt(depth + 1);
//             used[d]     = false;
//         }
//     };

//     auto start_time = std::chrono::high_resolution_clock::now();
//     bt(0);
//     auto end_time = std::chrono::high_resolution_clock::now();

//     std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
//     std::cout << "Execution : " << elapsed.count() << " ms\n";

//     // auto result = eval(flat, std::array{9, 5, 6, 7, 1, 0, 8, 2});
//     // CHECK(result == 0);
//     // CHECK(n_solutions == 1);
// }

// TEST_CASE("AB")
// {
//     const std::string context {"AB"};

//     //ContextGuard expr_guard(expr_reg, context);
//     ContextGuard neig_guard(neig_reg, context);
//     //ContextGuard cons_guard(cons_reg, context);
//     //ContextGuard doms_guard(doms_reg, context);

//     auto var = [&](IntervalPtr domains, int idx, int coeff = 1)
//     {
//         return make_expression<LinearExpression>(context, domains, idx, coeff);
//     };

//     auto A = var(context, make_continuous(2, 9), 0);
//     auto B = var(context, make_continuous(0, 4), 1);

//     auto constraint = equal(context, A->id, B->id);
    
//     ac3(context);

//     CHECK(constraint->compare({8, 8}));
//     CHECK(neig_reg.at(A->id) == std::vector {B->id});
//     CHECK(neig_reg.at(B->id) == std::vector {A->id});
//     CHECK(A->domains->flatten() == std::set {2, 3, 4});
// }

// TEST_CASE("[Test]")
// {
//     const std::string context {"CRYPT_1"};
    

//     ContextGuard flat_doms_guard(flat_doms_reg, context);

//     auto S = var(context, make_continuous(1, 9), 0);
//     auto E = var(context, make_continuous(0, 9), 1);
//     auto N = var(context, make_continuous(0, 9), 2);
//     auto D = var(context, make_continuous(0, 9), 3);
//     auto M = var(context, make_continuous(1, 9), 4);
//     auto O = var(context, make_continuous(0, 9), 5);
//     auto R = var(context, make_continuous(0, 9), 6);
//     auto Y = var(context, make_continuous(0, 9), 7);
//     auto zero = var(context, make_constant(0), 8);

//     std::vector<Expression> variables = {S, E, N, D, M, O, R, Y};

//     auto SEND  = 1000*S + 100*E + 10*N + D;
//     auto MORE  = 1000*M + 100*O + 10*R + E;
//     auto MONEY = 10000*M + 1000*O + 100*N + 10*E + Y;

//     // auto sum = SEND + MORE;
//     // auto res = MONEY;
//     auto result = SEND + MORE - MONEY;

//     auto constraint = equal(context, result->id, zero->id);
//     result->propagate_domains(make_constant(0));
//     //ac3(context);

//     // sum->propagate_domains(sum->domains);
//     // res->propagate_domains(res->domains);

//     std::vector<int> solutions;
//     size_t n_solutions = 0;

//     std::array<int, 9> data{};
//     std::array<std::set<int>, 8> flattened {};
//     bool used[10] = {};

//     for (size_t i = 0; i < variables.size(); ++i)
//     {
//         flattened[i] = flat_doms_reg.at(i);
//     }
//     constraint->prepare();

//     auto bt_impl = [&](auto& self, int depth) -> void
//     {
//         if (depth == 8)
//         {
//             if (constraint->compare(data))
//             {
//                 solutions.insert(solutions.end(), data.begin(), data.end());
//                 n_solutions++;
//             }
//             return;
//         }

//         for (const auto& d : flattened[depth])
//         {
//             if (used[d]) continue;
//             data[depth] = d;
//             used[d]     = true;
//             self(self, depth + 1);
//             used[d]     = false;
//         }
//     };

//     std::cout << "AFTER AC3" << std::endl;
//     for (const auto& v : variables)
//         std::cout << v->id << ": [" << v->domains->min() << ", " << v->domains->max() << "]" << std::endl;

//     auto start_time = std::chrono::high_resolution_clock::now();
//     bt_impl(bt_impl, 0);
//     auto end_time = std::chrono::high_resolution_clock::now();

//     std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
//     std::cout << "Execution : " << elapsed.count() << " ms\n";

//     CHECK(n_solutions == 1);
// }

TEST_CASE("[Test2]")
{
    const std::string context {"CRYPT_2"};
    ContextGuard flat_doms_guard(flat_doms_reg, context);
    ContextGuard expr_guard(expr_reg, context);

    auto S = var(context, make_continuous(1, 9), 0);
    auto E = var(context, make_continuous(0, 9), 1);
    auto N = var(context, make_continuous(0, 9), 2);
    auto D = var(context, make_continuous(0, 9), 3);
    auto M = var(context, make_continuous(1, 9), 4);
    auto O = var(context, make_continuous(0, 9), 5);
    auto R = var(context, make_continuous(0, 9), 6);
    auto Y = var(context, make_continuous(0, 9), 7);

    std::vector<Expression> variables = {S, E, N, D, M, O, R, Y};

    auto SEND  = 1000*S + 100*E + 10*N + D;
    auto MORE  = 1000*M + 100*O + 10*R + E;
    auto MONEY = 10000*M + 1000*O + 100*N + 10*E + Y;

    auto constraint = equal(SEND + MORE, MONEY);
    //ac3(context);
    ac4(context);

    std::vector<int> solutions;
    size_t n_solutions = 0;

    std::array<int, 9> data{};
    std::array<std::set<int>, 8> flattened {};
    bool used[10] = {};

    for (size_t i = 0; i < variables.size(); ++i)
    {
        flattened[i] = flat_doms_reg.at(variables[i]->id);
    }
    constraint->prepare();

    auto bt_impl = [&](auto& self, int depth) -> void
    {
        if (depth == 8)
        {
            if (constraint->compare(data))
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
            self(self, depth + 1);
            used[d]     = false;
        }
    };

    std::cout << "AFTER AC3" << std::endl;
    for (const auto& v : variables)
        std::cout << v->id << ": [" << v->domains->min() << ", " << v->domains->max() << "]" << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    bt_impl(bt_impl, 0);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution : " << elapsed.count() << " ms\n";

    auto expr = make_expression<LinearExpression>(context, make_constant(0), std::map<int, int> {}, 2026);

    CHECK(eval(expr->flatten(), std::vector<int>{}) == 2026);
    CHECK(n_solutions == 1);
}

TEST_CASE("[X, Y, Z]: EQ")
{
    std::string context {"XYZ|EQ"};
    auto X = var(context, make_continuous(1, 2), 0);
    auto Y = var(context, make_continuous(1, 2), 1);
    auto Z = var(context, make_constant(1), 2);

    equal(X, Y);
    equal(Y, Z);

    ac3(context);

    std::cout << "X: ";
    for (const auto& d : X->domains->flatten())
        std::cout << d << " ";
    std::cout << std::endl;

    std::cout << "Y: ";
    for (const auto& d : Y->domains->flatten())
        std::cout << d << " ";
    std::cout << std::endl;

    std::cout << "Z: ";
    for (const auto& d : Z->domains->flatten())
        std::cout << d << " ";
    std::cout << std::endl;
}

TEST_CASE("[X, Y]: NEQ")
{
    std::string context {"XY|NEQ"};
    auto X = var(context, make_continuous(1, 10), 0);
    auto Y = var(context, make_constant(5), 1);

    not_equal(X, Y);
    ac3(context);

    std::cout << "X: ";
    for (const auto& d : X->domains->flatten())
        std::cout << d << " ";
    std::cout << std::endl;

    std::cout << "Y: ";
    for (const auto& d : Y->domains->flatten())
        std::cout << d << " ";
    std::cout << std::endl;
}