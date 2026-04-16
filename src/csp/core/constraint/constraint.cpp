#include "constraint.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace kaiser::csp::core::constraint
{
    Constraint equal(std::string ctx, int left, int right)
    {
        return std::make_shared<EqualConstraint>(std::move(ctx), left, right);
    }

    Constraint equal(const Expression& left, const Expression& right)
    {
        try
        {
            std::string left_ctx = left->get_context();
            std::string right_ctx = right->get_context();

            if (left_ctx != right_ctx)
            {
                std::ostringstream oss;

                oss << "Expression:" << left->id << " and "
                    << "Expression:" << right->id
                    << " is not in the same context";

                throw std::invalid_argument(oss.str());
            }

            return std::make_shared<EqualConstraint>(
                std::move(left_ctx), left->id, right->id);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }

        return nullptr;
    }

    Constraint not_equal(const Expression& left, const Expression& right)
    {
        try
        {
            std::string left_ctx = left->get_context();
            std::string right_ctx = right->get_context();

            if (left_ctx != right_ctx)
            {
                std::ostringstream oss;

                oss << "Expression:" << left->id << " and "
                    << "Expression:" << right->id
                    << " is not in the same context";

                throw std::invalid_argument(oss.str());
            }

            return std::make_shared<NotEqualConstraint>(
                std::move(left_ctx), left->id, right->id);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }

        return nullptr;
    }
}