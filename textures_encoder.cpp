#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "bx/commandline.h"

#include "EtcSourceImage.h"
#include "EtcImage.h"
#include "EtcFile.h"

void help()
{
    static const char *help =
            R"(
Basic usage:
            --type etc2 --in file.in --out file.out.pkm
            --type astc --in file.in --out file.out.astc --block [4x4|6x6|8x8]
Example:
            --type etc2 --in Lenna.png --out Lenna.pkm
            --type astc --in Lenna.png --out Lenna.astc --block 4x4
)";
    printf("%s", help);
}

int main(int argc, const char * argv[])
{
    auto* c = new bx::CommandLine(argc, argv);
    if (c->getNum() < 2) {
        help();
        exit(1);
    }
    const char* compressType = c->findOption("type");
    const char* inputFile = c->findOption("in");
    const char* outputFile = c->findOption("out");

    if (inputFile == NULL || outputFile == NULL)
    {
        printf("need input/output file. \n");
        help();
        exit(1);
    }

    int width, height, channels;
    stbi_uc* data = stbi_load(inputFile, &width, &height, &channels, 0);

    if (data == NULL)
    {
        printf("image data load failed. require absolute path.\n");
        exit(1);
    }
    stbi_image_free(data);

    if (strcmp(compressType, "etc2") == 0)
    {
        Etc::Image::Format format = channels == 4 ? Etc::Image::Format::RGBA8 : Etc::Image::Format::RGB8;

        printf("Encoding:\n");
        printf("  effort = %.f%%\n", ETCCOMP_DEFAULT_EFFORT_LEVEL);
        printf("  encoding =  %s\n", Etc::Image::EncodingFormatToString(format));
        printf("  error metric: %s\n", ErrorMetricToString(Etc::ErrorMetric::BT709));

        Etc::SourceImage sourceImage(inputFile, -1, -1);
        Etc::Image image((float *)sourceImage.GetPixels(),
                         sourceImage.GetWidth(), sourceImage.GetHeight(),
                         Etc::ErrorMetric::BT709);
        Etc::Image::EncodingStatus encStatus = Etc::Image::EncodingStatus::SUCCESS;
        encStatus = image.Encode(format, Etc::ErrorMetric::BT709, ETCCOMP_DEFAULT_EFFORT_LEVEL, 8,1024);

        printf("  encode time = %dms\n", image.GetEncodingTimeMs());
        printf("EncodedImage: %s\n", outputFile);
        printf("status bitfield: %u\n", encStatus);

        Etc::File etcFile(outputFile, Etc::File::Format::INFER_FROM_FILE_EXTENSION,
                          format,
                          image.GetEncodingBits(), image.GetEncodingBitsBytes(),
                          image.GetSourceWidth(), image.GetSourceHeight(),
                          image.GetExtendedWidth(), image.GetExtendedHeight());
        etcFile.Write();
    }
    else if (strcmp(compressType, "astc") == 0)
    {
        const char* blockSize = c->findOption("block", "6x6");
    }
    else
    {
        printf("unsupported compress type. \n");
        help();
    }
}