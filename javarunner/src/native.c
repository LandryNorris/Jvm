
#include "native.h"

#include <dlfcn.h>
#include <string.h>

Symbol symbols[100];
int numLoadedSymbols = 0;

Library libraries[5];
int numLoadedLibraries = 0;

Library* loadSharedLibrary(const char* name) {
    // check if already loaded
    for (int i = 0; i < numLoadedLibraries; i++) {
        if (strcmp(libraries[i].name, name) == 0) return &libraries[i];
    }
    libraries[numLoadedLibraries].name = name;
    libraries[numLoadedLibraries].handle = dlopen(name, RTLD_NOW);
    numLoadedLibraries++;
    return &libraries[numLoadedLibraries - 1];
}

Symbol* loadSymbol(const char* name) {
    // check if already loaded
    for (int i = 0; i < numLoadedSymbols; i++) {
        if (strcmp(symbols[i].name, name) == 0) return &symbols[i];
    }

    for (int i = 0; i < numLoadedLibraries; i++) {
        const void* sym = dlsym(libraries[i].handle, symbols[i].name);
        if (sym != nullptr) {
            symbols[numLoadedSymbols].name = name;
            symbols[numLoadedSymbols].sym = sym;
            numLoadedSymbols++;
            return &symbols[numLoadedSymbols-1];
        }
    }

    // TODO(Landry): Handle symbol not found
}
