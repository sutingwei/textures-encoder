#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "bx/commandline.h"
#include "bx/os.h"
#include "bx/filepath.h"
#include "bx/string.h"

#ifdef _WIN32
#include <windows.h>
#endif

bx::DefaultAllocator crt;
bx::AllocatorI* g_allocator = &crt;
typedef bx::StringT<&g_allocator> bxString;

void help()
{
    static const char *help =
            R"(
Basic usage:
            etc2 (etc2 commands)
            astc (astc commands)
Example:
           etc2 ../Lenna.png -format RGB8 -output ../Lenna.pkm -verbose
           astc -cl ../Lenna.png ../Lenna.astc 6x6 -medium
)";
    printf("%s", help);
}

int main(int argc, const char * argv[])
{
    auto* c = new bx::CommandLine(argc, argv);
    if (argc < 2)
    {
        help();
        exit(1);
    }

#ifdef _WIN32
    setbuf(stdout, 0);
#endif

    const char* compressType = c->get(1);;
    auto* executorPath = new bx::FilePath(c->get(0));
    bxString cwd = bxString(executorPath->getPath());

    char buffer[512];
    if (strcmp(compressType, "etc2") == 0)
    {
#ifdef _WIN32
        snprintf (buffer, 512, "%s/EtcTool.exe", cwd.getPtr());
#elif __APPLE__
        snprintf (buffer, 512, "%sEtcTool", cwd.getPtr());
#endif
    }
    else if (strcmp(compressType, "astc") == 0)
    {
#ifdef _WIN32
        snprintf (buffer, 512, "%s/astcenc-native.exe", cwd.getPtr());
#elif __APPLE__
        snprintf (buffer, 512, "%sastcenc-native", cwd.getPtr());
#endif
    }
    else
    {
        printf("unsupported compress type. \n");
        help();
        exit(1);
    }

    char** _argv = new char*[argc];
    _argv[0] = buffer;
    for (int i = 2; i < argc; ++i) {
        _argv[i-1] = const_cast<char*>(argv[i]);
    }
    _argv[argc-1] = nullptr;

    void* handle = bx::exec(_argv);
    if (handle) {
#ifdef _WIN32
        WaitForSingleObject(handle, -1);
#elif __APPLE__
        int status;
        wait(&status);
#endif
    }
}