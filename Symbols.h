#include <atomic>
#include <string>
#include <map>

#define PACKAGE // Needed for Apple's MacPorts version of bfd.h

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
