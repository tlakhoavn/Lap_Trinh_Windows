#pragma once
class CDailyExpense
{
	int m_Criteria ;
	WCHAR m_Content[20480];
	long m_AmountOfMoney;

public:
	CDailyExpense();
	~CDailyExpense();

	int get_Criteria();
	void set_Criteria(int value);

	WCHAR* get_Content();
	void set_Content(WCHAR * value);

	long get_AmountOfMoney();
	void set_AmountOfMoney(long value);

	void set_Allvalue(int Criteria, WCHAR* Content, long AmountOfMoney);

};

