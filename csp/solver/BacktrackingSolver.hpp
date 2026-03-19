#ifndef __CSP_BACKTRACKING_SOLVER_HPP__
#define __CSP_BACKTRACKING_SOLVER_HPP__

#include "Solver.hpp"
#include <functional>

namespace kaiser::csp::solver
{
	class BacktrackingSolver : public Solver
	{
	private:
		using Solver::problem_;
		using Solver::solutions_;
		using Solver::n_solutions_;
		using Solver::offset_;

	public:
		BacktrackingSolver() = default;
		BacktrackingSolver(Problem problem);

		void solve() override;
		std::vector<int> get_solutions();
	};
}

#include "BacktrackingSolver.tpp"

#endif