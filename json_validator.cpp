#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<set>
#include<stack>
#include<string>
#include<vector>
#include "json/json_validator.h"
using namespace std;

// constructor
JSONValidator::JSONValidator(string path):path{path}, VALID{string("")},
    INVALID{string("\"")}, escape(
{'\"','\\','/','b','f','n','r','t','u'
})
{
    jsonString.clear();
    string c;
    ifstream in_file;
    in_file.open(path);
    if(in_file.is_open())
    {
        while(in_file >> c)
        {
            jsonString += c;
        }
        in_file.close();
    }
    else
    {
        cerr << "error: could not open the file" << endl;
        exit(0);
    }

}

// starts the validation, returns true for valid JSON
// false otherwise
bool JSONValidator::validateJSON()
{
    vector<string> tokens;
    if(lexer(tokens))
    {
        return parser(tokens);
    }
    return false;
}


bool JSONValidator::isValidNumber(const string num) const
{
    string s = num;
    int n = (int)(s.size());
    int i = 0;

    // optional -
    if(s[0] == '-')
    {
        s.erase(0,1);
        n--;
    }

    // next must be a digit
    if(!isdigit(s[i]))
    {
        return false;
    }
    else
    {
        if(s[i] == '0')
        {
            // 0 is a valid number
            if(n==1)
            {
                return true;
            }
            else
                i++;
        }
        else
        {
            while(i<n && isdigit(s[i]))
            {
                i++;
            }

            if(i == n)
            {
                return true;
            }
        }
    }

    // next must be one of .,E,e
    if(!(s[i]=='.' || s[i]=='E' || s[i]!='e'))
    {
        return false;
    }
    else
    {
        // numbers like 0. 0E 0e must be invalid";
        if(i+1 == n)
        {
            return false;
        }

        if(s[i] == '.')
        {
            i++;

            // numbers containing .E must be invalid
            if(!isdigit(s[i]))
            {
                return false;
            }
            else
            {
                while(i<n && isdigit(s[i]))
                {
                    i++;
                }
                if(i==n)
                {
                    return true;
                }
            }
        }
    }

    if(s[i] == 'E' || s[i] == 'e')
    {
        i++;
        // optional + or -
        if(i<n && (s[i]=='+' || s[i]=='-'))
        {
            i++;
        }

        // numbers containing E as terminal character must be invalid
        if(i==n)
        {
            return false;
        }
        else
        {
            while(i<n && isdigit(s[i]))
            {
                i++;
            }
            return i==n;
        }
    }
    return i==n;
}


// returns true if c is valid 'escaped' character, false otherwise
bool JSONValidator::isValidEscape(const char c) const
{
    return find(escape.begin(), escape.end(), c) != escape.end();
}

bool JSONValidator::isHexDigit(const char c) const
{
    return isdigit(c) || ((int)c >= 'A' && (int)c <= 'F');
}

// returns true if s is valid string, false otherwise
bool JSONValidator::isValidString(const string s) const
{
    int n = (int)(s.size());
    if(n>=2 && s.front() == '\"' && s.back() == '\"') // early check
    {
        if(n == 2)
        {
            return true; //empty string
        }
        else
        {
            for(int i = 1; i<n; ++i)
            {
                if(s[i] == '\\')
                {
                    i++;
                    if(!isValidEscape(s[i]))
                    {
                        return false;
                    }
                    else
                    {
                        if(s[i] == 'u')
                        {
                            if((i+4) <= (n-2))
                            {
                                for(int k = i+1; k <= i+4; ++k)
                                {
                                    if(!isHexDigit(s[k]))
                                    {
                                        return false;
                                    }
                                }
                            }
                            else
                            {
                                return false;
                            }
                            i += 4;
                        }
                    }
                }
                else
                {
                    if((s[i] == '\"') && (i!=(n-1)))
                    {
                        return false;
                    }
                }
            }
            return true;
        }
    }
    return false;
}

// returns true if v is a valid JSON value, false otherwise
bool JSONValidator::isValidValue(const string v) const
{
    if(v==VALID)
        return true;
    else if(v==INVALID)
        return false;
    const int n = v.size();
    if(n > 0)
    {
        if(v[0] == '\"')
        {
            bool res = isValidString(v);
            return res;
        }
        else
        {
            if((v[0] == '-' && n>1) || isdigit(v[0]))
                return isValidNumber(v);
            return v=="true" || v=="false" || v=="null";
        }
    }
    return false;
}

// helper method to validate name-value pair, by calling the
// respective methods
bool JSONValidator::isValidNameValue(const string n, const string v) const
{
    return isValidString(n) && isValidValue(v);
}

// parses a JSON object, and returns a valid value on success
// returns an invalid value otherwise
string JSONValidator::objectParser(stack<string> st)
{
    int n = (int)(st.size());
    if(n<2)
    {
        return INVALID;
    }

    // check for empty object
    if(n == 2)
    {
        if(st.top() == "{")
        {
            st.pop();
            if(st.top() == "}")
                return VALID;
        }

        return INVALID;
    }
    else
    {
        bool isValid = true;
        st.pop();
        set<string> uq_names;
        while(st.size() > 1)
        {
            string name = st.top();
            st.pop();
            if(st.top() == ":")
            {
                st.pop();
            }
            else  // invalid name-value pair
            {
                return INVALID;
            }

            string val = st.top();
            st.pop();

            // name-value pair must be valid and name must be unique in this object
            if(!isValidNameValue(name, val) || (uq_names.find(name)!=uq_names.end()))
            {
                isValid = false;
                break;
            }
            else
            {
                uq_names.insert(name);
            }

            // for multiple name-value, there has to be atleast
            // five more tokens: ',', 'name', ':', 'value', '}'
            if(st.size() >= 5)
            {
                // separator must be ','
                if(st.top() != ",")
                {
                    isValid = false;
                    break;
                }
                else
                {
                    st.pop();
                }
            }
        }
        return (isValid)?VALID : INVALID;
    }
}

// parses a JSON array, and returns a valid value on success
// returns an invalid value otherwise
string JSONValidator::arrayParser(stack<string> st)
{
    int n = (int)(st.size());

    // must have atleast two tokens
    if(n<2)
    {
        return INVALID;
    }

    // check for empty array
    if(n == 2)
    {
        if(st.top() == "[")
        {
            st.pop();
            if(st.top() == "]")
                return VALID;
        }
        return INVALID;
    }
    else
    {
        // for a non-empty array, there must always be odd number of tokens
        if(st.size() % 2 == 0)
        {
            return INVALID;
        }
        st.pop();

        // for multi-valued arrays
        while(st.size() >= 4)
        {
            bool d1 = isValidValue(st.top());
            st.pop();
            if(not (d1 && st.top() == ",") )
                return INVALID;
            else
                st.pop();
        }

        // single valued or already validated
        return (isValidValue(st.top())) ? VALID : INVALID;
    }
}

// helper method to validate an object or an array
// by calling appropriate function
string JSONValidator::validate(stack<string> st)
{
    if(st.top() == "{")
    {
        return objectParser(st);
    }
    else
    {
        if(st.top() == "[")
        {
            return arrayParser(st);
        }
    }
    return INVALID;
}

// returns corresponding opening brace for closing
string JSONValidator::opening(const string closing) const
{
    if(closing == "}")
    {
        return string("{");
    }
    else
    {
        return string("[");
    }
}

// returns true for valid JSON and false otherwise
bool JSONValidator::parser(vector<string> & tokens)
{
    int n = (int)(tokens.size());

    // single token is invalid
    if(n < 2)
    {
        return false;
    }

    stack<string> st;
    for(string t: tokens)
    {
        if(t == "}" || t == "]")
        {
            string opn = opening(t);

            // create a temporary stack to hold inner tokens
            stack<string> st2({t});

            if(st.empty())
            {
                return false;
            }
            while(!st.empty())
            {
                if(st.top() == opn)
                {
                    st2.push(st.top());
                    st.pop();
                    break;
                }
                st2.push(st.top());
                st.pop();
            }
            st.push(validate(st2));
        }
        else
        {
            st.push(t);
        }
    }
    return (st.size() == 1) && (st.top() == VALID);
}

// breaks JSON string into tokens
bool JSONValidator::lexer(vector<string> & tokens)
{
    string& s = jsonString;
    tokens.clear();
    int n = (int)(s.size());

    for(int i = 0; i<n; ++i)
    {
        if(s[i] == '\"') // string starts here
        {
            int j = i+1;
            while(s[j]!='\"' && j<n)
            {
                // check for escape character
                if(s[j] == '\\')
                {
                    // escape character cannot be the last character
                    if(j == n-1)
                    {
                        return false;
                    }
                    else
                    {
                        // special check for the " character is required
                        // since it is a closing character for string
                        if(s[j+1] == '\"')
                        {
                            j++;
                        }
                    }
                }
                j++; // pass through the escape sequence
            }
            if(j==n)
                return false;
            tokens.push_back(jsonString.substr(i, j-i+1));
            i = j;
        }
        else
        {
            if(isdigit(s[i]) || s[i] == '-' )
            {
                string tmp;
                while(i<n && (isdigit(s[i]) || (s[i] == '.') || (s[i] == 'E') || s[i] == 'e' || s[i] == '+' || s[i] == '-'))
                {
                    tmp.push_back(s[i]);
                    i++;
                }
                tokens.push_back(tmp);
                i--;
            }
            else
            {
                string tmp;
                tmp.push_back(s[i]);
                tokens.push_back(tmp);
            }
        }
    }
    return true;
}
