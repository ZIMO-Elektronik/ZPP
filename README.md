# ZPP

[![build](https://github.com/ZIMO-Elektronik/ZPP/actions/workflows/build.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ZPP/actions/workflows/build.yml) [![tests](https://github.com/ZIMO-Elektronik/ZPP/actions/workflows/tests.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ZPP/actions/workflows/tests.yml) [![license](https://img.shields.io/github/license/ZIMO-Elektronik/ZPP)](https://github.com/ZIMO-Elektronik/ZPP/raw/master/LICENSE)

<img src="data/images/logo.svg" width="15%" align="right">

ZPP is an acronym for ZIMO Precompiled Project, a file format for [sound project](https://www.zimo.at/web2010/sound/tableindex_EN.htm) updates. ZPP files are currently used by the following products:
- Command stations
  - [ZIMO MXULF](http://www.zimo.at/web2010/products/InfMXULF_EN.htm)
  - [Z21](https://www.z21.eu/en)
- Decoders
  - [ZIMO MN decoders](http://www.zimo.at/web2010/products/mn-nicht-sound-decoder_EN.htm)
  - [ZIMO small-](http://www.zimo.at/web2010/products/ms-sound-decoder_EN.htm) and [large-scale MS decoders](http://www.zimo.at/web2010/products/ms-sound-decoder-grossbahn_EN.htm)
  - [ZIMO small-](http://www.zimo.at/web2010/products/lokdecoder_EN.htm) and [large-scale MX decoders](http://www.zimo.at/web2010/products/lokdecodergrosse_EN.htm)

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#file-format-specification">File Format Specification</a></li>
      <ul>
        <li><a href="#header">Header</a></li>
        <li><a href="#flash-data">Flash Data</a></li>
      </ul>
    <li><a href="#getting-started">Getting Started</a></li>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#build">Build</a></li>
      </ul>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>

## File Format Specification
A ZPP file consists of a header and a binary blob of sound project data.

## Header
| Offset | Length | Content                  | Description                                                                                                                                   |
| ------ | ------ | ------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------- |
| 0      | 2      | ZPP file identifier      | SP - legacy sound project<br>**MS** - sound project<br>**MN** - non-sound project<br>**SX** - sound extension<br>**NX** - non-sound extension |
| 2      | 2      | Version                  | 1 - PIC<br>2 - STM32                                                                                                                          |
| 4      | 1      | Decoder type             | 0 or specific decoder ID                                                                                                                      |
| 5      | 4      | Flash data address       | Offset to flash data in file                                                                                                                  |
| 9      | 4      | Flash data length        | Length of flash data in file                                                                                                                  |
| 13     | 4      | CVs data address         | Offset to CVs data in file                                                                                                                    |
| 17     | 2      | CVs data length          | Length of CVs data in file                                                                                                                    |
| 19     | 1      | Coded flag               | 0 - not coded<br>1 - coded                                                                                                                    |
| 20     | 4      | Load code                | Load code for flash data decryption                                                                                                           |
| 24     | 4      | CRC                      | CRC                                                                                                                                           |
| 28     | 4      | Sound file names address | Offset to sound file strings in file                                                                                                          |
| 32     | 2      | Sound file names count   | Number of sound file strings in file                                                                                                          |
| 34     | 4      | Author name address      | Offset to author string in file                                                                                                               |
| 38     | 4      | Author e-mail address    | Offset to author e-mail string in file                                                                                                        |
| 42     | 4      | Firmware address         | Offset to firmware in file                                                                                                                    |
| 46     | 2      | Pointer to second header | Offset to second ZPP header                                                                                                                   |
| 48     | 4      | Script names address     | Offset to script strings in file                                                                                                              |
| 52     | 2      | Script names count       | Number of script strings in file                                                                                                              |
| 54     | 4      | FA names address         | Offset to function output strings in file                                                                                                     |
| 58     | 2      | FA names count           | Number of function output strings in file                                                                                                     |

## Flash Data
Within the flash data there is also the developer code, which must be sent to the decoders in advance in order to load an encrypted project. The [MDU](https://github.com/ZIMO-Elektronik/MDU) and [ZUSI](https://github.com/ZIMO-Elektronik/ZUSI) protocols make use of this.

| Offset | Length | Content        | Description                              |
| ------ | ------ | -------------- | ---------------------------------------- |
| 2190   | 4      | Developer code | Developer code for flash data decryption |

## Getting Started
### Prerequisites
- C++23 compatible compiler
- [CMake](https://cmake.org/) ( >= 3.25 )

### Installation
This library is meant to be consumed with CMake.

```cmake
# Either by including it with CPM
cpmaddpackage("gh:ZIMO-Elektronik/ZPP@0.0.0")

# or the FetchContent module
FetchContent_Declare(
  ZPP
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/ZPP"
  GIT_TAG v0.0.0)

target_link_libraries(YourTarget PRIVATE ZPP::ZPP)
```

### Build
If the build is running as a top-level CMake project then tests and a small example will be generated.
```sh
cmake -Bbuild
cmake --build build --target ZPPExamples
```

## Usage
```cpp
auto zpp_file{zpp::read("./data/Da_Di_Collection_ZIMO-mfx-16Bit_S02.zpp")};

// Print ID
std::cout << "ZPP ID " << zpp_file.id << "\n";

// Print version
std::cout << "ZPP version " << zpp_file.version << "\n";

// Print coded
std::cout << "ZPP coded? " << (zpp_file.coded ? "yes" : "no") << "\n";
```