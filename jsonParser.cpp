#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <sstream>
using namespace std;

class jsonTree
{
	public:
    int intVal;
    string stringVal;
    bool boolVal;
    map<string, jsonTree> hashMap;
    vector<jsonTree> vectorList;

    void loads(string stream)
    {
        string type;
        getType(stream, type);
        if (type == "array")
        {
            parseArray(stream, vectorList);
        }
        else if (type == "int")
        {
            intVal = stringToInt(stream);
        }
        else if (type == "bool")
        {
            boolVal = stringToBool(stream);
        }
        else if (type == "string")
        {
            stringVal = stream;
        }
        else // if (type == "object")
        {
            parseJsonObject(stream);
        }

    }
	private:
    void parseArray(string &stream, vector<jsonTree> & listObjects)
    {
        vector<string> objectsArray;
        getTokensArray(stream,objectsArray);
        
        for (int i=0; i<objectsArray.size(); i++)
        {
            string typeChild;
            getType(objectsArray[i], typeChild);
            
            jsonTree * temp = new jsonTree();
            temp -> loads(objectsArray[i]);
            /*
            if (typeChild == "int")
            {
                temp -> intVal = stringToInt(objectsArray[i]);
            }
            else if (typeChild == "bool")
            {
                temp -> boolVal = stringToBool(objectsArray[i]);
            }
            else if (typeChild == "string")
            {
                temp -> stringVal = objectsArray[i];
            }
            else // if(typeChild == "object" || typeChild == "array")
            {

            }*/
            listObjects.push_back(*temp);
        }

    }
    
    bool stringToBool(string value)
    {
        for (int i=0;i<value.size();i++)
        {
            if (value[i] == 't')
                return true;
            else if (value[i] == 'f')
                return false;
            else if (value[i] == ' ')
                continue;
            else
                break;
        }
        //write error handling code
        return false;
    }
    
    int stringToInt(string value)
    {
        stringstream ss;
        ss<<value;
        int val;
        ss>>val;
        return val;
    }

    void getTokensArray(string &stream, vector<string> &objectsArray)
    {
        /* input = [A,B,C....]
         output = vector<string> (A, B , C..) */
        
        stack<char> bracketMatcher;
        int start=1, end = -1;
        for (int i=1; i<stream.size(); i++)
        {
            if (stream[i] == '[' || stream[i] == '{')
            {
                bracketMatcher.push(stream[i]);
                start = i;
            }
            else if( (stream[i] == ',' || stream[i] == ']') && bracketMatcher.size() == 0)
            {
                end = i;
                objectsArray.push_back(stream.substr(start,end));
            }
            else if (stream[i] == ']' || stream[i] == '}')
            {
                if (bracketMatcher.top() != stream[i])
                {
                    cout<<"Error"<<endl;
                    return;
                    //add more error handling code here
                }
            }
        }
    }
    
    void getType(string &stream, string &type)
    {
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
            else if (stream[i] == ',' || stream[i] == '}')
            {
                type = "int";
                break;
            }
            else if (stream[i] == '"')
            {
                type = "string";
                break;
            }
            else if (stream[i] =='t' || stream[i] == 'f')
            {
                type = "bool";
                break;
            }
        }
    }

    void parseJsonObject(string stream)
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