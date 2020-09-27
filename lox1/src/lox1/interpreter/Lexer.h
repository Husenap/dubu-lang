#pragma once

#include "Token.h"

namespace lox1::internal {

class Lexer {
public:
	Lexer(const blob& ) {}

	const auto& GetTokens() const { return mTokens; }

private:
	std::vector<Token> mTokens;
};

}  // namespace lox1::internal