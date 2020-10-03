#pragma once

namespace lox1::internal {

enum class TokenType {
	// Single-character tokens
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
	Comma,
	Dot,
	Minus,
	Plus,
	Semicolon,
	Slash,
	Star,
	Colon,
	Query,
	Modulo,

	// One or two character tokens
	Bang,
	BangEqual,
	Equal,
	EqualEqual,
	Greater,
	GreaterEqual,
	Less,
	LessEqual,

	// Literals
	Identifier,
	String,
	Number,

	// Keywords
	And,
	Class,
	Else,
	False,
	For,
	Fn,
	If,
	Nil,
	Or,
	Print,
	Return,
	Super,
	This,
	True,
	Var,
	While,
	Eof
};

using Nil = std::monostate;
using Literal = std::variant<Nil, std::string, double, bool>;

class Token {
public:
	Token(TokenType              type,
	      std::string_view       lexeme,
	      std::optional<Literal> literal,
	      int                    line)
	    : mType(type)
	    , mLexeme(lexeme)
	    , mLiteral(literal)
	    , mLine(line) {}

	TokenType          GetType() const { return mType; }
	const std::string& GetLexeme() const { return mLexeme; }
	const Literal&     GetLiteral() const { return *mLiteral; }
	int                GetLine() const { return mLine; }

	std::string ToString() const {
		return std::to_string(static_cast<int>(mType)) + " " + mLexeme;
	}

private:
	TokenType              mType;
	std::string            mLexeme;
	std::optional<Literal> mLiteral;
	int                    mLine;
};

inline std::ostream& operator<<(std::ostream& stream, const Token& token) {
	stream << token.ToString();
	return stream;
}

}  // namespace lox1::internal