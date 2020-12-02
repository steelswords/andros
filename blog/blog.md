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

## 5 September 2020
Man, what a day. I spent almost all of it implementing stupid little itoa and atoi
functions. This is to let me read all of the junk in the multiboot header and print
it to the screen. It's now 8:30 pm and I think I'll be able to get to some of it tonight.
If I'm lucky.


## 15 September 2020
At some point in implementing a whole bunch of string functions from libc,
I realized that all this work would turn out much better for me if I just
implemented a string class for the kernel instead. I think it was exactly
the same time that GCC started spitting out a bunch of incoherent errors 
about multiple definitions of those libc functions. You'd think that these
were of course because I was defining functions in header files or including
source files, but no. I grep-ed through my whole codebase. I checked all
the compiled object files for redefined labels. Nothing. But a kstring
class is a better way to do this anyway. I hope at some point I can get
unstuck from my terminal/string manipulation ways and get to some I/O.

Pro to kstring refactor: Awesome class. Finally some sensible string 
stuff.
Con to kstring refactor: All of those unit tests I worked so hard on?
Ruined. Obliterated.

Although, in architecture design news, we're keeping all the kernel-level
strings in a 16 KiB block right under the stack. I'm going to keep track of
what space has been already allocated. At this point, there will be no 
deallocation.

This is the first memory management-related thing I will have done.

##1 Nov 2020
Well, it's been a while since I've updated this log. Since September I have
gotten a rudimentary kernel heap going and started to work on interrupts.

Man, this is a mess.

The modern way to do interrupts is to go through an advanced programmable interrupt
controller. This APIC replaces the old Intel 8259 programmable interrupt controller (PIC)
but it comes with a bunch of documenation. I eventually decided I couldn't wade through 
all that documentation and still make any sort of project deadlines, so just like choosing
32 bit over 64 bit architecture at this point, we're stuck with the legacy hardware
implementation until I get more time to work on this (if that will ever happen).

But just now another setback has occurred. Remember how I was using an older version of 
the GCC cross compiler I need because it was already compiled and ready for me to use?
Well, it turns out that the __attribute__((interrupt)) function attribute isn't available
until three whole major versions later. Smh. Rather than trying to reinvent my whole toolchain,
I'm just going to proceed with the older version and use assembly wrappers around my interrupts,
I think.

## 13 November 2020
I have been struggling with interrupts for a while now, and tonight I got some really
encouraging results!

I got an interrupt handler to be called from software! Sure, it generated a double
fault, but I got it! Feels good, man.

Some of the previous problems I was having were the GDT was not actually getting loaded,
so when an ISR was called, it would generate a general protection fault, which would go
to the Exception #0xd handler, which would triple fault. Right now my int 0x35 is double
faulting, but I think that shouldn't be too hard to fix.

I also found some great features of QEMU that dump out the CPU state when an interrupt is
being called. Good stuff! And aparently I just need to implement three hook functions (that
output over serial, sure) and I can have a remote GDB session. What the heck? That's too
good to be true, methinks. (Although I know firsthand that serial comms are nontrivial to do.
Not terribly difficult, but it will take some time. And at this point, I'm going to need all
the time I can get.
