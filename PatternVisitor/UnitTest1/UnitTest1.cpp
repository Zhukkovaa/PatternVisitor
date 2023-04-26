#include "pch.h"
#include "CppUnitTest.h"
#include "..\PatternVisitor.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		//тест на проверку работы конструктора класса Number
		TEST_METHOD(TestConstuctorNumber)
		{
			Number* n32 = new Number(32.0);
			Assert::IsTrue(n32->value() == 32.0);
		}
		//проверка виртуального метода «вычислить» класса Number
		TEST_METHOD(TestEvaluateNumber)
		{
			Number* n32 = new Number(32.0);
			Assert::IsTrue(n32->evaluate() == 32.0);
		}
		//тест для метода print() класса Number
		TEST_METHOD(TestPrintNumber)
		{
			Number* n32 = new Number(32.0);
			Assert::IsTrue(n32->print() == to_string(32.0));
		}
		//тест на проверку работы конструктора класса BinOperation
		TEST_METHOD(TestConstuctorBinOperation)
		{
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			Assert::IsTrue(minus->left() == n32 && minus->operation() == BinaryOperation::MINUS && minus->right() == n16);

		}
		//тест на проверку метода evaluate() класса BinOperation для вычитания
		TEST_METHOD(TestBinOperMinus) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			Assert::IsTrue(minus->evaluate() == 16.0);
		}
		//тест на проверку метода evaluate() класса BinOperation для сложения
		TEST_METHOD(TestBinOperPlus) {
			Number* n32 = new Number(-16.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* plus = new BinaryOperation(n32, BinaryOperation::PLUS, n16);
			Assert::IsTrue(plus->evaluate() == 0.0);
		}
		//тест на проверку метода evaluate() класса BinOperation для деления
		TEST_METHOD(TestBinOperDiv) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* div = new BinaryOperation(n32, BinaryOperation::DIV, n16);
			Assert::IsTrue(div->evaluate() == 2.0);
		}
		//тест на проверку метода evaluate() класса BinOperation для деления на 0
		TEST_METHOD(TestBinOperDivByZero) {
			Number* n32 = new Number(32.0);
			Number* n0 = new Number(0.0);
			BinaryOperation* divzero = new BinaryOperation(n32, BinaryOperation::DIV, n0);
			Assert::ExpectException<std::invalid_argument>([divzero]() {divzero->evaluate(); });

		}
		//тест на проверку метода evaluate() класса BinOperation для умножения
		TEST_METHOD(TestBinOperMul) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* mul = new BinaryOperation(n32, BinaryOperation::MUL, n16);
			Assert::IsTrue(mul->evaluate() == 512.0);
		}
		//тест на проверку метода print класса BinOperation
		TEST_METHOD(TestBinOperPrint) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			Assert::IsTrue(minus->print() == to_string(32.0) + string(1, BinaryOperation::MINUS) + to_string(16.0));
		}
		//тест для конструктора класса FunctionCall
		TEST_METHOD(TestConstructorFuncCall) {
			Number* n16 = new Number(16.0);
			FunctionCall* callSqrt = new FunctionCall("sqrt", n16);
			Assert::IsTrue(callSqrt->arg() == n16 && callSqrt->name() == "sqrt");

		}
		//тест для проверки метода Evalute при вычислении квадратного корня положит-го числа для класса FunctionCall
		TEST_METHOD(TestEvaluteFuncCallSQRT) {
			Number* n16 = new Number(16.0);
			FunctionCall* callSqrt = new FunctionCall("sqrt", n16);
			Assert::IsTrue(callSqrt->evaluate() == 4.0);

		}
		//тест для проверки метода Evalute при вычислении квадратного корня для отрицат-го числа класса FunctionCall
		TEST_METHOD(TestEvaluteNegativeSQRTFuncCall) {
			Number* n16 = new Number(-16.0);
			FunctionCall* callSqrt = new FunctionCall("sqrt", n16);
			Assert::ExpectException<std::invalid_argument>([callSqrt]() {callSqrt->evaluate(); });

		}
		//тест для проверки метода Evalute при вычислении модуля для отрицательного числа класса FunctionCall
		TEST_METHOD(TestEvaluteFuncCallABS) {
			Number* n16 = new Number(-16.0);
			FunctionCall* callSqrt = new FunctionCall("abs", n16);
			Assert::IsTrue(callSqrt->evaluate() == 16.0);

		}
		//тест для конструктора класса Value
		TEST_METHOD(TestConstructorValue) {
			Variable* var = new Variable("var");
			Assert::IsTrue(var->name() == "var");
		}
		//тест на проверку метода print класса Value
		TEST_METHOD(TestPrintValue) {
			Variable* var = new Variable("var");
			Assert::IsTrue(var->print() == "var");
		}
		//тест виртуального метода «вычислить»:
		TEST_METHOD(TestEvaluateValue) {
			Variable* var = new Variable("var");
			Assert::IsTrue(var->evaluate() == 0.0);
		}

		TEST_METHOD(CopyNumber)
		{
			CopySyntaxTree tree_copy;
			Number num(42.0);
			Expression* exp = num.transform(&tree_copy);
			Number* result = dynamic_cast<Number*>(exp);
			Assert::IsNotNull(result);
			Assert::AreEqual(42.0, result->value());
			delete exp;
		}
		TEST_METHOD(CopyVariable)
		{
			CopySyntaxTree tree_copy;
			Variable var("x");
			Expression* exp = var.transform(&tree_copy);
			Variable* result = dynamic_cast<Variable*>(exp);
			Assert::IsNotNull(result);
			Assert::AreEqual(std::string("x"), result->name());
			delete exp;
		}
		TEST_METHOD(TestCST) {
			Number* n32 = new Number(32.0);
			Number* n16 = new Number(16.0);
			BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
			FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
			Variable* var = new Variable("var");
			BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
			FunctionCall* callAbs = new FunctionCall("abs", mult);
			CopySyntaxTree CST;
			Expression* newExpr = callAbs->transform(&CST);
			Assert::IsTrue(newExpr->print() == callAbs->print());
		}
	};
}