#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <set>
#include <iomanip> // setw


#include "dictionary.h"

using namespace std;

void lower(string& s);
string stripPunct(const string& s);
void checkSpelling(ifstream& in, Dictionary& dict);

set<string> checkTransposition(const string word, Dictionary& dict);
set<string> checkRemove(const string word, Dictionary& dict);
set<string> checkReplacement(const string word, Dictionary& dict);
set<string> checkInsertion(const string word, Dictionary& dict);

// program arguments to run, example: main.exe ../../res/wordlist.txt ../../res/test.txt
int main(int argc, char* argv[]) {

    // Output usage message if improper command line args were given.
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " wordlist_filename input_file\n";
        return EXIT_FAILURE;
    }

    ifstream inf(argv[2]);
    if (! inf)
    {
        cerr << "Could not open " << argv[2] << "\n";
        return EXIT_FAILURE;
    }

    // Read dictionary, but let user know what we are working on.
    cout << "Loading dictionary, this may take awhile...\n";

    Dictionary d(argv[1]);

    checkSpelling(inf, d);

    inf.close();

    return EXIT_SUCCESS;
}

void checkSpelling(ifstream& in, Dictionary& dict) {

    int line_number = 0;

    while (in)
    {
        line_number++;
        set<string> totest;
        string line;
        getline(in, line);

        stringstream ss (stringstream::in | stringstream::out);
        ss << line;

        string word;
        while (ss >> word)
        {
            lower(word);
            word = stripPunct(word);
            if(!dict.search(word))
            {
                cout << "line " << line_number << ": \"" << word << "\", you probably meant: " << endl;

                set<string> foundWords = checkTransposition(word, dict);
                totest.insert(foundWords.begin(), foundWords.end());

                foundWords = checkRemove(word, dict);
                totest.insert(foundWords.begin(), foundWords.end());

                foundWords = checkReplacement(word, dict);
                totest.insert(foundWords.begin(), foundWords.end());

                foundWords = checkInsertion(word, dict);
                totest.insert(foundWords.begin(), foundWords.end());

                for (const auto& suggestion : totest)
                    cout << "\t\t\t\t" << suggestion << endl;
                totest.clear();
            }
        }
    }
}

set<string> checkTransposition(const string word, Dictionary& dict)
{
    set<string> foundWords;
    string tempString;
    for (int i = 0; i < word.length() - 1; ++i)
    {
        tempString = word;
        swap(tempString[i], tempString[i + 1]);
        if (dict.search(tempString))
            foundWords.insert(tempString);
    }
    return foundWords;
}

set<string> checkRemove(const string word, Dictionary& dict)
{
    set<string> foundWords;
    string tempString;
    for (int i = 0; i < word.length(); i++)
    {
        tempString = word.substr(0, i) + word.substr(i + 1, word.length());
        if (dict.search(tempString))
            foundWords.insert(tempString);
    }
    return foundWords;
}

set<string> checkReplacement(const string word, Dictionary& dict)
{
    set<string> foundWords;
    for (int i = 0; i < word.length(); i++)
    {
        string tempString = word;
        for (char letter = 'a'; letter <= 'z'; letter++)
        {
            tempString[i] = letter;
            if (dict.search(tempString))
                foundWords.insert(tempString);
        }
    }
    return foundWords;
}

set<string> checkInsertion(const string word, Dictionary& dict)
{
    set<string> foundWords;
    for (int i = 0; i <= word.length(); i++)
    {
        for (char letter = 'a'; letter <= 'z'; letter++)
        {
            string tempString = word;
            tempString.insert(i, string(1, letter));
            if (dict.search(tempString))
                foundWords.insert(tempString);
        }
    }
    return foundWords;
}

void lower(string& s) {

    // Ensures that a word is lowercase
    for (int i = 0; i < s.length(); i++)
    {
        s[i] = tolower(s[i]);
    }
}

string stripPunct(const string& s) {

    // Remove any single trailing
    // punctuation character from a word.
    if (ispunct(s[s.length() - 1]) )
    {
        return s.substr (0, s.length() - 1);
    }
    else
    {
        return s;
    }
}