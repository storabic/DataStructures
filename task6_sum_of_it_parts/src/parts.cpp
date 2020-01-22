#include <iostream>
using namespace std;
#include "parts.h"

NameContainer partContainer;

void Part::describe()
{
    cout << "Part " << name << " subparts are:" << endl;

    if (subparts.empty())
        cout << "It has no subparts." << endl;
    else
        for (map<Part*, int>::const_iterator it = subparts.begin(); it != subparts.end(); ++it)
            cout << "\t" << it->second << " " << it->first->name << endl;
}

int Part::count_howmany(Part const* p)
{
    if (p == this)
        return 1;
    int count = 0;
    for (map<Part*, int>::const_iterator it = subparts.begin(); it != subparts.end(); ++it)
        count += it->second * it->first->count_howmany(p);
    return count;
}

Part* NameContainer::lookup(string const& name)
{
    map<string, Part*>::const_iterator elem = name_map.find(name);
    if(elem == name_map.end())
    {
        Part* newPart = new Part(name);
        name_map[name] = newPart;
        return newPart;
    }
    else
        return elem->second;
}

NameContainer::~NameContainer(void)
{
    for (map<string, Part*>::const_iterator it = name_map.begin(); it != name_map.end(); ++it)
        delete it->second;
}

void add_part(string const& x, int q, string const& y)
{
    Part* parent = partContainer.lookup(x);
    Part* subPart = partContainer.lookup(y);
    parent->subparts.insert(make_pair(subPart, q));
}