# QR Code Generator

A simple and efficient QR code generator that converts text into QR codes in both PNG and SVG formats.

## Prerequisites

- C++ compiler with C++11 support
- libpng development library
- pkg-config

On Ubuntu/Debian, you can install the required packages with:
```bash
sudo apt-get install build-essential libpng-dev pkg-config
```

## Building the Project

1. Clone the repository:
```bash
git clone <your-repository-url>
cd qrcode-generator
```

2. Build the project using make:
```bash
make
```

This will create an executable named `qrcodegen`.

## Usage

The QR code generator accepts two command-line arguments:
1. The text to encode
2. The output file name (with either .png or .svg extension)

### Examples

Generate a PNG QR code:
```bash
./qrcodegen "Hello, World!" output.png
```

Generate an SVG QR code:
```bash
./qrcodegen "https://github.com" website.svg
```
### Run mobile app to read QR code 
pnpm install 
npx turbo run start --filter mobile

## Supported Output Formats

- PNG: Creates a bitmap image of the QR code
- SVG: Creates a scalable vector graphic of the QR code

## Error Handling

The program includes error handling for:
- Invalid number of arguments
- Unsupported file formats
- File creation errors
- QR code generation errors

## Clean Build

To clean the build files and start fresh:
```bash
make clean
```

## Project Structure

- `qrcodegen.hpp`: Header file containing QR code generation declarations
- `qrcodegen-impl.cpp`: Implementation of the QR code generation logic
- `qrcodegen-main.cpp`: Main program handling command-line arguments and file output
- `Makefile`: Build configuration

## License

This project uses the QR Code generation library by Project Nayuki, which is available under the MIT License. See the source files for the full license text.

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request
