#include "System.hpp"
#include "dev/hwid/MultibootHeaderInfo.hpp"
#include <stdint.h>
#include "boot.hpp"
#include "memory.hpp"
#include "dev/interrupts.hpp"
#include "dev/x86cpu.hpp"
#include "dev/ps2/ps2.hpp"
#include "dev/isrs/interruptHandlers.hpp"
#include "dev/keyboardScancodes.hpp"
#include "dev/cpuIO.hpp"

System* System::instance = 0;

System::System()
  : mbhi(multiboot_header_ptr)
{
  System::instance = this;
}

System* System::getInstance()
{
  if (System::instance == 0)
    instance = new System();
  return instance;
}

void debugSystemStuff(System* sys)
{
  ConsoleScreen* screen = sys->screen;
}

void System::init()
{
  initConsole();
  initMemoryManager();
  cpuInfo = new CPUInformation();
  initInterrupts();
  initTerminal();
  //debugSystemStuff();
  screen->print("Done with initialization.");
}

void System::initTerminal()
{
  terminal = (KernelTerminal*)malloc(sizeof(KernelTerminal));
  terminal->m_stdout = screen;
  terminal->m_stdout->print("Initialized Terminal\n");
}

void System::initConsole()
{
  screen = &vgaTextConsole;
  screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  screen->clear();
  screen->print("Welcome to AndrOS 0.0.1c\n\0");
}

void System::findBiggestChunkOfMemory(void*& begin, void*& end)
{
  MBHIMemoryMapEntry* entry = mbhi.m_mbhiMemoryMap;
  uint64_t contenderBegin = 56;
  uint64_t contenderEnd = 0;

  //Iterate through the entire buffer
  while((uint64_t)entry < ((uint64_t)mbhi.m_mbhiMemoryMap + (uint64_t)mbhi.m_mbhiMemoryMapLength))
  {
    // Computations
    uint64_t baseAddress = (((uint64_t)entry->baseAddressHigh) << 32) + ((uint64_t)entry->baseAddressLow);
    uint64_t endAddress  = ((uint64_t)entry->baseAddressHigh << 32) + ((uint64_t)entry->baseAddressLow);
    uint64_t length      = ((uint64_t)entry->lengthHigh << 32) + ((uint64_t)entry->lengthLow);
    endAddress += length;

    if (entry->type == Available)
    {
      //if (length > (contenderEnd - contenderBegin))
      {
        contenderEnd = endAddress;
        contenderBegin = baseAddress;
      }
    }
    
    // Update entry
    uint64_t buf = (uint64_t)entry;
    buf += entry->size + sizeof(entry->size);
    entry = (MBHIMemoryMapEntry*)buf;
  }

  begin = (void*)contenderBegin;
  end   = (void*)contenderEnd;
}

//#define DEBUG_PRINTOUTS
#define DEBUG_MEMORY_MAP
void System::initMemoryManager()
{
  kernelStackEnd = stack_ptr;
  kernelStackBegin = (void*)((uint32_t)(stack_ptr) - 16384);

  findBiggestChunkOfMemory(kernelHeapBegin, kernelHeapEnd);
#ifdef DEBUG_MEMORY_MAP
  screen->print("Printing MBH info:\n");
  mbhi.printMemoryTable(screen);
  screen->nl();
  screen->print("Largest chunk of memory at: ");
  screen->printlHex((uint64_t)kernelHeapBegin);
  screen->print(" to ");
  screen->printlHex((uint64_t)kernelHeapEnd);
  screen->nl();
#endif

  
  memoryManager.initGDT();

  memoryManager.m_heapStart = kernelHeapBegin;
  memoryManager.m_heapEnd   = kernelHeapEnd;
  memoryManager.m_heapCur   = kernelHeapBegin;

#ifdef DEBUG_PRINTOUTS
  screen->print("Size of a size_t:");
  screen->print(sizeof(size_t));
  screen->nl();

  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printlHex((uint64_t)memoryManager.m_heapCur);
  screen->nl();
#endif
  uint32_t* pInt1 = (uint32_t*)malloc(sizeof(uint32_t));
#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a 4 byte structure.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
#endif

  uint32_t* pInt2 = (uint32_t*)malloc(sizeof(uint32_t));

  if (pInt1 == pInt2)
  {
    //TODO: Throw error.
    screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_RED);
    screen->print("ERROR! Allocator overlapping memory!");
    screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  }

#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a 4 byte structure.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
  screen->print("Allocated two 32-bit pointers: ");
  screen->printHex((uint32_t)pInt1);
  screen->print(" and ");
  screen->printHex((uint32_t)pInt2);
  screen->nl();
#endif

  //Test new operator
  GDTEntry* testEntry = new GDTEntry();
#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a ");
  screen->printHex(sizeof(GDTEntry));
  screen->print(" byte structure with new operator.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
#endif

  GDTEntry* testEntry2 = new GDTEntry();
#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a ");
  screen->printHex(sizeof(GDTEntry));
  screen->print(" byte structure with new operator.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
#endif
  
}

#if 0
__attribute__((interrupt))
void handler(InterruptFrame* frame)
#endif

extern "C" void modifyScancode();

void testInterruptSystem(ConsoleScreen* screen)
{
#if 0
  screen->print("sizeof(IDTGate)=");
  screen->print(sizeof(IDTGate));

  screen->print("Interrupt Return Value: "); 
  screen->print(interruptReturnValue);
  screen->nl();

  screen->print("Now changing Interrupt Return Value: "); 
  //changeIRV();
  interruptReturnValue = 46;
  screen->print(interruptReturnValue);
  screen->nl();

  //Not yet.
  screen->print("Interrupt Return Value: "); 
  screen->print(interruptReturnValue);
  screen->nl();
  screen->print("EFLAGS = ");
  screen->printHex(getEFlags());
  screen->print("Enabling interrupt mask...");

  enableInterruptFlag();
  screen->print("...done\n");
  screen->print("EFLAGS = ");
  screen->printHex(getEFlags());


  screen->print("Calling test interrupt... ");
  testInterrupts();
  screen->print("Interrupt Return Value: "); 

  screen->print(interruptReturnValue);
  screen->nl();
#endif

// Test ability to call C++ from assembly
#if 0
  uint32_t value = 26;
  screen->print(value);
  screen->print(" + 2 = ");
  screen->print(addTwo(value));
  screen->nl();
#endif

// Test ability to get data out of assembly module
#if 0
  keyboardScancode = 0;
  modifyScancode();
  screen->print("Scancode: ");
  screen->print(keyboardScancode);
 
  modifyScancode();
  screen->print("Modified Scancode: ");
  screen->print(keyboardScancode);
  screen->nl();
#endif

  //screen->print("PS/2 test: ");
  //screen->printHex(PS2Keyboard::testPS2Controller());

  screen->print(". PS/2 status: ");
  screen->printHex(PS2Keyboard::readStatusRegister());

  //screen->print(" Char: ");
  //screen->print(PS2Keyboard::readByte());

  screen->nl();
 
}

void System::initInterrupts()
{
  //this->idt = malloc(sizeof(IDT));
  screen->print("Constructing IDT");

  //TODO: Debug why the following line doesn't work.
  //IDT* dummyIdt2 = new IDT();
  idt = (IDT*) malloc(sizeof(IDT));
  idt->init();
  screen->print("Initialized IDT.\n");
  //TODO
  screen->print("Loading IDT...");
  idt->load();
  screen->print(" done.\n");
  testInterruptSystem(screen);
}

void System::pollKeyboardAndHandle()
{
  inb(0x60);
  uint32_t oldKey = 0;
  while (1)
  {
    uint32_t key = inb(0x60);
    if (key != oldKey)
    {
      keystrokes->add(key);
      processKeyboardInput(keystrokes, terminal->m_stdin);
      terminal->handleInput();
      oldKey = key;
    }
  }
}
