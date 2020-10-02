#include <gtest/gtest.h>
#include <lox1/lox1.h>

TEST(lox1, run_files) {
	lox1::Lox lox;

	EXPECT_NO_THROW(lox.RunFile("test.lox"));
	EXPECT_NO_THROW(lox.RunFile("test1.lox"));
	EXPECT_NO_THROW(lox.RunFile("test2.lox"));
	EXPECT_NO_THROW(lox.RunFile("test3.lox"));
	EXPECT_NO_THROW(lox.RunFile("test4.lox"));
	EXPECT_NO_THROW(lox.RunFile("test5.lox"));
	EXPECT_THROW(lox.RunFile("bad_test.lox"), lox1::internal::ParseError);
	EXPECT_NO_THROW(lox.RunFile("bad_test1.lox"));
	EXPECT_NO_THROW(lox.RunFile("bad_test2.lox"));
}
