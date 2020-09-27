#pragma once

#include "Token.h"

namespace lox1::internal {

class Lexer {
public:
	Lexer(blob&& code);

	const auto& GetTokens() const { return mTokens; }

private:
	void ScanToken();

	char Advance();
	char Peek(int offset = 0);

	bool Match(char expected);
	void MatchString();
	void MatchNumber();
	void MatchIdentifier();

	void AddToken(TokenType type);
	void AddToken(TokenType type, std::optional<Literal> literal);

	bool IsDigit(char c) { return c >= '0' && c <= '9'; }
	bool IsAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
	bool IsAlphaNumeric(char c) { return IsDigit(c) || IsAlpha(c); }

	blob mCode;
	std::size_t mStart   = 0;
	std::size_t mCurrent = 0;
	int         mLine    = 1;

	std::vector<Token> mTokens;
	bool               IsAtEnd();
};

}  // namespace lox1::internal