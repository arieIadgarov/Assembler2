Labratory-C-Final-Project

Project Grade - 95

This is the Final project (mmn 14) in Laboratory C 20465- 2024a course at the Open University.
It's a comprehensive assembler program for a specific assembly language, written in C.

---------------------------------------------------------------------------------------------------------------------------------------

Project Structure

The assembler works in three main stages:


Pre-Processing - 

The pre-assembler reads the source file line by line and identifies macro definitions. If a macro is identified, the lines of the macro are added to a macro table and replaced in the source file.

First Pass - 

The first pass identifies symbols (labels) and assigns them numerical values representing their corresponding memory addresses 
and generates the final machine code on the lines of code that does not contains any symbols, replacing operation names with their binary equivalents .

Second Pass - 

the second pass 
generates the final machine code on the lines of code that containing symbols while replacing symbol names with their assigned memory locations.

