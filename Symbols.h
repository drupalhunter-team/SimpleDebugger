#include <atomic>
#include <string>
#include <map>

#define PACKAGE // Needed for Apple's MAcPorts version of bfd.h

using namespace std;

class Symbols
{
public:
    void Load(string path);
    long get(string name);

private:
    static atomic_flag isBfdInit;

    map<string, long> mTable;

    static void Init();
};

/*
class Symbol
{
public:
    string name;
    long addr;

    Symbol(char* _name, long _addr): name(_name), addr(_addr) {}
};
*/
