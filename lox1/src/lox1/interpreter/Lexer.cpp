#include "Lexer.h"

#include "Lox.h"

namespace lox1::internal {

Lexer::Lexer(const blob& code)
    : mCode(code) {
	while (!IsAtEnd()) {
		mStart = mCurrent;
		ScanToken();
	}

	mTokens.emplace_back(TokenType::Eof, "", std::nullopt, mLine);
}

void Lexer::ScanToken() {
	// clang-format off
	switch (mCode[mCurrent++]) {
	case '(': AddToken(TokenType::LeftParen); break;
	case ')': AddToken(TokenType::RightParen); break;
	case '{': AddToken(TokenType::LeftBrace); break;
	case '}': AddToken(TokenType::RightBrace); break;
	case ',': AddToken(TokenType::Comma); break;
	case '.': AddToken(TokenType::Dot); break;
	case '-': AddToken(TokenType::Minus); break;
	case '+': AddToken(TokenType::Plus); break;
	case ';': AddToken(TokenType::Semicolon); break;
	case '*': AddToken(TokenType::Star); break;
	case '!': AddToken(Match('=') ? TokenType::BangEqual : TokenType::Bang); break;
	case '=': AddToken(Match('=') ? TokenType::EqualEqual : TokenType::Equal); break;
	case '<': AddToken(Match('=') ? TokenType::LessEqual : TokenType::Less); break;
	case '>': AddToken(Match('=') ? TokenType::GreaterEqual : TokenType::Greater); break;
	case '"': MatchString(); break;
	case '/':
		if (Match('/')) {
			while (Peek() != '\n' && !IsAtEnd()) {
				Advance();
			}
		} else {
			AddToken(TokenType::Slash);
		}
		break;
	case ' ': case '\r': case '\t': break;
	case '\n': ++mLine; break;
	default:
		Lox::Error(mLine, "Unexpected Character!");
		break;
	}
	// clang-format on
}

char Lexer::Advance() {
	return mCode[mCurrent++];
}

char Lexer::Peek() {
	if (IsAtEnd()) {
		return '\0';
	}
	return mCode[mCurrent];
}

bool Lexer::Match(char expected) {
	if (IsAtEnd()) {
		return false;
	}
	if (mCode[mCurrent] != expected) {
		return false;
	}

	++mCurrent;
	return true;
}

void Lexer::MatchString() {
	while (Peek() != '"' && !IsAtEnd()) {
		if (Peek() == '\n') {
			++mLine;
		}
		Advance();
	}

	if (IsAtEnd()) {
		Lox::Error(mLine, "Undetermined String!");
	}

	Advance();

	AddToken(TokenType::String, std::string(mCode.begin() + mStart + 1, mCode.begin() + mCurrent - 1));
}

void Lexer::AddToken(TokenType type) {
	AddToken(type, std::nullopt);
}

void Lexer::AddToken(TokenType type, std::optional<Literal> literal) {
	std::string_view lexeme(&mCode[mStart], mCurrent - mStart);
	mTokens.emplace_back(type, lexeme, literal, mLine);
}

bool Lexer::IsAtEnd() {
	return mCurrent >= mCode.size();
}

}  // namespace lox1::internal