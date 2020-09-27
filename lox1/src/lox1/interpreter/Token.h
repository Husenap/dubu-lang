#pragma once

namespace lox1::internal {

class Token {
public:
};

inline std::ostream& operator<<(std::ostream& stream, const Token&) {
	stream << "token";
	return stream;
}

}  // namespace lox1::internal