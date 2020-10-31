#include<iostream>
#include<string>
#include "json/json_validator.h"
using namespace std;


int main()
{
    cout << "running on valid json" << endl;
    for(int i = 1; i<=4; ++i)
    {
        string path = "sample_json_files/valid_json_" + to_string(i) + ".json";
        JSONValidator jsonv(path);
        if(jsonv.validateJSON())
        {
            cout << "Valid JSON" << endl;
        }
        else{
            cout << "Invalid JSON" << endl;
        }
    }


    cout << endl << "running on Invalid json" << endl;
    for(int i = 1; i<=4; ++i)
    {
        string path = "sample_json_files/invalid_json_" + to_string(i) + ".json";
        JSONValidator jsonv(path);
        if(jsonv.validateJSON())
        {
            cout << "Valid JSON" << endl;
        }
        else{
            cout << "Invalid JSON" << endl;
        }
    }

    return 0;
}


