#include "stdafx.h"
#include "DailyExpense.h"


CDailyExpense::CDailyExpense()
{
	m_Criteria = 0;
	wsprintf(m_Content, L"");
	m_AmountOfMoney = 0;
}


CDailyExpense::~CDailyExpense()
{
}

int CDailyExpense::get_Criteria()
{
	return m_Criteria;
}

void CDailyExpense::set_Criteria(int value)
{
	m_Criteria = value;
}

WCHAR * CDailyExpense::get_Content()
{
	return m_Content;
}

void CDailyExpense::set_Content(WCHAR * value)
{
	wsprintf(m_Content, value);
}

long CDailyExpense::get_AmountOfMoney()
{
	return m_AmountOfMoney;
}

void CDailyExpense::set_AmountOfMoney(long value)
{
	m_AmountOfMoney = value;
}

void CDailyExpense::set_Allvalue(int Criteria, WCHAR * Content, long AmountOfMoney)
{
	m_Criteria = Criteria;
	wsprintf(m_Content, Content);
	m_AmountOfMoney = AmountOfMoney;
}
