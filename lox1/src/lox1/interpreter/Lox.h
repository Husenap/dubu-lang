#pragma once

namespace lox1 {

class Lox {
public:
	void RunFile(std::filesystem::path filePath);
	void Run(internal::blob&& code);

	static void Error(int line, std::string_view message);
	static void Report(int line, std::string_view where, std::string_view message);

private:
	static bool HadError;
};

}  // namespace lox1