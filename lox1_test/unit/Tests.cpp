#include <gtest/gtest.h>
#include <lox1/lox1.h>

TEST(lox1, run_file) {
	lox1::Lox lox;
	lox.RunFile("test.lox");
}
