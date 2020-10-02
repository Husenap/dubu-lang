#include "Parser.h"

#include <iostream>

#include "Expression.h"
#include "Lox.h"

namespace lox1::internal {

Parser::Parser(const std::vector<Token>& tokens)
    : mTokens(tokens) {}

std::unique_ptr<Expression> Parser::Parse() {
	try {
		return GetExpression();
	} catch (ParseError e) {
		std::cerr << e.what() << std::endl;
		throw e;
	}
}

std::unique_ptr<Expression> Parser::GetExpression() {
	return Equality();
}

std::unique_ptr<Expression> Parser::Equality() {
	return ParseLeftAssociativeSeries([this] { return Comparison(); },
	                                  TokenType::BangEqual,
	                                  TokenType::EqualEqual);
}

std::unique_ptr<Expression> Parser::Comparison() {
	return ParseLeftAssociativeSeries([this] { return Addition(); },
	                                  TokenType::Greater,
	                                  TokenType::GreaterEqual,
	                                  TokenType::Less,
	                                  TokenType::LessEqual);
}

std::unique_ptr<Expression> Parser::Addition() {
	return ParseLeftAssociativeSeries(
	    [this] { return Multiplication(); }, TokenType::Minus, TokenType::Plus);
}

std::unique_ptr<Expression> Parser::Multiplication() {
	return ParseLeftAssociativeSeries(
	    [this] { return Unary(); }, TokenType::Slash, TokenType::Star);
}

std::unique_ptr<Expression> Parser::Unary() {
	return Match(TokenType::Bang, TokenType::Minus)
	           ? std::make_unique<Expression>(
	                 UnaryExpression{Previous(), Unary()})
	           : Primary();
}

std::unique_ptr<Expression> Parser::Primary() {
	if (Match(TokenType::False)) {
		return std::make_unique<Expression>(LiteralExpression{false});
	}
	if (Match(TokenType::True)) {
		return std::make_unique<Expression>(LiteralExpression{true});
	}
	if (Match(TokenType::Nil)) {
		return std::make_unique<Expression>(
		    LiteralExpression{Nil()});
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

	throw Error(Peek(), "Expected an expression!");
}

bool Parser::Check(TokenType tokenType) {
	if (IsAtEnd()) {
		return false;
	}
	return Peek().GetType() == tokenType;
}

const Token& Parser::Advance() {
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

	throw Error(Peek(), message);
}

bool Parser::IsAtEnd() {
	return Peek().GetType() == TokenType::Eof;
}

const Token& Parser::Peek() {
	return mTokens[mCurrent];
}

const Token& Parser::Previous() {
	return mTokens[mCurrent - 1];
}

void Parser::Synchronize() {
	Advance();

	while (!IsAtEnd()) {
		if (Previous().GetType() == TokenType::Semicolon) {
			return;
		}

		switch (Peek().GetType()) {
		case TokenType::Class:
		case TokenType::Fn:
		case TokenType::Var:
		case TokenType::For:
		case TokenType::If:
		case TokenType::While:
		case TokenType::Print:
		case TokenType::Return:
			return;
		}

		Advance();
	}
}

ParseError Parser::Error(const Token& token, std::string_view message) {
	Lox::Error(token, message);
	return ParseError();
}

}  // namespace lox1::internal