
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

double Parsing::Calculator::CalculateExp(std::string lexeme, int priority, int& bc)
{
	double result = 0.0;

	if (lexeme == "(")
	{
		bc++;

		result = CalculateExp(m_scaner->GetNextLexeme(), 0, bc);

		if (bc == 0)
			return result;

	}

	if (lexeme == ")")
	{
		bc--;
		return 0.0;
	}

	//if(IsNumber(lexeme))
	double op1 = atof(lexeme.c_str());

	std::string nexetLex;
	if (!m_scaner->GetNextLexeme(nexetLex))
	{
		return op1;
	}

	if (nexetLex == ")")
	{
		bc--;
		return op1;
	}

	std::string operators{ "+-*/^" };
	auto pos = operators.find_first_of(nexetLex.c_str());

	if (pos != std::string::npos)
	{
		int pr = pos + 1;

		if (pr < priority)
		{
			m_scaner->MoveCurrPos((nexetLex.size()));
			return op1;
		}

		double op2 = CalculateExp(m_scaner->GetNextLexeme(), pr, bc);

		switch (operators[pos])
		{
		case '+':
			result = op1 + op2; break;
		case '-':
			result = op1 - op2; break;
		case '*':
			result = op1 * op2; break;
		case '/':
			result = op1 / op2; break;
		default:
			break;
		}



		return CalculateExp(std::to_string(result), priority, bc);
	}



	return 0.0;
}

//double CalculateExpr(std::string& lexeme, Scaner& sc, int priority)
//{
//	if (lexeme == "(")
//	{
//		std::string nextLexeme = GetNextLexeme(sc);
//
//		auto result = CalculateExpr(nextLexeme, sc, 0);
//
//		if (GetNextLexeme(sc) != ")")
//		{
//			throw std::runtime_error("synax error: expected )");
//		}
//
//		return result;
//	}
//
//	auto op1 = std::stod(lexeme.c_str()); // throws 
//
//}


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