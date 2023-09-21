# C Language Interpreter
Interpreter for a Turing-complete language like C

This is a simple interpreter for the C programming language written in C. It allows you to execute C code directly from the command line.

## Installation

To install the C language interpreter, follow these steps:

1. Clone the repository: 
```bash
git clone https://github.com/ALanovaya/ortgl.git
```
2. Navigate to the project directory:
```bash
cd ortgl/interpreter
```
3. Compile the source code:
```bash
gcc *.c -o ortgl
```
4. Run the interpreter:
```bash
ortgl main.ortgl
```

## Usage

As input, the interpreter takes the path to the file in which the code is written in the ortgl language, the response is output using the console output function.

Here are some examples of how to use the interpreter:

1. Print "Hello, World!":

```c
printf("Hello, World!");
```

2. Calculate the sum of two numbers:

```c
a = 5;
b = 10;
sum = a + b;
printf(sum);
```

3. Find the factorial of a number:

```c
n = 5;
factorial = 1;

for (i = 1; i <= n; i++) {
    factorial *= i;
}

printf(factorial);
```
## Limitations

Please note that this interpreter is a simplified version of the C programming language and may not support all features and syntax of the full language. It is intended for educational purposes and basic code execution.

The language only has integer types. Input and output functions are implemented, that is, the program can accept data and print it. Loops are also written: for and while, and branches, that is, the language is Turing complete

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.