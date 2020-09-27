#include "Lox.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Lexer.h"

namespace lox1 {

void Lox::RunFile(std::filesystem::path filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if (file.bad()) {
		throw std::runtime_error("Failed to open file: " + filePath.string());
	}

	auto size = file.tellg();

	internal::blob code;
	code.resize(static_cast<std::size_t>(size));

	file.seekg(0);
	file.read(&code[0], size);

	Run(code);

	if (mHadError) {
		exit(64);
	}
}

void Lox::Run(const internal::blob& code) {
	internal::Lexer lexer(code);

	for (auto& token : lexer.GetTokens()) {
		std::cout << token << std::endl;
	}
}

void Lox::Error(int line, std::string_view message) {
	Report(line, "", message);
}

void Lox::Report(int line, std::string_view where, std::string_view message) {
	std::cerr << "[line " << line << "] Error " << where << ": " << message << std::endl;
	mHadError = true;
}

}  // namespace lox1
