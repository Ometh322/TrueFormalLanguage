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
	set<string> Alphabit; //входной алфавит
	set<string> StartSt; //стартовые состояния
	set<string> AllowSt;//допустимые состояния алфавита
	set<string> EndSt;//заключительные состояния
	map<string, map<string, set<string>>> ConversionTable; //таблица переходов
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
			} while (in.peek() != '\n'); //считываем приоритет автомата
			
			do
			{
				in >> s;
				alphabitAsString += s;
				Alphabit.insert(s);
			} while (in.peek() != '\n'); //	считываем алфафит
			
			do
			{
				in >> s;
				AllowSt.insert(s);
			} while (in.peek() != '\n'); //допустимые состояния
			
			do
			{
				in >> s;
				StartSt.insert(s);
			} while (in.peek() != '\n'); // начальные состояния
			
			do
			{
				in >> s;
				EndSt.insert(s);
			} while (in.peek() != '\n'); // конечные состояния
			
			string start, jump, end;

			do
			{
				in >> start >> jump;
				if (start != "}")
					in >> end;
				ConversionTable[start][jump].insert(end);
			} while (start != "}");//таблица переходов
			
			tmp = "";
			
		}
		/*cout << endl << endl << endl;
		cout << "Приоритет токена:" << endl;
		cout << priority << endl;

		cout << "Входной алфавит :" << endl;
		set<string> ::iterator it;
		for (it = Alphabit.begin(); it != Alphabit.end(); it++)
		{
			cout << *it << " ";
		}

		cout << endl << "Допустимые состояния автомата : " << endl;
		for (it = AllowSt.begin(); it != AllowSt.end(); it++)
		{
			cout << *it << " ";
		}

		cout << endl << "Начальные состояния автомата : " << endl;
		for (it = StartSt.begin(); it != StartSt.end(); it++)
		{
			cout << *it << " ";
		}

		cout << endl << "Конечные состояния автомата : " << endl;
		for (it = EndSt.begin(); it != EndSt.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;*/
	}


	//реализация maxString
	pair<bool, int> maxString(Automate A, string str, int k)
	{
		pair<bool, int> result;
		result.first = false;
		result.second = 0;
		set<string> currentStates = A.StartSt; //множество текущих состояний = множеству начальных состояний
		set<string> ::iterator itCurSt, itEndSt;
		for (itCurSt = currentStates.begin(); itCurSt != currentStates.end(); itCurSt++) //проверяем, является ли начально состояние - конечным
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
			if (!flag) //если не соответствует входному алфавиту, то прекращаем
			{
				/*cout << tmp << " - Элемент не из алфавита" << endl;*/
				break;
			}
			else
			{
				set<string> newCurrentStates;
				set<string> ::iterator itSt;
				for (itSt = currentStates.begin(); itSt != currentStates.end(); itSt++) //идем по нашим состояниям 
				{
					for (auto it = A.ConversionTable[*itSt][tmp].begin(); it != A.ConversionTable[*itSt][tmp].end(); it++) //проверяем куда перейдет элемент
					{
						newCurrentStates.insert(*it);
					}
				}
				currentStates = newCurrentStates;
				for (itCurSt = currentStates.begin(); itCurSt != currentStates.end(); itCurSt++) //проверяем, является ли начально состояние - конечным
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
		for (int i = 0; i < tokens.size(); i ++) //определяем к какому классу токенов относится данный автомат
			if (tokens[i].priority == A.priority)
			{
				tokens[i].Automates.push_back(A);
				A.tokenClass = tokens[i].name;
			}
		machines.push_back(A);
		amount++;
	} 
	//инициализируем пробел вручную, так как я не знаю, как считать пробел из файла ;D
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
	cout << endl << "Введите входную строку :" << endl;
	getline(cin, str);
	/*cout << "Введите начальную позицию строки :" << endl;
	int k;
	cin >> k;*/

	int lastPosSpace = 0, currentPosSpace = 0;
	int i = 0;
	while (i < str.size())
	{
		if (str.find(" ", lastPosSpace) != string::npos) //если пробелы еще существуют
		{
			currentPosSpace = str.find(" ", lastPosSpace);//ищем вхождения пробелов
			string newStr = str.substr(lastPosSpace, currentPosSpace - lastPosSpace);//вырезаем из строки новую строку до пробела
			vector< pair<bool, int>> results(machines.size());
			int index = 0, maxSize = 0;
			bool flag = false;
			for (int j = 0; j < machines.size(); j++) //идем по вектору автоматов
			{
				results[j] = machines[j].maxString(machines[j], newStr, 0); //вычисляем макс стринг от данной строки для данного автомата
				if (results[j].first && results[j].second == newStr.size()) //если макс стринг выдает тру и длина вырезанной строки = возвращаемому значению максстриг
				{
					flag = true; //если есть элементы 
					if (results[j].second > maxSize) //ищем максимальный трушный результат и запоминаем этот автомат
					{
						maxSize = results[j].second;
						index = j;
					}
					else if (results[j].second == maxSize) //если два автомата имеют одинаковую трушную длину, выбираем с наибольшим приоритетом
					{
						if (machines[j].priority > machines[index].priority)
						{
							index = j;
						}
					}
				}
			}
			
			if (flag) //если вырезаемая строка является заранее заданным автоматом
				cout << "<" << machines[index].alphabitAsString << ", " << machines[index].tokenClass << ">" << endl;
			else //если нет, находим класс токена с приоритетом = 0 и его название = названию класса данного токена
			{
				cout << "<" << newStr << ", ";
				for (int i = 0; i < tokens.size(); i++)
					if (tokens[i].priority == 0)
					{
						cout << tokens[i].name << ">" << endl;
						break;
					}
			}
			//выводи сведение о пробеле
			cout << "< , ";
			for (int i = 0; i < tokens.size(); i++)
				if (whitespace.tokenClass == tokens[i].name)
				{
					cout << tokens[i].name << ">" << endl;
					break;
				}
			lastPosSpace = currentPosSpace + 1; //запоминаем позицию пробела
			i += newStr.size() + 1;
		}
		else if(str.find(" ", lastPosSpace) == string::npos) //когда пробелы закончились 
		{
			string newStr = str.substr(lastPosSpace, str.size() - lastPosSpace);
			cout << newStr << endl;
			vector< pair<bool, int>> results(machines.size());
			int index = 0, maxSize = 0;
			bool flag = false;
			for (int j = 0; j < machines.size(); j++) //идем по вектору автоматов
			{
				results[j] = machines[j].maxString(machines[j], newStr, 0); //вычисляем макс стринг от данной строки для данного автомата
				if (results[j].first && results[j].second == newStr.size()) //если макс стринг выдает тру и длина вырезанной строки = возвращаемому значению максстриг
				{
					flag = true; //если есть элементы 
					if (results[j].second > maxSize) //ищем максимальный трушный результат и запоминаем этот автомат
					{
						maxSize = results[j].second;
						index = j;
					}
					else if (results[j].second == maxSize) //если два автомата имеют одинаковую трушную длину, выбираем с наибольшим приоритетом
					{
						if (machines[j].priority > machines[index].priority)
						{
							index = j;
						}
					}
				}
			}

			if (flag) //если вырезаемая строка является заранее заданным автоматом
				cout << "<" << machines[index].alphabitAsString << ", " << machines[index].tokenClass << ">" << endl;
			else //если нет, находим класс токена с приоритетом = 0 и его название = названию класса данного токена
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