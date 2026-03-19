#ifndef __CSP_ABSTRACT_SOLVER_HPP__
#define __CSP_ABSTRACT_SOLVER_HPP__

#include <vector>

namespace kaiser::csp::core
{
	class Problem;
}

using namespace kaiser::csp::core;

namespace kaiser::csp::solver
{
	class Solver
	{
	protected:
		Problem problem_;
		std::vector<int> solutions_;
		size_t n_solutions_;
		size_t offset_;
		/*
		std::vector<T> solutions_;
		std::function<bool (const std::vector<T>&)> constraints_;
		size_t offset_;
		*/


	public:
		virtual void solve() = 0;
		virtual ~Solver() = default;
	};
}

#endif