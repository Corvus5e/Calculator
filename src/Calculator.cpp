
#include "Calculator.h"

#include <string>
#include <cctype>
#include <unordered_set>
#include <algorithm>
#include <map>

#include <iostream>


Parsing::Scaner::Scaner(const std::string& expression, const std::unordered_set<char>& delimeters) :
	m_expression(expression),
	m_delimeters(delimeters)
{ }

bool Parsing::Scaner::IsDelimeter(char symbol)
{
		return m_delimeters.find(symbol) != m_delimeters.end();
}

bool Parsing::Scaner::GetNextLexeme(std::string& lexeme)
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

std::string Parsing::Scaner::GetNextLexeme()
{
	std::string lexeme;

	if (!GetNextLexeme(lexeme))
		throw std::runtime_error("Failed to extract next lexeme");

	return lexeme;
}

void Parsing::Scaner::MoveCurrPos(std::string::size_type diff)
{
	m_currentPosition -= diff;
}

std::string::size_type Parsing::Scaner::GetCurrentPosition() const
{
	return m_currentPosition;
}



Parsing::Calculator::Calculator()
{ }

double Parsing::Calculator::CalculateExpression(const char* expression)
{
	std::unordered_set<char> delimeters(operators.begin(), operators.end());
	delimeters.insert(brackets.begin(), brackets.end());

	m_scaner.reset(new Scaner(expression, delimeters));

	int bc = 0;
	double op = 0;
	return CalculateExp(m_scaner->GetNextLexeme(), 0, bc);
}

double Parsing::Calculator::CalculateExp(std::string lexeme, int priority, int& bracketsCount)
{
	double result = 0.0;

	double operandOne = 0.0;

	if (lexeme == "(")
	{
		bracketsCount++;

		result = CalculateExp(m_scaner->GetNextLexeme(), 0, bracketsCount);

		operandOne = result;

	}
	else //if(IsNumber(lexeme))
	{
		operandOne = atof(lexeme.c_str());
	}

	std::string nexetLex;

	if (!m_scaner->GetNextLexeme(nexetLex))
	{
		return operandOne;
	}

	if (nexetLex == ")")
	{
		bracketsCount--;
		return operandOne;
	}

	std::string operators{ "+-*/^" };
	auto pos = operators.find_first_of(nexetLex.c_str());

	if (pos != std::string::npos)
	{
		int currentPriority = pos + 1;

		if (currentPriority < priority)
		{
			m_scaner->MoveCurrPos((nexetLex.size()));
			return operandOne;
		}

		double operandTwo = CalculateExp(m_scaner->GetNextLexeme(), currentPriority, bracketsCount);

		switch (operators[pos])
		{
		case '+':
			result = operandOne + operandTwo; break;
		case '-':
			result = operandOne - operandTwo; break;
		case '*':
			result = operandOne * operandTwo; break;
		case '/':
			result = operandOne / operandTwo; break;
		default:
			break;
		}

		return CalculateExp(std::to_string(result), priority, bracketsCount);
	}

	return 0.0;
}


long double Parsing::PrintLexemes(const char* expression)
{
	Scaner s(expression, { '+','-','*','/', '^', '(', ')' });

	std::string lexeme;

	while (s.GetNextLexeme(lexeme))
	{
		std::cout << lexeme << ", ";
	}

	return 0.0;
}