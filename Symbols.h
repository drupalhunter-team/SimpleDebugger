#include <atomic>
#include <string>
#include <map>

#define PACKAGE // Needed for Apple's MacPorts version of bfd.h

#include <bfd.h>

using namespace std;

class Symbols
{
public:
    void Load(char *path);
    long Get(string name);
    void Close();

private:
    static atomic_flag isBfdInit;
    bfd *mBfd = nullptr;

    map<string, long> mTable;

    static void Init();
    static void DebugPrintSym(const char *name, const long addr);
};
