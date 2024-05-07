# 🔭 Compile Sight

*Note: This project is currently under development and is not yet usable.*

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

Start the server on port 8080 and open the browser automatically.

```zsh
./csight --port 8080 --auto
```

Forwards the compiler output in the terminal (by default, it does not print the original output).
```zsh
./csight --forward
```

## 📚 Usage

### Command Line Interface

```zsh
Usage: csight [options]

Options:
  -h, --help            Show this help message and exit
  -v, --version         Show the version and exit
  -p, --port <port>     Start the server on the specified port
  -a, --auto            Automatically open the browser
  -f, --forward         Forward the original compiler output
```

### Web Interface

The web interface displays the filtered compiler output, color-coded error messages, and a file tree to navigate the project structure. The interface allows users to filter error messages by file, line, and error type, making it easier to identify and fix issues.

![Web Interface](
https://user-images.githubusercontent.com/68425608/137594073-3b3b3b3b-1b3b-4b3b-8b3b-3b3b3b3b3b3b.png)



## Future Improvements



