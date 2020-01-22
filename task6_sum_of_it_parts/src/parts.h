// parts.h

#ifndef _PARTS_H_
#define _PARTS_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

//**************** Part ****************
class Part 
{
public:
    string name;
    Part(string const &n) : name(n) {};
    void describe(void);
    int count_howmany(Part const *p);

    std::map<Part*,int> subparts;
};

//**************** NameContainer ****************
class NameContainer  
{

private:
	map<string,Part*> name_map;

public:
	NameContainer(void) {};
	~NameContainer(void);
	Part* lookup(string const &name);
};

extern void add_part(string const &x, int q, string const &y);
extern NameContainer partContainer;

#endif
