#include "erpc_port.h"
#include <new>
#include <stdlib.h>

void *erpc_malloc(size_t size)
{
    void *p = malloc(size);
    return p;
}

void erpc_free(void *ptr)
{
    free(ptr);
}

/* Provide function for pure virtual call to avoid huge demangling code being linked in ARM GCC */
/* XXX ???
#if ((defined(__GNUC__)) && (defined(__arm__)))
extern "C" void __cxa_pure_virtual()
{
    while (1)
        ;
}
#endif
*/
