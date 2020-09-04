# Getting into Protected Mode

## 4 September 2020
Last night I made a DAG showing what I needed to do to get the keyboard
working. And my goodness, it's a lot. But we start where we can. Also last
night, I made it so my Makefile will automatically compile assembly code
objects as well. Yesterday and tonight I've been looking into the GDT & Co.

It seemed to me as though the GDT stuff would benefit from a healthy dose
of pure assembly code, so I'm trying my hand at integrating assembly and
C++ code. I looked though my computer architecture book and found that
the first argument to a function should be passed in through `%rdi`. 
I wrote up an "increment" module, declared it as an extern "C" int (int),
but to no avail. The function did nothing. So I wrote a C++ function that
did the same thing so I could compare assembly code. I then disabled optimizations
and peeked at the assembled code with objdump. Then I encountered errors
because __cxx_pure_virtual() was not defined. So I stuck a dummy definition
in my kernel and proceeded. To my surprise, the 
first arguments were being passed in on the stack. That doesn't seem right, does it?

Well, doing a little bit of research, it seems like in the days of 32-bit, i386 
architecure, arguments were passed on the stack (last first), This 32-bit, 64-bit
nonsense is getting to be a bit much. I want to get some working memory management
and I/O access before I decide if it's worth pursuing a 64-bit build. I need to
continue on one path rather than doing a frenetic dash from one development track
to another. Put another way, I need to do a depth-first development rather than
breadth-first.

Anyway, this is the 32-bit calling convention:

- *Function-preserved registers*: `ebx, esi, edi, ebp,` and `esp`. 
- Stack grows downward
- *Scratch registers*: `eax, ecx, edx`
- *Return value placed in*: `eax`
