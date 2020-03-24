#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <string>
#include <unordered_set>
#include <vector>
#include <memory>

namespace Parsing
{
	class Scaner
	{
	public:
		Scaner(const std::string& expression, const std::unordered_set<char>& delimeters);
		bool GetNextLexeme(std::string& lexeme);
		std::string GetNextLexeme();
		void MoveCurrPos(std::string::size_type diff);
		std::string::size_type GetCurrentPosition() const;

	private:
		bool IsDelimeter(char symbol);

	private:
		const std::string m_expression;
		const std::unordered_set<char> m_delimeters;
		std::string::size_type m_currentPosition = 0;
	};

	class Calculator
	{
	public:
		Calculator();
		double CalculateExpression(const char* expression);

	private:

		double CalculateExp(std::string lexeme, int priority, int& bracketsCounter);

	private:
		const std::vector<char> operators = { '+','-','*','/', '^' };
		const std::vector<char> brackets = { '(', ')' };

		std::unique_ptr<Scaner> m_scaner;
	};

	long double PrintLexemes(const char* expression);
}

#endif
