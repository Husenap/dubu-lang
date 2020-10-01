#include "Lexer.h"

#include "Lox.h"

namespace lox1::internal {

const static std::map<std::string, TokenType> KEYWORDS{
    {"and", TokenType::And},
    {"class", TokenType::Class},
    {"else", TokenType::Else},
    {"false", TokenType::False},
    {"for", TokenType::For},
    {"fn", TokenType::Fn},
    {"if", TokenType::If},
    {"nil", TokenType::Nil},
    {"or", TokenType::Or},
    {"print", TokenType::Print},
    {"return", TokenType::Return},
    {"super", TokenType::Super},
    {"this", TokenType::This},
    {"var", TokenType::Var},
    {"while", TokenType::While},
};

Lexer::Lexer(blob&& code)
    : mCode(std::move(code)) {
	while (!IsAtEnd()) {
		mStart = mCurrent;
		ScanToken();
	}

	mTokens.emplace_back(TokenType::Eof, "", std::nullopt, mLine);
}

void Lexer::ScanToken() {
	char c = Advance();
	switch (c) {
	// clang-format off
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
	case ' ': case '\r': case '\t': break;
	case '\n': ++mLine; break;
	// clang-format on
	case '/':
		if (Match('/')) {
			while (Peek() != '\n' && !IsAtEnd()) {
				Advance();
			}
		} else {
			AddToken(TokenType::Slash);
		}
		break;
	default:
		if (IsDigit(c)) {
			MatchNumber();
		} else if (IsAlpha(c)) {
			MatchIdentifier();
		} else {
			Lox::Error(mLine, "Unexpected Character!");
		}
		break;
	}
}  // namespace lox1::internal

char Lexer::Advance() {
	return mCode[mCurrent++];
}

char Lexer::Peek(int offset) {
	if (mCurrent + offset >= mCode.size()) {
		return '\0';
	}
	return mCode[mCurrent + offset];
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

	AddToken(
	    TokenType::String,
	    std::string(mCode.begin() + mStart + 1, mCode.begin() + mCurrent - 1));
}

void Lexer::MatchNumber() {
	while (IsDigit(Peek())) {
		Advance();
	}

	if (Peek() == '.' && IsDigit(Peek(1))) {
		Advance();

		while (IsDigit(Peek())) {
			Advance();
		}
	}

	std::string number(mCode.begin() + mStart, mCode.begin() + mCurrent);
	AddToken(TokenType::Number, std::atof(number.c_str()));
}

void Lexer::MatchIdentifier() {
	while (IsAlphaNumeric(Peek())) {
		Advance();
	}

	std::string text(mCode.begin() + mStart, mCode.begin() + mCurrent);

	TokenType type = TokenType::Identifier;

	auto it = KEYWORDS.find(text);
	if (it != KEYWORDS.end()) {
		type = it->second;
	}

	AddToken(type);
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