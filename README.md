# Bitwise Instruction Interpreter

A simple interpreter that emulates a basic encoder/decoder for arithmetic operations.
The interpreter is capable of decoding and executing elementary arithmetic instructions such as **addition, subtraction, multiplication, and division**.
At its core, information is represented as bits which can be categorized into *instructions* and *data*.
The interpretor processes a string of bits as an instruction and then executes it.

![INSTRUCTION_FORMAT](/pictures/instruction.png)

## Instruction Decoding

The binary instruction is given in a specific format which includes the number of: 
- operations to execute: `N` [The number of operations, represented by **3** bits].
- the operation codes: `Op` [The operation code for `+`,`-`,`*`,`/`, represented by **2** bits].
- the operand sizes: `Dim` [The size of an operand, represented by **4** bits].

The output will display *N, the operators, and the operand size*.

## Instruction Execution

This task extends `Instruction Decoding` by adding **operand reading and instruction execution**.

Operands are read as `unsigned short` numbers and the number of operands.

The number is based on the following formula: `((N + 1) * Dim) / 16`, which is used to calculate the number of operands, where:

- `16` is significant as it represents the size, in bits, of the `unsigned short` numbers being read as operands.
- `Dim` and the number `N + 1` are multiplied together to calculate the total bit length of the operand data.

**Note:** Dividing this total by 16 converts the bit length into the count of unsigned short numbers, aligning with the data type's storage capacity and ensuring accurate operand handling within the system's architecture.
The instruction execution is carried out in the order of operations received.

## Build the Project

The provided examples illustrate the usage of a command-line tool named bitwise for performing operations encoded in a specific format. The tool seems to support at least two commands: `decode` and `execute`.
But first build the project, follow the next commands:

```bash
$ cd build
$ make build
gcc ../bitwise.c -lm -o ../bitwise
```

**Example 1:**

Format `decode` command: `./bitwise decode <Code>`

```bash
$ ./bitwise decode 1646133248
Command: decode
Code: 1646133248
N : 4
Ops : + - + + 
Dim : 16
```

Format `execute` command: `./bitwise execute <Code> <operand1> <operand2> <operand3> ...`

**Example 2:**

```bash
$ ./bitwise execute 1142423552 3 7 8 10
Command: execute
Code: 1142423552
Enter block:
operands: 3 7 8 10 
Answer: 90
```

### Specifications

- The total size of an instruction does not exceed `23 (3 + 8 * 2 + 4)` bits and thus fits into an `unsigned int`.
- Numbers read from the keyboard are considered **unsigned**, but the ***result may be negative***; hence, an int type variable is used for the result.
- Only the resulting number is displayed for compatibility with the automated checker.
