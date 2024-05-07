# 🔭 Compile Sight

**Compile Sight** enhances the readability of C++ compiler error messages by filtering, formatting, and color-coding them, making it easier for developers to understand and fix code issues. This tool aims to turn the often daunting compiler output into a more approachable and informative feedback loop.

## 📋 Features

- Supports multiple C++ compilers, including GCC, Clang, (MSVC in the future).
- Filters compiler output to highlight relevant information.
- Web Interface: Offers a user-friendly web interface to navigate and filter error messages.


## 🪓 Parsing

- Robustness: Ensures robust parsing by validating file paths in error messages, accommodating unusual file name characters. The parser must be executed in the correct directory relative to the paths in the error messages, otherwise, it will not detect errors.
- Path Validation: If a path is not valid, it is ignored, ensuring that parsing continues uninterrupted.
- Color Handling: Effectively handles 🎨 color escape sequences in compiler outputs, ensuring seamless parsing of colored error logs.


## 🚀 Getting Started

Clone the repository and compile the source code:

```zsh
git clone 'https://github.com/123Untitled/compile_sight.git'
make
```

Run the executable with the compiler output as input:

```zsh
$CXX -c main.cpp 2>&1 | ./csight
```


