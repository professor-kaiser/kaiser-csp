#ifndef __CSP_PROBLEM_HPP__
#define __CSP_PROBLEM_HPP__

#include <functional>
#include <vector>
#include <memory>
#include "constraint/Constraint.hpp"
#include "variable/Variable.hpp"

using namespace kaiser::csp::core::constraint;
using namespace kaiser::csp::core::variable;

namespace kaiser::csp::core
{
	struct Problem
	{
		std::shared_ptr<Variable> variables;
		std::shared_ptr<Constraint> constraints;
	};
}

#endif