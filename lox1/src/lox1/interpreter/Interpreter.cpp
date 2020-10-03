#include "Interpreter.h"

#include "Lox.h"

namespace lox1::internal {

void Interpreter::Interpret(const std::unique_ptr<Expression>& expr) const {
	try {
		Literal value = expr->Visit<Literal>(InterpreterVisitor());
		std::cout << std::visit(LiteralVisitor(), value) << std::endl;
	} catch (const RuntimeError& e) {
		Lox::ReportRuntimeError(e);
	}
}

}  // namespace lox1::internal
