#include <kern/file.h>
#include <lcxxabi.h>
extern "C" int main(int argc, char **argv);

extern "C" void __entry_point(int argc, char **argv, char **env)
{
    asm volatile("and rsp, -16");
    sys::stdin = sys::file("/dev/stdin");
    sys::stdout = sys::file("/dev/stdout");
    sys::stderr = sys::file("/dev/stderr");
    main(argc, argv);
    __cxa_finalize(nullptr);
}
