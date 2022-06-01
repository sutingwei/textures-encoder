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
           etc2 ..\Lenna.png -format RGB8 -output ..\Lenna.pkm -verbose
           astc -cl ..\Lenna.png ..\Lenna.astc 6x6 -medium
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
        snprintf (buffer, 512, "%s/EtcTool.exe", cwd.getPtr());
    }
    else if (strcmp(compressType, "astc") == 0)
    {
        snprintf (buffer, 512, "%s/astcenc-native.exe", cwd.getPtr());
    }
    else
    {
        printf("unsupported compress type. \n");
        help();
        exit(1);
    }

    char** _argv = new char*[2];
    _argv[0] = buffer;
    for (int i = 2; i < argc; ++i) {
        _argv[i-1] = const_cast<char*>(argv[i]);
    }
    _argv[argc-1] = nullptr;

    void* handle = bx::exec(_argv);

#ifdef _WIN32
    WaitForSingleObject(handle, -1);
#endif
}