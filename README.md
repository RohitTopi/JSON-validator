# JSON Validator

JSON Validator validates JSON files

## Compiling
The header file exists in `json/` directory,
`json_validator.cpp` is the implementation file.

To compile, `cd` into the project folder and run the following commands
<pre>
g++ -c json_validator.cpp -o json_validator.o
g++ -c main.cpp -o main.o
g++ -o a.exe json_validator.o main.o -I json
</pre>

## Running
Sample files are in the `sample_json_files/` folder.
These are the example files that are being validated.

## Sample output
<pre>
a  
running on valid json  
Valid JSON  
Valid JSON  
Valid JSON  
Valid JSON

running on Invalid json  
Invalid JSON  
Invalid JSON  
Invalid JSON  
Invalid JSON
</pre>

