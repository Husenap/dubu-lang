#pragma once

#include "Parser.h"
#include "Token.h"
#include "Interpreter.h"

namespace lox1 {

class Lox {
public:
	void RunFile(std::filesystem::path filePath);
	void Run(internal::blob&& code);

	static void Error(int line, std::string_view message);
	static void Error(const internal::Token& token, std::string_view message);
	static void Report(int              line,
	                   std::string_view where,
	                   std::string_view message);
	static void ReportRuntimeError(internal::RuntimeError error);

private:
	static bool HadError;
	static bool HadRuntimeError;
};

}  // namespace lox1