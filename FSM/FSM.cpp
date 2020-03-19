#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <iterator>


using namespace std;

ifstream in("input.txt");

class Automate {
public:
	set<string> Alphabit; //������� �������
	set<string> StartSt; //��������� ���������
	set<string> AllowSt;//���������� ��������� ��������
	set<string> EndSt;//�������������� ���������
	map<string, map<string, set<string>>> ConversionTable; //������� ���������

	Automate() {}

	void initAutomate()
	{
		int k = 0;
		string s;
		string tmp;

		while (in.peek() != '\n') //	��������� �������
		{
			in >> s;
			Alphabit.insert(s);
		}

		do
		{
			in >> s;
			AllowSt.insert(s);
		} while (in.peek() != '\n'); //���������� ���������

		do
		{
			in >> s;
			StartSt.insert(s);
		} while (in.peek() != '\n'); // ��������� ���������

		do
		{
			in >> s;
			EndSt.insert(s);
		} while (in.peek() != '\n'); // �������� ���������

		string start, jump, end;

		while (in.peek() != EOF)//������� ���������
		{
			in >> start >> jump >> end;
			ConversionTable[start][jump].insert(end);
		} 

		cout << "������� ������� :" << endl;
		set<string> ::iterator it;
		for (it = Alphabit.begin(); it != Alphabit.end(); it++)
		{
			cout << *it << " ";
		}

		cout << endl << "���������� ��������� �������� : " << endl;
		for (it = AllowSt.begin(); it != AllowSt.end(); it++)
		{
			cout << *it << " ";
		}

		cout << endl << "��������� ��������� �������� : " << endl;
		for (it = StartSt.begin(); it != StartSt.end(); it++)
		{
			cout << *it << " ";
		}

		cout << endl << "�������� ��������� �������� : " << endl;
		for (it = EndSt.begin(); it != EndSt.end(); it++)
		{
			cout << *it << " ";
		}
	}


	//���������� maxString
	pair<bool, int> maxString(Automate A, string str, int k)
	{
		pair<bool, int> result;
		result.first = false;
		result.second = 0;
		set<string> currentStates = A.StartSt; //��������� ������� ��������� = ��������� ��������� ���������
		set<string> ::iterator itCurSt, itEndSt;
		for (itCurSt = currentStates.begin(); itCurSt != currentStates.end(); itCurSt++) //���������, �������� �� �������� ��������� - ��������
		{
			for (itEndSt = A.EndSt.begin(); itEndSt != A.EndSt.end(); itEndSt++)
				if (*itCurSt == *itEndSt)
					result.first = true;
		}

		int i = k;
		int len = str.length();

		while (i < len)
		{
			set<string> ::iterator itAlph;
			bool flag = false;
			string tmp;
			tmp = str[i];
			for (itAlph = A.Alphabit.begin(); itAlph != A.Alphabit.end(); itAlph++)
				if (tmp == *itAlph)
					flag = true;
			if (!flag) //���� �� ������������� �������� ��������, �� ����������
				break;
			else
			{
				set<string> newCurrentStates;
				set<string> ::iterator itSt;
				for (itSt = currentStates.begin(); itSt != currentStates.end(); itSt++) //���� �� ����� ���������� 
				{
					for (auto it = A.ConversionTable[*itSt][tmp].begin(); it != A.ConversionTable[*itSt][tmp].end(); it++) //��������� ���� �������� �������
					{
						newCurrentStates.insert(*it);
					}
				}
				currentStates = newCurrentStates;
				for (itCurSt = currentStates.begin(); itCurSt != currentStates.end(); itCurSt++) //���������, �������� �� �������� ��������� - ��������
				{
					for (itEndSt = A.EndSt.begin(); itEndSt != A.EndSt.end(); itEndSt++)
						if (*itCurSt == *itEndSt)
						{
							result.first = true;
							result.second = i - k + 1;
						}
				}
				i++;
			}
		}
		return result;
	}
};

int main() {
	setlocale(LC_ALL, "RUS");
	Automate A;
	A.initAutomate();

	string str;
	cout << endl << "������� ������� ������ :" << endl;
	cin >> str;

	cout << "������� ��������� ������� ������ :" << endl;
	int k;
	cin >> k;

	pair<bool, int> result;
	result = A.maxString(A, str, k);
	cout << result.first << " " << result.second << endl;
}