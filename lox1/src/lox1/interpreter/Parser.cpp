#include "Parser.h"

#include "Lox.h"

namespace lox1::internal {

Parser::Parser(const std::vector<Token>& tokens)
    : mTokens(tokens) {}

inline std::unique_ptr<Expression> Parser::GetExpression() {
	return Equality();
}

inline std::unique_ptr<Expression> Parser::Equality() {
	return ParseLeftAssociativeSeries([this] { return Comparison(); },
	                                  TokenType::BangEqual,
	                                  TokenType::EqualEqual);
}

inline std::unique_ptr<Expression> Parser::Comparison() {
	return ParseLeftAssociativeSeries([this] { return Addition(); },
	                                  TokenType::Greater,
	                                  TokenType::GreaterEqual,
	                                  TokenType::Less,
	                                  TokenType::LessEqual);
}

inline std::unique_ptr<Expression> Parser::Addition() {
	return ParseLeftAssociativeSeries(
	    [this] { return Multiplication(); }, TokenType::Minus, TokenType::Plus);
}

inline std::unique_ptr<Expression> Parser::Multiplication() {
	return ParseLeftAssociativeSeries(
	    [this] { return Unary(); }, TokenType::Slash, TokenType::Star);
}

inline std::unique_ptr<Expression> Parser::Unary() {
	return Match(TokenType::Bang, TokenType::Minus)
	           ? std::make_unique<Expression>(
	                 UnaryExpression{Previous(), Unary()})
	           : Primary();
}

inline std::unique_ptr<Expression> Parser::Primary() {
	if (Match(TokenType::False)) {
		return std::make_unique<Expression>(LiteralExpression{false});
	}
	if (Match(TokenType::True)) {
		return std::make_unique<Expression>(LiteralExpression{true});
	}
	if (Match(TokenType::Nil)) {
		return std::make_unique<Expression>(
		    LiteralExpression{std::monostate()});
	}
	if (Match(TokenType::Number, TokenType::String)) {
		return std::make_unique<Expression>(
		    LiteralExpression{Previous().GetLiteral()});
	}
	if (Match(TokenType::LeftParen)) {
		auto expression = GetExpression();
		Consume(TokenType::RightParen, "Expected ')' after expression!");
		return std::make_unique<Expression>(
		    GroupingExpression{std::move(expression)});
	}
}

inline bool Parser::Check(TokenType tokenType) {
	if (IsAtEnd()) {
		return false;
	}
	return Peek().GetType() == tokenType;
}

inline const Token& Parser::Advance() {
	if (!IsAtEnd()) {
		++mCurrent;
	}
	return Previous();
}

const lox1::internal::Token& Parser::Consume(TokenType          tokenType,
                                             const std::string& message) {
	if (Check(tokenType)) {
		return Advance();
	}

	Lox::Error(Peek(), message);
	throw std::runtime_error(message);
}

inline bool Parser::IsAtEnd() {
	return Peek().GetType() == TokenType::Eof;
}

inline const Token& Parser::Peek() {
	return mTokens[mCurrent];
}

inline const Token& Parser::Previous() {
	return mTokens[mCurrent - 1];
}

}  // namespace lox1::internal