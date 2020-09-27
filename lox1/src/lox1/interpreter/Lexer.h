#pragma once

#include "Token.h"

namespace lox1::internal {

class Lexer {
public:
	Lexer(const blob& code);

	const auto& GetTokens() const { return mTokens; }

private:
	void ScanToken();

	char Advance();
	char Peek();

	bool Match(char expected);
	void MatchString();

	void AddToken(TokenType type);
	void AddToken(TokenType type, std::optional<Literal> literal);

	const blob& mCode;
	std::size_t mStart = 0;
	std::size_t mCurrent = 0;
	int mLine = 1;

	std::vector<Token> mTokens;
	bool               IsAtEnd();
};

}  // namespace lox1::internal