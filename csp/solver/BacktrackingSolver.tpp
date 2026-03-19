#include <stack>

namespace kaiser::csp::solver
{
	BacktrackingSolver::BacktrackingSolver(Problem problem)
	{
		problem_ = problem;
	}

	std::vector<int> BacktrackingSolver::get_solutions()
	{
		return solutions_;
	}

	void BacktrackingSolver::solve()
	{
		std::stack<std::vector<int>> stack;
		stack.push({});

		while(!stack.empty())
		{
			std::vector<int> data = std::move(stack.top());
			stack.pop();

			if(data.size() == problem_.variables->size())
			{
				if (problem_.constraints->compute(data))
		        {
		            solutions_.insert(solutions_.end(), data.begin(), data.end());
		            n_solutions_++;
		        }
		        continue;
			}

			size_t i = data.size();
			for (const int& d : problem_.variables->get_domain(i))
			{
				data.emplace_back(d);
				if (problem_.constraints->compute(data))
					stack.push(data);
				data.pop_back();
			}

			// for (const auto& d : problem_.domains)
		    // {
		    // 	   data.emplace_back(d);

		    //     if (problem_.constraints->compute(data))
		    //         stack.push(data);

		    //     data.pop_back();
		    // }
		}
	}
}