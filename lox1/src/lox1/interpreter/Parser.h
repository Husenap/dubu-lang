#pragma once

#include "Expression.h"
#include "Token.h"

namespace lox1::internal {

class Parser {
public:
	Parser(const std::vector<Token>& tokens);

	std::unique_ptr<Expression> GetExpression();

private:
	std::unique_ptr<Expression> Equality();
	std::unique_ptr<Expression> Comparison();
	std::unique_ptr<Expression> Addition();
	std::unique_ptr<Expression> Multiplication();
	std::unique_ptr<Expression> Unary();
	std::unique_ptr<Expression> Primary();

	bool         Check(TokenType tokenType);
	const Token& Advance();
	const Token& Consume(TokenType tokenType, const std::string& message);
	bool         IsAtEnd();
	const Token& Peek();
	const Token& Previous();

	template <typename SubRule, typename... TokenTypes>
	std::unique_ptr<Expression> ParseLeftAssociativeSeries(
	    SubRule subRule, TokenTypes&&... tokenTypes) {
		auto expression = subRule();

		while (Match(std::forward<TokenTypes>(tokenTypes)...)) {
			expression = std::make_unique<Expression>(
			    BinaryExpression{std::move(expression), Previous(), subRule()});
		}

		return expression;
	}

	template <typename... TokenTypes>
	bool Match(TokenTypes&&... tokenTypes) {
		if ((Check(tokenTypes) || ...)) {
			Advance();
			return true;
		}

		return false;
	}

	const std::vector<Token> mTokens;
	int                      mCurrent;
};

}  // namespace lox1::internal