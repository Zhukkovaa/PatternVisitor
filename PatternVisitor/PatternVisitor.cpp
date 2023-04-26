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

//Этот код объявляет класс CopySyntaxTree, который является наследником абстрактного класса Transformer. 
//Класс CopySyntaxTree переопределяет четыре метода, унаследованных от Transformer для копирования выражений разных типов.
struct CopySyntaxTree : Transformer
{
	//transformNumber: метод, который принимает указатель на объект типа Number и создает и возвращает новый объект типа Number, 
	//со значением числа, равным значению переданного объекта number.
	Expression* transformNumber(Number const* number)
	{
		Expression* exp = new Number(number->value());
		return exp;
	}
	//transformBinaryOperation: метод, который принимает указатель на объект типа BinaryOperation и создает и возвращает новый объект
	//типа BinaryOperation, используя метод transform для рекурсивной копии подвыражений left и right, которые также являются выражениями.
	Expression* transformBinaryOperation(BinaryOperation const* binop)
	{
		Expression* exp = new BinaryOperation((binop->left())->transform(this),
			binop->operation(),
			(binop->right())->transform(this));
		return exp;
	}
	//transformFunctionCall: метод, который принимает указатель на объект типа FunctionCall и создает и возвращает новый объект типа FunctionCall. 
	//В этом методе происходит копирование имени функции name и аргумента функции arg, используя метод transform для копирования подвыражений.
	Expression* transformFunctionCall(FunctionCall const* fcall)
	{
		Expression* exp = new FunctionCall(fcall->name(),
			(fcall->arg())->transform(this));
		return exp;
	}
	//transformVariable: метод, который принимает указатель на объект типа Variable и создает и возвращает новый объект типа Variable, 
	//используя имя переменной name, переданное в объекте var.
	Expression* transformVariable(Variable const* var)
	{
		Expression* exp = new Variable(var->name());
		return exp;
	}
	//класс имеет пустой деструктор, который не делает ничего, поскольку объекты, созданные в методах класса, уничтожаются в другом месте программы
	~CopySyntaxTree() { };
};


int main()
{
	/*
	//------------------------------------------------------------------------------
	// Создаем указатели на выражения
	Expression * e1 = new Number(1.234);
	Expression * e2 = new Number(-1.234);
	// Создаем объект типа BinaryOperation, используя указатели
	Expression * e3 = new BinaryOperation(e1, BinaryOperation::DIV, e2);
	// Вызываем метод evaluate() для вычисления результата и выводим его на экран
	cout << e3->evaluate() << endl;
	//------------------------------------------------------------------------------
	// Создаем указатели на выражения (Создаем указатели на объекты класса Number, со значениями 32.0 и 16.0)
	Expression* n32 = new Number(32.0);
	Expression* n16 = new Number(16.0);
	// Создаем объект типа Двоичная операция и Функциональный вызов, используя указатели
	//Создаем объект класса BinaryOperation, которому передаем указатели на n32 и n16, и операцию вычитания (MINUS)
	Expression* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
	//Создаем объект класса FunctionCall, которому передаем строку "sqrt" (название функции), и указатель на объект, созданный на предыдущем шаге (minus)
	Expression* callSqrt = new FunctionCall("sqrt", minus);
	//Создаем указатель на объект класса Number со значением 2.0
	Expression* n2 = new Number(2.0);
	//Создаем объект класса BinaryOperation, которому передаем указатель на объект, созданный на шаге 3 (callSqrt), указатель на объект, созданный на шаге 4 (n2), и операцию умножения (MUL)
	Expression* mult = new BinaryOperation(n2, BinaryOperation::MUL, callSqrt);
	//Создаем объект класса FunctionCall, которому передаем строку "abs" (название функции), и указатель на объект, созданный на предыдущем шаге (mult)
	Expression* callAbs = new FunctionCall("abs", mult);
	//Вызываем метод evaluate() у объекта callAbs, который производит вычисление выражения. Результат выводим на экран
	cout << callAbs->evaluate() << endl;
	//------------------------------------------------------------------------------
	*/

	//Создаем объект класса Number с значением 32.0 и присваиваем указатель на него переменной n32
	Number* n32 = new Number(32.0);
	//Создаем объект класса Number с значением 16.0 и присваиваем указатель на него переменной n16
	Number* n16 = new Number(16.0);
	//Создаем объект класса BinaryOperation с указателями на n32 и n16 и операцией вычитания и присваиваем указатель на него переменной minus
	BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
	//Создаем объект класса FunctionCall с именем функции "sqrt" и указателем на объект BinaryOperation и присваиваем указатель на него переменной callSqrt
	FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
	//Создаем объект класса Variable с именем "var" и присваиваем указатель на него переменной var
	Variable* var = new Variable("var");
	//Создаем объект класса BinaryOperation с указателями на объект Variable и объект FunctionCall с указателем на объект
	//BinaryOperation и операцией умножения и присваиваем указатель на него переменной mult
	BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
	//Создаем объект класса FunctionCall с именем функции "abs" и указателем на объект BinaryOperation и присваиваем указатель на него переменной callAbs
	FunctionCall* callAbs = new FunctionCall("abs", mult);
	//Создаем объект класса CopySyntaxTree и присваиваем указатель на него переменной CST
	CopySyntaxTree CST;
	//Применяем метод transform объекта callAbs, передавая в качестве аргумента указатель на объект CST и присваиваем указатель на полученный объект Expression переменной newExpr
	Expression* newExpr = callAbs->transform(&CST);
	std::cout << newExpr->print() << std::endl;
	//Выводим на экран строковое представление объекта newExpr
}

