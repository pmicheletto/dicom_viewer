# DicomViewer

## Description
DicomViewer is a C++ project developed to fulfill the requirements of the technical test for the **C++ Developer position at Dr.Tis**.

The project focuses on building a simple graphical DICOM viewer, emphasizing clean code, modern C++ practices, and cross-platform build configuration using CMake.

## Technologies
- **C++17**
- **CMake**
- **Raylib** (graphics and windowing)
- **TinyFileDialogs** (native file dialogs)

## Build Requirements
- CMake >= 3.14
- C++ compiler with C++17 support
- Git
- OpenGL-compatible environment (required by Raylib)

## Build Instructions

```bash
mkdir build
cmake -S . -B build
cmake --build build
