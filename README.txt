README FILE:

Welcome to my compiler project. I've always thought compilers were cool, but
after talking to my adviser at Penn State I found out that PSU's two major
compilers courses aren't being taught until after I graduate. At first I
thought "that's a bummer," but then I thought "dawg, just figure it out
yourself." So here we are. I like C, so I decided I'd write a C compiler in C.
If you have any questions or suggestions feel free to let me know and thanks for
checking it out!

TODO:
-Shift operators <<, >>, and the other assignment operators
like <<=, >>=, ^=, |= , &= (and others I can't think of right
now if I'm forgetting them)

-Add in the e notation for floats

-Make a cleaner makefile

DONE:
-Make a state machine for the assignment operators (+=, /=, %=, etc)
and the math operators (+, -, *, /, %)

-bitwise and logical operators (&&, ||, &, |, ^, !, ~)

-Fixed bug where parsing "()" caused an infinite loop instead of failing.
The problem was due to forgetting to reset the lookahead pointer to *pos
(basically missed back tracking in one spot).

-Updated the make file a bit just to add debugging info and fix missing
new compilation every time a file is changed. Still could add more options.
