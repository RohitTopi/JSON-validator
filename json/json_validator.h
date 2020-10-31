#ifndef JSON_VALIDATOR_H_INCLUDED
#define JSON_VALIDATOR_H_INCLUDED

#include<string>
#include<stack>
#include<vector>
using namespace std;

class JSONValidator
{
public:

    // constructor
    JSONValidator(string path);

    // starts the validation, returns true for valid JSON
    // false otherwise
    bool validateJSON();

private:
    string path;
    string jsonString;
    string VALID, INVALID;
    vector<char> escape;

    bool isValidNumber(const string num) const;

    // returns true if c is valid 'escaped' character, false otherwise
    bool isValidEscape(const char c) const;

    // returns true if c is a hexadecimal digit
    bool isHexDigit(const char c) const;

    // returns true if s is valid string, false otherwise
    bool isValidString(const string s) const;

    // returns true if v is a valid JSON value, false otherwise
    bool isValidValue(const string v) const;

    // helper method to validate name-value pair, by calling the
    // respective methods
    bool isValidNameValue(const string n, const string v) const;

    // parses a JSON object, and returns a valid value on success
    // returns an invalid value otherwise
    string objectParser(stack<string> st);

    // parses a JSON array, and returns a valid value on success
    // returns an invalid value otherwise
    string arrayParser(stack<string> st);

    // helper method to validate an object or an array
    // by calling appropriate function
    string validate(stack<string> st);

    // returns corresponding opening brace for closing
    string opening(const string closing) const;

    // returns true for valid JSON and false otherwise
    bool parser(vector<string> & tokens);

    // breaks JSON string into tokens
    bool lexer(vector<string> & tokens);
};

#endif // JSON_VALIDATOR_H_INCLUDED
