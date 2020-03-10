
#include "Calculator.h"

#include <string>
#include <cctype>
#include <unordered_set>
#include <algorithm>

#include <iostream>

namespace
{
	class Scaner
	{
	public:
		Scaner(const std::string& expression);
		bool GetNextLexeme(std::string& lexeme);

	private:
		bool IsDelimeter(char symbol);

	private:
		const std::string m_expression;
		const std::unordered_set<char> m_delimeters = { '+','-','*','/', '^', '(', ')' };
		std::string::size_type m_currentPosition = 0;
	};

	Scaner::Scaner(const std::string& expression) : m_expression(expression)
	{ }

	bool Scaner::IsDelimeter(char symbol)
	{
		return m_delimeters.find(symbol) != m_delimeters.end();
	}

	bool Scaner::GetNextLexeme(std::string& lexeme)
	{
		if (m_currentPosition == m_expression.size())
			return false;

		for (std::string::size_type i = m_currentPosition; i < m_expression.length(); i++)
		{
			auto symbol = m_expression[i];

			if (!std::isspace(symbol))
			{
				if (IsDelimeter(symbol))
				{
					std::string::size_type length = std::max(i - m_currentPosition, (unsigned int)1);

					lexeme = m_expression.substr(m_currentPosition, length);

					m_currentPosition += length;

					return true;
				}
			}
		}

		lexeme = m_expression.substr(m_currentPosition);

		m_currentPosition = m_expression.size();

		return true;
	}
}

long double Calculator::CalculateExpression(const char* expression)
{
	Scaner s(expression);

	std::string lexeme;

	while (s.GetNextLexeme(lexeme))
	{
		std::cout << lexeme << ", ";
	}

	return 0.0;
}