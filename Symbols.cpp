#include "Symbols.h"
#include <stdlib.h>

#include <cxxabi.h>

atomic_flag Symbols::isBfdInit = ATOMIC_FLAG_INIT;

void Symbols::Load(char *path) {
    Init();

    mBfd = bfd_openr(path, NULL);
    if (mBfd == NULL) {
        return;
    }
    bfd_check_format(mBfd, bfd_object);

    long size = bfd_get_symtab_upper_bound(mBfd);
    if (size <= 0) {
        return;
    }
    asymbol **symtable = (asymbol**)malloc((size_t)size);
    long count = bfd_canonicalize_symtab(mBfd, symtable);
    if (count) {
        for (int i = 0; i < count; ++i) {
            const char *name = bfd_asymbol_name(symtable[i]);
            const long value = bfd_asymbol_value(symtable[i]);
            mTable.insert(pair<string, long>(name, value));

            DebugPrintSym(name, value);
        }
    }

    free(symtable);
}


void Symbols::Close() {
    if (mBfd != nullptr) {
        bfd_close(mBfd);
        mBfd = nullptr;
    }
}

long Symbols::Get(string name) {
    return mTable[name];
}


void Symbols::Init() {
    if (isBfdInit.test_and_set()) {
        bfd_init();
    }
}


void Symbols::DebugPrintSym(const char *name, const long addr) {
    int status;
#ifdef __APPLE__
    const char *source = ++name;
#else
    const char *source = name;
#endif
    char *demangled = abi::__cxa_demangle(source, 0, 0, &status);
    printf("SYM: [%s] 0x%08lx\n", (status == 0) ? (const char*)demangled : name, addr);
    free(demangled);
}
