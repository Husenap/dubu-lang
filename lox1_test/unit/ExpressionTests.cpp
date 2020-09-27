#include <gmock/gmock.h>
#include <lox1/interpreter/Expression.h>

using namespace lox1::internal;

TEST(expression_tests, simple_expression) {
	// clang-format off
	auto expression = std::make_unique<Expression>(BinaryExpression{
		std::make_unique<Expression>(UnaryExpression{
			Token(TokenType::Minus, "-", std::nullopt, 1),
			std::make_unique<Expression>(LiteralExpression{123.})
		}),
		Token(TokenType::Star, "*", std::nullopt, 1),
		std::make_unique<Expression>(GroupingExpression{
			std::make_unique<Expression>(LiteralExpression{45.67})
		})
	});
	// clang-format on

	std::string result   = expression->Visit<std::string>(AstPrinter{});
	std::string expected = "(* (- 123.000000) (group 45.670000))";

	EXPECT_EQ(result, expected);
}
