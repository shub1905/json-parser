#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <sstream>
using namespace std;

/* add null type in Json
 */
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
        string tempStr = stream;
        
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
            stringVal = stringToString(stream);
        }
        else // if (type == "object")
        {
            parseJsonObject(stream);
            tempStr = stream;
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
            listObjects.push_back(*temp);
        }
        
    }
    
    string stringToString(string value)
    {
        size_t posBegin = value.find('"');
        size_t posEnd = value.find('"',posBegin+1);
        return value.substr(posBegin+1, posEnd-posBegin-1);
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
            }
            else if( (stream[i] == ',' || stream[i] == ']') && bracketMatcher.size() == 0)
            {
                end = i;
                objectsArray.push_back(stream.substr(start,end-start));
                start = end+1;
            }
            else if (stream[i] == ']' || stream[i] == '}')
            {
                if ( (bracketMatcher.top() == '[' && ']' != stream[i]) || (bracketMatcher.top() == '{' && '}' != stream[i]) )
                {
                    cout<<"Error"<<endl;
                    return;
                    //add more error handling code here
                }
                bracketMatcher.pop();
            }
        }
        
        stream = stream.substr(start);
    }
    
    void getType(string stream, string &type)
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
            else if (stream[i] == ',' || stream[i] == '}' || i == stream.size()-1)
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
    
    bool getToken(string &stream, string &key, string &value)
    {
        stack<char> bracketMatcher;
        
        size_t posBegin = stream.find('"');
        size_t posEnd = stream.find('"', posBegin+1);
        size_t posColon = stream.find(':');
        
        if (posBegin == string::npos || posEnd == string::npos || posColon == string::npos)
            return false;
        
        bool init = true;
        key = stream.substr(posBegin+1, posEnd-posBegin-1);
        stream = stream.substr(posColon+1);
        
        size_t start=0,end=-1;
        for (int i=0;i<stream.size();i++)
        {
            if (stream[i] == '[' || stream[i] == '{')
            {
                bracketMatcher.push(stream[i]);
                if (init)
                {
                    start = i; //presence of bracket implies
                    init = false;
                }
            }
            else if( (stream[i] == ',' || stream[i] == '}') && bracketMatcher.size() == 0)
            {
                end = i;
                value = stream.substr(start,end-start);
                stream = stream.substr(end+1);
                return true;
            }
            else if (stream[i] == ']' || stream[i] == '}')
            {
                if ( (bracketMatcher.top() == '[' && ']' != stream[i]) || (bracketMatcher.top() == '{' && '}' != stream[i]) )
                {
                    return false;
                    //write error code; unmatching brackets
                }
                bracketMatcher.pop();
            }
            
        }
        return true;
    }
    
    
    void parseJsonObject(string &stream)
    {
        string key;
        string type;
        string value;
        while(true)
        {
            if(!getToken(stream, key, value))
            {
                break;
            }
            jsonTree *temp = new jsonTree();
            temp -> loads(value);
            hashMap[key] = *temp;
        }
    }
};


int main()
{
    jsonTree a;
    a.loads("{\"type\":1, \"source\":\"shubham\",\"arr\":[{\"hi\":45,\"bye\":[2,3,4]},2,3]}");
    cout<<"Hello Json Parser"<<endl;
    cout<<a.hashMap["type"].intVal<<endl;
    cout<<a.hashMap["source"].stringVal<<endl;
    for (int i=1;i<a.hashMap["arr"].vectorList.size();i++)
    {
        cout<<a.hashMap["arr"].vectorList[i].intVal<<endl;
    }
    jsonTree b = a.hashMap["arr"].vectorList[0];
    cout<<b.hashMap["hi"].intVal<<endl;
    for (int i=1;i<b.hashMap["bye"].vectorList.size();i++)
    {
        cout<<b.hashMap["bye"].vectorList[i].intVal<<endl;
    }
    
    return 0;
}