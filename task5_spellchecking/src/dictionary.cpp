#include <iostream>
#include <fstream>
#include <string>
#include "dictionary.h"


Dictionary::Dictionary(string filename)
{
    ifstream input(filename);
    if(!input)
        throw invalid_argument("Wrong filepath or file name");
    string tempStr;

    while(!input.eof())
    {
        getline(input, tempStr);
        insert(tempStr);
    }
}
