#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <png.h>
#include "qrcodegen.hpp"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// Function prototypes
static std::string toSvgString(const QrCode &qr, int border);
static void writePng(const QrCode &qr, const char* filename);
static bool hasEnding(std::string const &fullString, std::string const &ending);

// The main application program.
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <text> <output.[png|svg]>" << std::endl;
        return EXIT_FAILURE;
    }

    const char *text = argv[1];
    const char *outFile = argv[2];
    const QrCode::Ecc errCorLvl = QrCode::Ecc::HIGH;

    try {
        const QrCode qr = QrCode::encodeText(text, errCorLvl);
        
        std::string outFileName(outFile);
        if (hasEnding(outFileName, ".svg")) {
            std::string svg = toSvgString(qr, 4);  // 4 modules for border
            std::ofstream out(outFile);
            if (!out) {
                throw std::runtime_error("Failed to create output file");
            }
            out << svg;
            out.close();
        } else if (hasEnding(outFileName, ".png")) {
            writePng(qr, outFile);
        } else {
            throw std::runtime_error("Unsupported file format. Use .png or .svg extension");
        }
        
        std::cout << "QR code generated successfully to " << outFile << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

// Helper function to check file extension
static bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    return false;
}

// Write QR Code to a PNG file
static void writePng(const QrCode &qr, const char* filename) {
    const int scale = 8;  // Pixels per module
    const int border = 4;  // Number of border modules
    const int size = qr.getSize();
    const int outSize = (size + 2 * border) * scale;
    
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        throw std::runtime_error("Failed to create PNG file");
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        throw std::runtime_error("Failed to create PNG write structure");
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        throw std::runtime_error("Failed to create PNG info structure");
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        throw std::runtime_error("Error during PNG creation");
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, outSize, outSize, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    std::vector<png_byte> rowData(outSize * 3);
    for (int y = 0; y < outSize; y++) {
        for (int x = 0; x < outSize; x++) {
            int moduleX = x / scale - border;
            int moduleY = y / scale - border;
            bool isDark = moduleX >= 0 && moduleX < size &&
                         moduleY >= 0 && moduleY < size &&
                         qr.getModule(moduleX, moduleY);
            
            png_byte color = isDark ? 0 : 255;
            rowData[x * 3 + 0] = color;  // R
            rowData[x * 3 + 1] = color;  // G
            rowData[x * 3 + 2] = color;  // B
        }
        png_write_row(png, rowData.data());
    }

    png_write_end(png, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

// Returns a string of SVG code for an image depicting the given QR Code, with the given number
// of border modules. The string always uses Unix newlines (\n), regardless of the platform.
static std::string toSvgString(const QrCode &qr, int border) {
    if (border < 0)
        throw std::domain_error("Border must be non-negative");
    long dimension = qr.getSize() + border * 2;
    
    std::ostringstream sb;
    sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
    sb << dimension << " " << dimension << "\" stroke=\"none\">\n";
    sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
    sb << "\t<path d=\"";
    for (long y = 0; y < qr.getSize(); y++) {
        for (long x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                if (x != 0 || y != 0)
                    sb << " ";
                sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
            }
        }
    }
    sb << "\" fill=\"#000000\"/>\n";
    sb << "</svg>\n";
    return sb.str();
}
