#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <iterator>


using namespace std;

ifstream in("listOfMachines.txt");
ifstream tokIn("tokins.txt");


class Automate {
public:
	Automate() {}
	string tokenClass;
	set<string> Alphabit; //������� �������
	set<string> StartSt; //��������� ���������
	set<string> AllowSt;//���������� ��������� ��������
	set<string> EndSt;//�������������� ���������
	map<string, map<string, set<string>>> ConversionTable; //������� ���������
	string alphabitAsString = "";
	int priority;
	



	void initAutomate()
	{
		int k = 0;
		string s;
		//string startEndHooks;
		string tmp;
		if (in.peek() == '{')
		{
			do
			{
				in >> tmp;
			} while (in.peek() != '\n');
			
		}
		if (in.peek() != '{')
		{
			
			do
			{
				in >> s;
				priority = atoi(s.c_str());
			} while (in.peek() != '\n'); //��������� ��������� ��������
			
			do
			{
				in >> s;
				alphabitAsString += s;
				Alphabit.insert(s);
			} while (in.peek() != '\n'); //	��������� �������
			
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

			do
			{
				in >> start >> jump;
				if (start != "}")
					in >> end;
				ConversionTable[start][jump].insert(end);
			} while (start != "}");//������� ���������
			
			tmp = "";
			
		}
		/*cout << endl << endl << endl;
		cout << "��������� ������:" << endl;
		cout << priority << endl;

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
		cout << endl;*/
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
			{
				/*cout << tmp << " - ������� �� �� ��������" << endl;*/
				break;
			}
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

class Tokens
{
public:
	vector<Automate> Automates;
	int priority;
	string name;

	void initToken()
	{
		do
		{
			tokIn >> name >> priority;
		} while (tokIn.peek() != '\n' && tokIn.peek() != EOF);
	}
};

int main() {
	setlocale(LC_ALL, "RUS");

	vector<Tokens> tokens;
	while (tokIn.peek() != EOF)
	{
		Tokens T;
		T.initToken();
		tokens.push_back(T);
	}
	vector<Automate> machines;
	int amount = 0;
	while (in.peek() != EOF)
	{
		Automate A;
		A.initAutomate();
		for (int i = 0; i < tokens.size(); i ++) //���������� � ������ ������ ������� ��������� ������ �������
			if (tokens[i].priority == A.priority)
			{
				tokens[i].Automates.push_back(A);
				A.tokenClass = tokens[i].name;
			}
		machines.push_back(A);
		amount++;
	} 
	//�������������� ������ �������, ��� ��� � �� ����, ��� ������� ������ �� ����� ;D
	Automate whitespace;
	whitespace.Alphabit.insert(" ");
	whitespace.alphabitAsString = " ";
	whitespace.AllowSt.insert("1");
	whitespace.EndSt.insert("1");
	whitespace.StartSt.insert("1");
	whitespace.ConversionTable["1"][" "].insert("1");
	for (int i = 0; i < tokens.size(); i ++)
		if (tokens[i].name == "whitespace")
		{
			whitespace.priority = tokens[i].priority;
			whitespace.tokenClass = tokens[i].name;
			tokens[i].Automates.push_back(whitespace);
			break;
		}
	string str;
	cout << endl << "������� ������� ������ :" << endl;
	getline(cin, str);
	/*cout << "������� ��������� ������� ������ :" << endl;
	int k;
	cin >> k;*/

	int lastPosSpace = 0, currentPosSpace = 0;
	int i = 0;
	while (i < str.size())
	{
		if (str.find(" ", lastPosSpace) != string::npos) //���� ������� ��� ����������
		{
			currentPosSpace = str.find(" ", lastPosSpace);//���� ��������� ��������
			string newStr = str.substr(lastPosSpace, currentPosSpace - lastPosSpace);//�������� �� ������ ����� ������ �� �������
			vector< pair<bool, int>> results(machines.size());
			int index = 0, maxSize = 0;
			bool flag = false;
			for (int j = 0; j < machines.size(); j++) //���� �� ������� ���������
			{
				results[j] = machines[j].maxString(machines[j], newStr, 0); //��������� ���� ������ �� ������ ������ ��� ������� ��������
				if (results[j].first && results[j].second == newStr.size()) //���� ���� ������ ������ ��� � ����� ���������� ������ = ������������� �������� ���������
				{
					flag = true; //���� ���� �������� 
					if (results[j].second > maxSize) //���� ������������ ������� ��������� � ���������� ���� �������
					{
						maxSize = results[j].second;
						index = j;
					}
					else if (results[j].second == maxSize) //���� ��� �������� ����� ���������� ������� �����, �������� � ���������� �����������
					{
						if (machines[j].priority > machines[index].priority)
						{
							index = j;
						}
					}
				}
			}
			
			if (flag) //���� ���������� ������ �������� ������� �������� ���������
				cout << "<" << machines[index].alphabitAsString << ", " << machines[index].tokenClass << ">" << endl;
			else //���� ���, ������� ����� ������ � ����������� = 0 � ��� �������� = �������� ������ ������� ������
			{
				cout << "<" << newStr << ", ";
				for (int i = 0; i < tokens.size(); i++)
					if (tokens[i].priority == 0)
					{
						cout << tokens[i].name << ">" << endl;
						break;
					}
			}
			//������ �������� � �������
			cout << "< , ";
			for (int i = 0; i < tokens.size(); i++)
				if (whitespace.tokenClass == tokens[i].name)
				{
					cout << tokens[i].name << ">" << endl;
					break;
				}
			lastPosSpace = currentPosSpace + 1; //���������� ������� �������
			i += newStr.size() + 1;
		}
		else if(str.find(" ", lastPosSpace) == string::npos) //����� ������� ����������� 
		{
			string newStr = str.substr(lastPosSpace, str.size() - lastPosSpace);
			cout << newStr << endl;
			vector< pair<bool, int>> results(machines.size());
			int index = 0, maxSize = 0;
			bool flag = false;
			for (int j = 0; j < machines.size(); j++) //���� �� ������� ���������
			{
				results[j] = machines[j].maxString(machines[j], newStr, 0); //��������� ���� ������ �� ������ ������ ��� ������� ��������
				if (results[j].first && results[j].second == newStr.size()) //���� ���� ������ ������ ��� � ����� ���������� ������ = ������������� �������� ���������
				{
					flag = true; //���� ���� �������� 
					if (results[j].second > maxSize) //���� ������������ ������� ��������� � ���������� ���� �������
					{
						maxSize = results[j].second;
						index = j;
					}
					else if (results[j].second == maxSize) //���� ��� �������� ����� ���������� ������� �����, �������� � ���������� �����������
					{
						if (machines[j].priority > machines[index].priority)
						{
							index = j;
						}
					}
				}
			}

			if (flag) //���� ���������� ������ �������� ������� �������� ���������
				cout << "<" << machines[index].alphabitAsString << ", " << machines[index].tokenClass << ">" << endl;
			else //���� ���, ������� ����� ������ � ����������� = 0 � ��� �������� = �������� ������ ������� ������
			{
				cout << "<" << newStr << ", ";
				for (int i = 0; i < tokens.size(); i++)
					if (tokens[i].priority == 0)
					{
						cout << tokens[i].name << ">" << endl;
						break;
					}
			}
			i += newStr.size();
		}
	}
}