# JSON-validator

## About
JSON validator validates JSON files

## Compiling
The header file exists in `json/` directory,
json_validator.cpp is the implementation file.

To compile, `cd` into the project folder and run the following commands
g++ -c json_validator.cpp -o json_validator.o
g++ -c main.cpp -o main.o
g++ -o a.exe json_validator.o main.o -I json

## Running
Sample files are in the `sample_json_files/` folder.
These are the example files that are being validated.

## Sample output
`a  
<br/>running on valid json  
<br/>Valid JSON  
<br/>Valid JSON  
<br/>Valid JSON  
<br/>Valid JSON`  
<br/>  
<br/>`running on Invalid json  
<br/>Invalid JSON  
<br/>Invalid JSON  
<br/>Invalid JSON  
<br/>Invalid JSON`

