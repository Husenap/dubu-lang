#pragma once

namespace lox1 {

class Lox {
public:
	void RunFile(std::filesystem::path filePath);
	void Run(const internal::blob& code);

private:
	void Error(int line, std::string_view message);
	void Report(int line, std::string_view where, std::string_view message);

	bool mHadError = false;
};

}  // namespace lox1