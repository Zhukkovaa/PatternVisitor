#include <iostream>//подключаем библиотеку ввода/вывода
#include <cassert>//подключаем библиотеку для проверки утверждений (assertions)    
#include <string>//подключаем библиотеку для работы со строками 
using namespace std;//используем пространство имен std

//Пусть имеется иерархия классов «Выражение» → («Число», «Бинарная операция», «Вызов
//функции», «Переменная»), описанные ниже.Бинарная операция подразумевает возможность выполнения сложения, умножения,
//вычитания и деления; вызов функции — подразумевает работу с вызовом sqrt(квадратный
//корень) и abs(модуль).У переменной есть имя(значение пока ей не присваиваем).

struct Transformer;//объявляем структуру Transformer
struct Number;//объявляем структуру Number
struct BinaryOperation;//объявляем структуру BinaryOperation
struct FunctionCall;//объявляем структуру FunctionCall
struct Variable;//объявляем структуру Variable

struct Expression //объявляем базовую абстрактную структуру Expression
{
	virtual ~Expression() { }//объявляем виртуальный деструктор для базовой структуры Expression
	virtual double evaluate() const = 0; //абстрактный метод «вычислить»  (объявляем чистый виртуальный метод evaluate() для базовой структуры Expression)
	virtual Expression* transform(Transformer* tr) const = 0;//абстрактный метод для преобразования выражения 
	virtual std::string print() const = 0;//абстрактный метод "печать"
};

struct Transformer//абстрактный класс, который определяет интерфейс для преобразователя, который может использоваться для изменения выражения
{
	virtual ~Transformer() {}
	virtual Expression* transformNumber(Number const*) = 0;
	virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
	virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
	virtual Expression* transformVariable(Variable const*) = 0;
};
//Он имеет четыре чисто виртуальные функции, которые преобразуют Number, BinaryOperation FunctionCall и Variable
struct Number : Expression // стуктура «Число»
{
	Number(double value) : value_(value) {} //конструктор
	double value() const { return value_; } // метод чтения значения числа
	double evaluate() const { return value_; } // реализация виртуального метода «вычислить»
	~Number() {}//деструктор, тоже виртуальный
	//Данный код является реализацией метода класса Expression, который выполняет преобразование
	//числа (Number) с помощью объекта-трансформера (Transformer). Метод transform() возвращает указатель 
	//на объект типа Expression*, который является результатом применения трансформера к числу.
	//Параметр tr является указателем на объект - трансформер типа Transformer*, который используется для преобразования числа.
	//Вызов метода transformNumber() на объекте tr передает указатель на текущий объект Number(this), 
	//который будет использован для выполнения преобразования.Метод transformNumber() является методом класса 
	//Transformer и реализуется в зависимости от конкретного типа трансформера.
	//После выполнения преобразования метод transform() возвращает указатель на объект типа Expression*, 
	//который представляет результат преобразования числа с помощью трансформера.Результат может быть объектом того 
	//же типа(Number), если трансформер не выполняет преобразование, либо объектом другого типа, если трансформер производит преобразование в другой тип выражения.
	Expression* transform(Transformer* tr) const
	{
		return tr->transformNumber(this);
	}
	//Метод print() возвращает строковое представление числового значения объекта класса Number.
	//Он использует функцию std::to_string() для преобразования числового значения value_ в строку и возвращает эту строку.
	std::string print() const {
		return std::to_string(this->value_);
	}

private:
	double value_; // само вещественное число
};
struct BinaryOperation : Expression // «Бинарная операция»
{
	//  Этот фрагмент кода объявляет перечисление, которое связывает символы математических операций с числовыми константами.
	//  PLUS = +: Константа PLUS имеет значение + и будет использоваться для обозначения операции сложения.
	//	MINUS = -: Константа MINUS имеет значение - и будет использоваться для обозначения операции вычитания.
	//	DIV = / : Константа DIV имеет значение / и будет использоваться для обозначения операции деления.
	//	MUL = *: Константа MUL имеет значение * и будет использоваться для обозначения операции умножения.
	enum { // перечислим константы, которыми зашифруем символы операций
		PLUS = '+',
		MINUS = '-',
		DIV = '/',
		MUL = '*'
	};
	//Этот код определяет конструктор для BinaryOperation класса, который принимает три аргумента: left, op и right.
	//left и right являются указателями на Expression объекты, представляющие левый и правый операнды двоичной операции 
	//соответственно.op - это целочисленный код для операции, которая должна быть выполнена.Коды определены с использованием
	//перечисления, приведенного ранее в коде, где PLUS присваивается + , MINUS to - , DIV to / и MUL to * .
	//Конструктор инициализирует переменные - члены класса left_, op_ и right_ соответствующими аргументами.
	// Он также включает утверждение, что left_ и right_ являются ненулевыми указателями, указывающими на то, что 
	// конструктор ожидает передачи допустимых выражений.Если left_ или right_ является нулевым указателем, утверждение 
	// завершится ошибкой, и программа завершится с сообщением об ошибке.
	// в конструкторе надо указать 2 операнда — левый и правый, а также сам символ операции
	BinaryOperation(Expression const* left, int op, Expression const* right) : left_(left), op_(op), right_(right)
	{
		assert(left_ && right_);
	}
	~BinaryOperation() //в деструкторе освободим занятую память
	{
		delete left_;
		delete right_;
	}
	Expression const* left() const { return left_; } // чтение левого операнда
	Expression const* right() const { return right_; } // чтение правого операнда
	int operation() const { return op_; } // чтение символа операции
	double evaluate() const // реализация виртуального метода «вычислить»
	{
		double left = left_->evaluate(); // вычисляем левую часть
		double right = right_->evaluate(); // вычисляем правую часть
		switch (op_) // в зависимости от вида операции, складываем, вычитаем, или делим левую и правую части
		{
		case PLUS: return left + right;
		case MINUS: return left - right;
		case DIV: {
			if (right == 0.0) {
				throw std::invalid_argument("Division by zero");
			}
			else {
				return left / right;
			}
		}
		case MUL: return left * right;
		}
	}
	std::string print() const {
		return this->left_->print() + std::string(1, this->op_) + this->right_->print();
	}
	Expression* transform(Transformer* tr) const
	{
		return tr->transformBinaryOperation(this);
	}
private:
	Expression const* left_; // указатель на левый операнд
	Expression const* right_; // указатель на правый операнд
	int op_; // символ операции
};
struct FunctionCall : Expression // структура «Вызов функции»
{
	//Этот код описывает конструктор класса FunctionCall, который создает объект для представления вызова функции в математическом выражении.
	//Аргументами конструктора являются имя функции в виде строки name и аргумент arg в виде указателя на объект класса Expression.
	//В конструкторе также есть два вызова функции assert, которые выполняют проверки утверждений.
	//Первый assert проверяет, что arg не является нулевым указателем, чтобы избежать возможных ошибок во время выполнения программы.
	//Второй assert проверяет, что имя функции является корректным.В данном случае поддерживаются только функции "sqrt" и "abs".Если имя функции не соответствует этим значениям, то программа прерывается с сообщением об ошибке.
	// в конструкторе надо учесть имя функции и ее аргумент
	FunctionCall(std::string const& name, Expression const* arg) : name_(name),
		arg_(arg)
	{
		assert(arg_);
		assert(name_ == "sqrt" || name_ == "abs");
	} // разрешены только вызов sqrt и abs
	std::string const& name() const
	{
		return name_;
	}
	Expression const* arg() const // чтение аргумента функции
	{
		return arg_;
	}
	~FunctionCall() { delete arg_; } // освобождаем память в деструкторе
	virtual double evaluate() const { // реализация виртуального метода
		//«вычислить»
		if (name_ == "sqrt")
			if (arg_->evaluate() < 0.0) {
				throw std::invalid_argument("Square root of negative number");
			}
			else {
				return sqrt(arg_->evaluate()); // либо вычисляем корень квадратный}
			}
		else return fabs(arg_->evaluate());
	} // либо модуль — остальные функции запрещены
	Expression* transform(Transformer* tr) const
	{
		return tr->transformFunctionCall(this);
	}
	std::string print() const {
		return this->name_ + "(" + this->arg_->print() + ")";
	}
private:
	std::string const name_; // имя функции
	Expression const* arg_; // указатель на ее аргумент
};
struct Variable : Expression // структура «Переменная»
{
	Variable(std::string const& name) : name_(name) { } //в конструкторе надо
	//указать ее имя
	std::string const& name() const { return name_; } // чтение имени переменной
	double evaluate() const // реализация виртуального метода «вычислить»
	{
		return 0.0;
	}
	Expression* transform(Transformer* tr) const
	{
		return tr->transformVariable(this);
	}
	std::string print() const {
		return this->name_;
	}
private:
	std::string const name_; // имя переменной
};

int main()
{
	//------------------------------------------------------------------------------
	Expression* e1 = new Number(1.234);
	Expression* e2 = new Number(-1.234);
	Expression* e3 = new BinaryOperation(e1, BinaryOperation::DIV, e2);
	cout << e3->evaluate() << endl;
	//------------------------------------------------------------------------------
	Expression* n32 = new Number(32.0);
	Expression* n16 = new Number(16.0);
	Expression* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
	Expression* callSqrt = new FunctionCall("sqrt", minus);
	Expression* n2 = new Number(2.0);
	Expression* mult = new BinaryOperation(n2, BinaryOperation::MUL, callSqrt);
	Expression* callAbs = new FunctionCall("abs", mult);
	cout << callAbs->evaluate() << endl;
	//
}

