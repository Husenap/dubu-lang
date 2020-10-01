#include <gtest/gtest.h>
#include <lox1/lox1.h>

TEST(lox1, run_file) {
	lox1::Lox lox;

	EXPECT_NO_THROW(lox.RunFile("test.lox"));
	EXPECT_THROW(lox.RunFile("bad_test.lox"), std::runtime_error);
}
