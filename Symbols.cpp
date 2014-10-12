#include "Symbols.h"
#include <stdlib.h>
#include <bfd.h>

#include <cxxabi.h>

atomic_flag Symbols::isBfdInit = ATOMIC_FLAG_INIT;

void Symbols::Load(string path)
{
    Init();

    bfd *abfd = bfd_openr(path.c_str(), NULL); // TODO: close
    if (abfd == NULL) {
        return;
    }
    bfd_check_format(abfd, bfd_object);

    long size = bfd_get_symtab_upper_bound(abfd);
    if (size <= 0) {
        return;
    }
    asymbol **table = (asymbol**)malloc((size_t)size); // TODO: free
    long count = bfd_canonicalize_symtab(abfd, table);
    if (count < 0) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        const char *name = bfd_asymbol_name(table[i]);
        long value = bfd_asymbol_value(table[i]);

        int status;
        char *demangled = abi::__cxa_demangle(name + 1, 0, 0, &status);
        if (status == 0) {
            printf("SYM: [%s] 0x%08lx\n", demangled, value);

        } else {
            printf("SYM: [%s] 0x%08lx\n", name, value);
        }

        mTable.insert(pair<string, long>(name, value));
    }
    int a = 0;
}

long Symbols::get(string name) {
    return mTable[name];
}


void Symbols::Init() {
    if (isBfdInit.test_and_set()) {
        bfd_init();
    }
}
