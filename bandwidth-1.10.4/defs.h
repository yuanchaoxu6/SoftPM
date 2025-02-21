/*============================================================================
  bandwidth, a benchmark to estimate memory transfer bandwidth.
  Copyright (C) 2005-2021 by Zack T Smith.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  The author may be reached at 1@zsmith.co.
 *===========================================================================*/

//---------------------------------------------------------------------------
// Change log
// 0.18	Grand unified release supports x86/intel64/arm, linux/win32/winmobile.
// 0.19	Now have 128-bit writer that goes to cache AND one that bypasses.
// 0.20	Added my bmplib and graphing of output. Also added --slow option.
// 0.21	Adds random testing. Min chunk size = 256 B. Allows non-2^n chunks.
// 0.22	Adds register-to-register and register-to/from-stack transfers.
// 0.23	Adds vector-to-vector and register-to-vector transfers, & Mac support.
// 0.24	Adds network bandwidth tests from this PC to specified others.
// 0.25	Made network tests bidirectional to test asymmetric networks.
// 0.26	Fixes to prevent certain vector instructions being used w/AMD chips.
// 0.27 Added 128-byte tests for greater precision.
// 0.28	Added use of CPUID.
// 0.29 Added more 128-byte tests.
// 0.30 Adds cache identification for Intel CPUs in 64-bit mode.
// 0.31 Adds cache identification for Intel CPUs in 32-bit mode.
// 0.32 Added AVX support.
// 1.0	Moved graphing logic into BMPGraphing. Added string-instruction test.
// 1.1	Switched to larger font in graphing module.
// 1.2	Re-added ARM 32 support.
// 1.3	Added CSV output support. Added 32-bit Raspberry π 3 support.
// 1.4	Added 256-bit routines RandomReaderAVX, RandomWriterAVX.
// 1.4.1 Added --limit parameter.
// 1.4.2 Fixed compiler warnings.
// 1.5 Fixed AVX writer bug that gave inaccurate results. Added nice mode.
// 1.5.1 Fixed crashing bug.
// 1.5.2 Disabled AVX tests if AMD CPU.
// 1.6	Converted the code to object-oriented C.
// 1.6.1 Moved CPUID calls into utility*asm. Detection of SHA, SGX, CET.
// 1.6.2 Converted some code to Object-Oriented C.
// 1.6.3 Removed string instruction tests. Improved Object-Oriented C.
// 1.6.4 Code cleanup.
// 1.6.5 Fixed Linux issues.
// 1.7 Separated object-oriented C (OOC) from bandwidth.
// 1.8 OO improvements. Makefile improvements. Added Win64 support.
// 1.9 OO improvements. Win64. Removed Linux fbdev.
// 1.10 ARM64 and improved ARM32. Apple M1 support.
//---------------------------------------------------------------------------

#ifndef _DEFS_H
#define _DEFS_H

#define RELEASE "1.10.4"

#include <stdint.h>

#define require(COND) if(COND){}

extern uint64_t getMicrosecondTime ();

extern int Reader (void *ptr, unsigned long size, unsigned long loops);
extern int Reader_128bytes (void *ptr, unsigned long size, unsigned long loops);
extern int RandomReader (void *ptr, unsigned long n_chunks, unsigned long loops);

extern int Writer (void *ptr, unsigned long size, unsigned long loops, unsigned long value);
extern int Writer_128bytes (void *ptr, unsigned long size, unsigned long loops, unsigned long value);
extern int RandomWriter (void *ptr, unsigned long size, unsigned long loops, unsigned long value);
extern int RandomWriterVector (void *ptr, unsigned long size, unsigned long loops, unsigned long value);

extern int RegisterToRegister (unsigned long);

extern int StackReader (unsigned long);
extern int StackWriter (unsigned long);

extern int RegisterToVector (unsigned long);	// SSE2
extern int Register8ToVector (unsigned long);	// SSE2
extern int Register16ToVector (unsigned long);	// SSE2
extern int Register32ToVector (unsigned long);	// SSE2
extern int Register64ToVector (unsigned long);	// SSE2

extern int VectorToVector (unsigned long);	// SSE2

extern int VectorToVectorAVX (unsigned long);	

extern int VectorToRegister (unsigned long);	// SSE2
extern int Vector8ToRegister (unsigned long);	// SSE2
extern int Vector16ToRegister (unsigned long);	// SSE2
extern int Vector32ToRegister (unsigned long);	// SSE2
extern int Vector64ToRegister (unsigned long);	// SSE2

extern int Copy (void*, void*, unsigned long, unsigned long);	
extern int CopySSE (void*, void*, unsigned long, unsigned long);
extern int CopyAVX (void*, void*, unsigned long, unsigned long);
extern int CopySSE_128bytes (void*, void*, unsigned long, unsigned long);

extern int ReaderAVX (void *ptr, unsigned long, unsigned long);
extern int ReaderAVX512 (void *ptr, unsigned long, unsigned long);
extern int RandomReaderAVX (void *ptr, unsigned long, unsigned long);

extern int ReaderSSE2 (void *ptr, unsigned long, unsigned long);
extern int ReaderSSE2_bypass (void *ptr, unsigned long, unsigned long);
extern int RandomReaderSSE2 (unsigned long **ptr, unsigned long, unsigned long);
extern int RandomReaderSSE2_bypass (unsigned long **ptr, unsigned long, unsigned long);

extern int WriterAVX (void *ptr, unsigned long, unsigned long, unsigned long);
extern int RandomWriterAVX (void *ptr, unsigned long, unsigned long, unsigned long);

extern int WriterSSE2 (void *ptr, unsigned long, unsigned long, unsigned long);
extern int RandomWriterSSE2(unsigned long **ptr, unsigned long, unsigned long, unsigned long);

extern int ReaderSSE2_128bytes(void *ptr, unsigned long, unsigned long);
extern int WriterSSE2_128bytes(void *ptr, unsigned long, unsigned long, unsigned long);

extern int ReaderSSE2_128bytes_bypass (void *ptr, unsigned long, unsigned long);
extern int WriterSSE2_128bytes_bypass (void *ptr, unsigned long, unsigned long, unsigned long);

extern int WriterAVX_bypass (void *ptr, unsigned long, unsigned long, unsigned long);
extern int WriterSSE2_bypass (void *ptr, unsigned long, unsigned long, unsigned long);
extern int RandomWriterSSE2_bypass (unsigned long **ptr, unsigned long, unsigned long, unsigned long);

extern void IncrementRegisters (unsigned long count);
extern void IncrementStack (unsigned long count);

extern void get_cpuid_family (char *family_return);
extern void get_cpuid_cache_info (uint32_t *array, int index);
extern unsigned get_cpuid1_ecx ();
extern unsigned get_cpuid1_edx ();
extern unsigned get_cpuid7_ebx ();
extern unsigned get_cpuid7_ecx ();
extern unsigned get_cpuid_80000001_ecx ();
extern unsigned get_cpuid_80000001_edx ();

#define CPUID80000001_EDX_INTEL64 (1<<29)	// "Long Mode" on AMD.
#define CPUID80000001_ECX_SSE4A (1<<6)
#define CPUID80000001_EDX_NX (1<<20)
#define CPUID80000001_EDX_MMXEXT (1<<22)

#define CPUID1_EDX_MMX (1<<23)
#define CPUID1_EDX_SSE (1<<25)
#define CPUID1_EDX_SSE2 (1<<26)
#define CPUID1_ECX_SSE3 (1)
#define CPUID1_ECX_SSSE3 (1<<9)
#define CPUID1_ECX_SSE41 (1<<19)
#define CPUID1_ECX_SSE42 (1<<20)
#define CPUID1_ECX_AES (1<<25)	// Encryption.
#define CPUID1_ECX_AVX (1<<28)	// 256-bit YMM registers.
#define CPUID1_ECX_HYPER_GUEST (1<<31)
#define CPUID1_EDX_HTT (1<<28)

#define CPUID7_EBX_ADX (1<<19)
#define CPUID7_EBX_SHA (1<<29)
#define CPUID7_EBX_SGX (1<<2)
#define CPUID7_EBX_AVX2 (1<<5)
#define CPUID7_EBX_AVX512F (1<<16)
#define CPUID7_ECX_CET (1<<7)

#define FBLOOPS_R 400
#define FBLOOPS_W 800
#define FB_SIZE (640*480*2)

#endif

