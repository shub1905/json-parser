#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
using namespace std;

class jsonTree
{
	public:
		int intVal;
		string stringVal;
		bool boolVal;
		map<string, jsonTree> hashMap;
		vector<jsonTree> vectorList;

		void parseJson(string stream)
		{
			string key;
			string type;
			while(true)
			{
				if(!getToken(stream, key, type))
				{
					break;
				}
				string valueString;
				getValueString(stream, valueString);
				jsonTree * second = new jsonTree();
				
				second->parseJson(valueString);
				hashMap[key] = *second;
			}
		}

	private:
		bool getValueString(string &stream, string &valueString)
		{
			bool colon = false;
			bool init = false;
			int start=0, end=0;
			stack<char> bracketMatcher;

			for (int i=0; i<stream.size(); i++)
			{
				if (stream[i] == ':')
				{
					colon = true;
					start = i+1;
				}
				if (stream[i] == '[' || stream[i] == '{')
				{
					if (!init)
					{
						start = i;
						init = true;
					}
					bracketMatcher.push(stream[i]);
				}
				if (stream[i] == ']' || stream[i] == '}')
				{
					if (stream[i] != bracketMatcher.top())
					{
						return false;
					}
					bracketMatcher.pop();
					if (bracketMatcher.size() == 0)
					{
						end = i;
						break;
					}
				}
				if (!init && stream[i] == ',')
				{
					end = i-1;
					break;
				}
			}

			valueString = stream.substr(start,end+1);
			stream = stream.substr(end);
			return true;
		}

		void getType(string &stream, string &type)
		{
			bool colon = false;
			for (int i=0; i<stream.size(); i++)
			{
				if (stream[i] == '{')
				{
					type = "object";
					break;
				}
				else if (stream[i] == '[')
				{
					type = "array";
					break;
				}
				else if (stream[i] == ':')
				{
					colon = true;
				}
			}			
		}
		bool getToken(string &stream, string &key, string &type)
		{
			getType(stream, type);

			if (type == "object")
			{
				int position = stream.find("\"");
				int count = 0;
				for (int i=0; i<stream.length(); ++i)
				{
					if (stream[i] == '\"')
					{
						break;
					}
					count++;
				}
				key = stream.substr(position, count);
				stream = stream.substr(position+count);
				return true;
			}
			return false;
		}
};


int main()
{
	jsonTree a;
	a.parseJson("{\"type\":1}");
	cout<<"Hello Json Parser"<<endl;
	return 0;
}