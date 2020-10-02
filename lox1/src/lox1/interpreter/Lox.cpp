#include "Lox.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Expression.h"
#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"

namespace lox1 {

bool Lox::HadError        = false;
bool Lox::HadRuntimeError = false;

void Lox::RunFile(std::filesystem::path filePath) {
	std::ifstream file(filePath, std::ios::binary);

	if (file.fail()) {
		throw std::runtime_error("Failed to open file: " + filePath.string());
	}

	file.seekg(0, file.end);
	auto size = file.tellg();
	file.seekg(0, file.beg);

	internal::blob code;

	code.resize(static_cast<std::size_t>(size));

	file.seekg(0);
	file.read(&code[0], size);

	Run(std::move(code));

	/*
	if (HadError) {
		exit(64);
	}
	if (HadRuntimeError) {
		exit(70);
	}
	*/
}

void Lox::Run(internal::blob&& code) {
	HadError        = false;
	HadRuntimeError = false;

	internal::Lexer lexer(std::move(code));

	internal::Parser parser(lexer.GetTokens());

	if (auto expression = parser.Parse(); expression) {
		std::cout << "=========================================" << std::endl;
		std::cout << "Expression: ";
		std::string result =
		    expression->Visit<std::string>(internal::AstPrinter());
		std::cout << result << std::endl;
		std::cout << "    Result: ";
		internal::Interpreter interpreter;
		interpreter.Interpret(expression);
	}
}

void Lox::Error(int line, std::string_view message) {
	Report(line, "", message);
}

void Lox::Error(const internal::Token& token, std::string_view message) {
	if (token.GetType() == internal::TokenType::Eof) {
		Report(token.GetLine(), " at end", message);
	} else {
		Report(token.GetLine(), " at '" + token.GetLexeme() + "'", message);
	}
}

void Lox::Report(int line, std::string_view where, std::string_view message) {
	std::cerr << "[line " << line << "] Error " << where << ": " << message
	          << std::endl;
	HadError = true;
}

void Lox::ReportRuntimeError(internal::RuntimeError error) {
	std::cerr << error.what() << std::endl;
	HadRuntimeError = true;
}

}  // namespace lox1
