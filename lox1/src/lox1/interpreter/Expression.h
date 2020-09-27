#pragma once

#include <memory>
#include <tuple>
#include <variant>

#include "Token.h"

namespace lox1::internal {

struct Expression;

using GroupingExpression = std::tuple<std::unique_ptr<Expression>>;
using UnaryExpression    = std::tuple<Token, std::unique_ptr<Expression>>;
using BinaryExpression   = std::tuple<std::unique_ptr<Expression>, Token, std::unique_ptr<Expression>>;
using LiteralExpression  = std::tuple<Literal>;

struct Expression {
	using Expr = std::
	    variant<std::unique_ptr<Expression>, GroupingExpression, LiteralExpression, UnaryExpression, BinaryExpression>;

	Expression(Expr&& expr)
	    : mExpr(std::move(expr)) {}

	template <typename R, typename T>
	R Visit(const T& visitor) const {
		return std::visit(visitor, mExpr);
	}

	Expr mExpr;
};

struct LiteralVisitor {
	template <typename T>
	std::string operator()(const T& value) {
		return std::to_string(value);
	}
	std::string operator()(const std::monostate&) { return "nil"; }
	std::string operator()(const std::string& value) { return value; }
};

struct AstPrinter {
	std::string operator()(const std::unique_ptr<Expression>& expr) const { return expr->Visit<std::string>(*this); }

	std::string operator()(const GroupingExpression& expr) const {
		auto& [expression] = expr;
		return Parenthesize("group", expression);
	}

	std::string operator()(const LiteralExpression& expr) const {
		auto& [literal] = expr;
		return std::visit(LiteralVisitor(), literal);
	}

	std::string operator()(const UnaryExpression& expr) const {
		auto& [op, expression] = expr;
		return Parenthesize(op.GetLexeme(), expression);
	}

	std::string operator()(const BinaryExpression& expr) const {
		auto& [left, op, right] = expr;
		return Parenthesize(op.GetLexeme(), left, right);
	}

	template <typename... Args>
	std::string Parenthesize(std::string name, Args&&... args) const {
		std::string output = "(" + name;

		((output += (" " + operator()(args))), ...);

		output += ")";

		return output;
	}
};

}  // namespace lox1::internal