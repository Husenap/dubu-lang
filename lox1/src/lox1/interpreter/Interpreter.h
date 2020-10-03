#pragma once

#include <iostream>

#include "Expression.h"
#include "Token.h"

namespace lox1::internal {

class RuntimeError : public std::exception {
public:
	RuntimeError(const Token& token, const std::string& message) {
		mMessage = "RuntimeError [line " + std::to_string(token.GetLine()) + "]: " + message;
	}

	char const* what() const override { return mMessage.c_str(); }

private:
	std::string mMessage;
};

struct InterpreterVisitor {
	Literal Evaluate(const std::unique_ptr<Expression>& expr) const {
		return expr->Visit<Literal>(*this);
	}
	Literal operator()(const std::unique_ptr<Expression>& expr) const {
		return expr->Visit<Literal>(*this);
	}
	Literal operator()(const LiteralExpression& expr) const {
		auto& [literal] = expr;
		return literal;
	}
	Literal operator()(const GroupingExpression& expr) const {
		auto& [subExpression] = expr;
		return Evaluate(subExpression);
	}
	Literal operator()(const UnaryExpression& expr) const {
		auto& [op, rightExpression] = expr;
		Literal right               = Evaluate(rightExpression);

		switch (op.GetType()) {
		case TokenType::Minus:
			CheckNumberOperand(op, right);
			return -std::get<double>(right);
		case TokenType::Bang:
			return !IsTruthy(right);
		}

		return Nil();
	}
	Literal operator()(const BinaryExpression& expr) const {
		auto& [leftExpression, op, rightExpression] = expr;

		Literal left  = Evaluate(leftExpression);
		Literal right = Evaluate(rightExpression);

		switch (op.GetType()) {
		case TokenType::Comma:
			return right;
		case TokenType::Greater:
			CheckNumberOperands(op, left, right);
			return std::get<double>(left) > std::get<double>(right);
		case TokenType::GreaterEqual:
			CheckNumberOperands(op, left, right);
			return std::get<double>(left) >= std::get<double>(right);
		case TokenType::Less:
			CheckNumberOperands(op, left, right);
			return std::get<double>(left) < std::get<double>(right);
		case TokenType::LessEqual:
			CheckNumberOperands(op, left, right);
			return std::get<double>(left) <= std::get<double>(right);
		case TokenType::BangEqual:
			return !IsEqual(left, right);
		case TokenType::EqualEqual:
			return IsEqual(left, right);
		case TokenType::Minus:
			CheckNumberOperands(op, left, right);
			return std::get<double>(left) - std::get<double>(right);
		case TokenType::Slash:
			CheckNumberOperands(op, left, right);
			if (std::get<double>(right) == 0.0) {
				throw RuntimeError(op, "Division by zero!");
			}
			return std::get<double>(left) / std::get<double>(right);
		case TokenType::Modulo:
			CheckNumberOperands(op, left, right);
			if (std::get<double>(right) == 0.0) {
				throw RuntimeError(op, "Modulo by zero!");
			}
			return std::fmod(std::get<double>(left), std::get<double>(right));
		case TokenType::Star:
			CheckNumberOperands(op, left, right);
			return std::get<double>(left) * std::get<double>(right);
		case TokenType::Plus:
			if (std::holds_alternative<double>(left) &&
			    std::holds_alternative<double>(right)) {
				return std::get<double>(left) + std::get<double>(right);
			}
			if (std::holds_alternative<std::string>(left) &&
			    std::holds_alternative<std::string>(right)) {
				return std::get<std::string>(left) +
				       std::get<std::string>(right);
			}
			if (std::holds_alternative<std::string>(left)) {
				return std::get<std::string>(left) +
				       std::visit(LiteralVisitor(), right);
			}
			if (std::holds_alternative<std::string>(right)) {
				return std::visit(LiteralVisitor(), left) +
				       std::get<std::string>(right);
			}
			throw RuntimeError(op,
			                   "Operands must be two numbers or at least one string!");
		}

		return Nil();
	}
	Literal operator()(const TernaryExpression& expr) const {
		auto& [condition, left, right] = expr;

		if (IsTruthy(Evaluate(condition))) {
			return Evaluate(left);
		} else {
			return Evaluate(right);
		}
	}

	void CheckNumberOperand(const Token& op, const Literal& operand) const {
		if (std::holds_alternative<double>(operand)) {
			return;
		}
		throw RuntimeError(op, "Operand must be a number!");
	}
	void CheckNumberOperands(const Token&   op,
	                         const Literal& left,
	                         const Literal& right) const {
		if (std::holds_alternative<double>(left) &&
		    std::holds_alternative<double>(right)) {
			return;
		}
		throw RuntimeError(op, "Operands must be numbers!");
	}

	bool IsEqual(Literal a, Literal b) const {
		if (std::holds_alternative<Nil>(a) && std::holds_alternative<Nil>(b)) {
			return true;
		}
		if (std::holds_alternative<Nil>(a)) {
			return false;
		}
		return a == b;
	}

	bool IsTruthy(Literal literal) const {
		if (std::holds_alternative<Nil>(literal)) {
			return false;
		}
		if (std::holds_alternative<bool>(literal)) {
			return std::get<bool>(literal);
		}
		return true;
	}
};

class Interpreter {
public:
	void Interpret(const std::unique_ptr<Expression>& expr) const;
};

}  // namespace lox1::internal