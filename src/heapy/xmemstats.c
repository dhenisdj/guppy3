/* Extra, low-level memory statistics functions.
   Some is system dependent,
   some require special Python compilation.
*/

static char hp_xmemstats_doc[] =
"xmemstats()\n"
"\n"
"Print extra memory statistics. What is printed depends on the system\n"
"configuration.  ";

#include <dlfcn.h>

int totalloc, totfree, reallocfree, reallocalloc, numalloc, numfree, numdiff;

static int has_malloc_hooks;

static void **dlptr___malloc_hook, **dlptr___realloc_hook, **dlptr___free_hook;
static size_t (*dlptr_malloc_usable_size)(void *ptr);
static void (*dlptr_malloc_stats)(void);
static int (*dlptr__PyObject_DebugMallocStats)(FILE *out);
static Py_ssize_t *dlptr__Py_RefTotal;

void *(*org_alloc)(size_t size);
void *(*org_realloc)(void *p, size_t size);
void (*org_free)(void *p);

void
breakit(void *p, char c)
{
    // fprintf(stderr, "breakit %p %c %d\n", p, c, dlptr_malloc_usable_size(p));
}


void *
mallochook(size_t size) {
    void *o = *dlptr___malloc_hook;
    void *p;
    Py_ssize_t f;
    *dlptr___malloc_hook = 0;
    p = org_alloc(size);
    f = dlptr_malloc_usable_size(p);
    totalloc += f;
    *dlptr___malloc_hook = o;
    numalloc += 1;
    numdiff += 1;
    if (f > 265000) {
        breakit(p, 'm');
    }
    return p;
}

void *
reallochook(void *p, size_t size) {
    void *q;
    Py_ssize_t f;
    void *o = *dlptr___realloc_hook;
    if (p)
        f = dlptr_malloc_usable_size(p);
    else
        f = 0;
    *dlptr___realloc_hook = 0;
    q = org_realloc(p, size);
    if (q != p) {
        totfree += f;
        reallocfree += f;
        f = dlptr_malloc_usable_size(q);
        totalloc += f;
        reallocalloc += f;
    } else {
        f = dlptr_malloc_usable_size(q) - f;
        if (f > 0) {
            totalloc += f;
            reallocalloc += f;
        } else {
            totfree -= f;
            reallocfree -= f;
        }
    }
    *dlptr___realloc_hook = o;
    if (f > 265000) {
        breakit(q, 'r');
    }

    return q;
}

void
freehook(void *p) {
    void *o = *dlptr___free_hook;
    *dlptr___free_hook = 0;
    totfree += dlptr_malloc_usable_size(p);
    org_free(p);
    *dlptr___free_hook = o;
    numfree -= 1;
    numdiff -= 1;
}

void
xmemstats_init(void) {
    dlptr___malloc_hook              = dlsym(RTLD_DEFAULT, "__malloc_hook");
    dlptr___realloc_hook             = dlsym(RTLD_DEFAULT, "__realloc_hook");
    dlptr___free_hook                = dlsym(RTLD_DEFAULT, "__free_hook");
    dlptr_malloc_usable_size         = dlsym(RTLD_DEFAULT, "malloc_usable_size");
    dlptr_malloc_stats               = dlsym(RTLD_DEFAULT, "malloc_stats");
    dlptr__PyObject_DebugMallocStats = dlsym(RTLD_DEFAULT, "_PyObject_DebugMallocStats");
    dlptr__Py_RefTotal               = dlsym(RTLD_DEFAULT, "_Py_RefTotal");

    has_malloc_hooks = dlptr___malloc_hook && dlptr___realloc_hook &&
        dlptr___free_hook && dlptr_malloc_usable_size;
    if (has_malloc_hooks) {
        org_alloc   = &malloc;
        org_realloc = &realloc;
        org_free    = &free;
        *dlptr___malloc_hook  = &mallochook;
        *dlptr___realloc_hook = &reallochook;
        *dlptr___free_hook    = &freehook;
    }
}

static PyObject *
hp_xmemstats(PyObject *self, PyObject *args)
{
    if (dlptr_malloc_stats) {
        fprintf(stderr, "======================================================================\n");
        fprintf(stderr, "Output from malloc_stats\n\n");
        dlptr_malloc_stats();
    }

    if (dlptr__PyObject_DebugMallocStats) {
        fprintf(stderr, "======================================================================\n");
        fprintf(stderr, "Output from _PyObject_DebugMallocStats()\n\n");
        dlptr__PyObject_DebugMallocStats(stderr);
    }

    if (has_malloc_hooks) {
        fprintf(stderr, "======================================================================\n");
        fprintf(stderr, "Statistics gathered from hooks into malloc, realloc and free\n\n");

        fprintf(stderr, "Allocated bytes                    =         %12d\n", totalloc);
        fprintf(stderr, "Allocated - freed bytes            =         %12d\n", totalloc-totfree);
        fprintf(stderr, "Calls to malloc                    =         %12d\n", numalloc);
        fprintf(stderr, "Calls to malloc - calls to free    =         %12d\n", numdiff);
    }

#ifndef Py_TRACE_REFS
    if (dlptr__Py_RefTotal) {
#endif
        fprintf(stderr, "======================================================================\n");
        fprintf(stderr, "Other statistics\n\n");
#ifndef Py_TRACE_REFS
    }
#endif

    if (dlptr__Py_RefTotal) {
        fprintf(stderr, "Total reference count              =         %12zd\n", *dlptr__Py_RefTotal);
    }

#ifdef Py_TRACE_REFS
    {
        PyObject *x; int i;
        for (i = 0, x = this_module->_ob_next; x != this_module; x = x->_ob_next, i++);
        fprintf(stderr, "Total heap objects                 =         %12d\n", i);
    }
#endif
    fprintf(stderr, "======================================================================\n");

    Py_INCREF(Py_None);
    return Py_None;
}
