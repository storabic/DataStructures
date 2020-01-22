#ifndef  _DICTIONARY_H_
#define  _DICTIONARY_H_

#include  <iostream>
#include  <vector>
#include  <list>
#include  <algorithm>
#include  <string>
#include  <stdlib.h>

#include  "hashset.h"
#include  "hashset.cpp"

using namespace std;

class hash_function
{
public:

    unsigned int operator()(const std::string& s) const
    {
        char* buffer = new char[s.length()];
        for (int i = 0; i < s.length(); i++)
        {
            buffer[i] = s[i];
        }
        unsigned int hash_code =  adler_32(buffer, s.length());
        delete[] buffer;
        return hash_code;
    }

private:

    //adler_32 hash algorithm
    unsigned int adler_32(char* buf, unsigned long len) const
    {
        uint32_t s1 = 1;
        uint32_t s2 = 0;

        while (len--)
        {
            s1 = (s1 + *(buf++)) % 65521;
            s2 = (s2 + s1) % 65521;
        }
        return (s2 << 16) + s1;
    }
};

class equality
{
public:
    equality() {}

    bool operator()(const std::string& A, const std::string& B) const
    {
        return (A == B);
    }
};

class Dictionary : public HashSet<string, hash_function, equality>
{

public:
    Dictionary(string filename);

};

#endif // _DICTIONARY_H_