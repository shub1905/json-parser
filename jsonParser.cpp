#include <iostream>
#include <vector>
#include <string>
using namespace std;

class jsonTree
{
	public:
		int firstInt;
		string firstKey;
		vector<jsonTree> second;

		void parseJson(string stream, jsonTree &root)
		{
			string key = getToken(stream);
			root.firstKey = key;
		}

	private:
		string getToken(string &stream)
		{
			int position = stream.find("\"");
			int count = 0;
			for (int i=0; i<stream.length(); ++i)
			{
				if (stream[i] == "\"")
				{
					break;
				}
				count++;
			}
			string ret = stream.substr(position, count);
			stream = stream.substr(position+count);
			return ret;
		}
}