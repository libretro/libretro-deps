
#ifndef LIBMPEG2_CONFIG_H
#define LIBMPEG2_CONFIG_H

/* =========================================================================
 *  libmpeg2 - static config header for baked-in builds (libretro)
 *  Covered targets: unix/linux, win/msvc, osx, emscripten, rpi*,
 *  armv7/armv8 families, gcw0 (mips), miyoo/miyoomini (arm),
 *  libnx (switch), ps3/psl1ght, wii, wiiu, ctr (3DS), vita,
 *  qnx, genode, ios/tvos.
 * ========================================================================= */

/* ------ Package info (minimal set) -------------------------------------- */
#define PACKAGE         "libmpeg2"
#define PACKAGE_NAME    "libmpeg2"
#define PACKAGE_VERSION "0.5.1"
#define VERSION         PACKAGE_VERSION

/* ------ Standard headers availability ----------------------------------- */
#define STDC_HEADERS  1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1

/* ------ Compiler builtins ------------------------------------------------ */
#if defined(__GNUC__) || defined(__clang__)
#  define HAVE_BUILTIN_EXPECT 1
#endif

/* ------ Endianness -------------------------------------------------------
 * Priority:
 *   1) macros already provided by the core build (RETRO_IS_BIG_ENDIAN /
 *      WORDS_BIGENDIAN),
 *   2) compiler's __BYTE_ORDER__.
 * ------------------------------------------------------------------------ */
#if defined(RETRO_IS_BIG_ENDIAN) && (RETRO_IS_BIG_ENDIAN+0)
#  ifndef WORDS_BIGENDIAN
#    define WORDS_BIGENDIAN 1
#  endif
#elif !defined(WORDS_BIGENDIAN)
#  if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#    define WORDS_BIGENDIAN 1
#  endif
#endif

/* =========================================================================
 *  Architecture / acceleration selection
 *  libmpeg2 provides optimized code paths for:
 *   - x86 (MMX)
 *   - PowerPC (AltiVec/VMX)
 *   - SPARC (VIS)
 *   - ARM (generic C/ASM)
 * ========================================================================= */

/* ---- x86/x86_64 -> enable MMX unless building for web/wasm ------------- */
#if (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)) \
    && !defined(__EMSCRIPTEN__)
#  ifndef ARCH_X86
#    define ARCH_X86 1
#  endif
#  ifndef HAVE_MMX
#    define HAVE_MMX 1
#  endif
#endif

/* ---- PowerPC family (PS3/PSL1GHT, Wii/WiiU, generic PPC) ---------------- */
#if defined(__powerpc__) || defined(__powerpc64__) || defined(__ppc__) || defined(__PPC__) || defined(__PPC64__) \
 || defined(PLAYSTATION3) || defined(__PS3__) || defined(__PSL1GHT) || defined(WIIU) || defined(GEKKO)
#  ifndef ARCH_PPC
#    define ARCH_PPC 1
#  endif
  /* AltiVec/VMX:
   *  - PS3/PSL1GHT typically support VMX/VSX,
   *  - Wii/WiiU generally do NOT.
   * Enable only when the compiler signals support or when building PSL1GHT.
   */
#  if defined(__ALTIVEC__) || defined(__VEC__) || defined(__VSX__) || defined(__PSL1GHT)
#    ifndef HAVE_ALTIVEC
#      define HAVE_ALTIVEC 1
#    endif
#  endif
#endif

/* ---- SPARC/VIS ---- */
#if defined(__sparc__) || defined(__sparc)
#  ifndef ARCH_SPARC
#    define ARCH_SPARC 1
#  endif
#endif

/* ---- ARM / AArch64 (rpi*, libnx, ios/tvos, vita, ctr, miyoo, etc.) ------ */
#if defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64) \
 || defined(ARM) || defined(__ARM__)
#  ifndef ARCH_ARM
#    define ARCH_ARM 1
#  endif
  /* libmpeg2 does not require NEON macros explicitly; ARM optimized code
   * in the classic tree is generic C/ASM. Add extra discriminants here
   * in the future only if you need them. */
#endif

/* ---- MIPS (GCW0 is mipsel) --------------------------------------------- */
#if defined(__mips__) || defined(__mips) || defined(__MIPS__)
#  ifndef ARCH_MIPS
#    define ARCH_MIPS 1
#  endif
#endif

/* ---- Emscripten (WASM) -> force generic pipeline ----------------------- */
#if defined(__EMSCRIPTEN__)
#  undef ARCH_X86
#  undef HAVE_MMX
#  undef ARCH_PPC
#  undef HAVE_ALTIVEC
#  undef ARCH_SPARC
#  undef ARCH_ARM
#  undef ARCH_MIPS
#endif

/* =========================================================================
 *  Small compatibility helpers
 * ========================================================================= */

/* MSVC: inline in C is __inline, and restrict may be missing */
#if defined(_MSC_VER) && !defined(__cplusplus)
#  ifndef inline
#    define inline __inline
#  endif
#  ifndef restrict
#    define restrict __restrict
#  endif
#endif

/* POSIX memalign: enable only if your toolchain truly supports it
 * (libmpeg2 does not require it by default) */
/* #define HAVE_POSIX_MEMALIGN 1 */

/* Some iOS/tvOS builds define IOS; this header is decoupled from GL drivers. */
#ifdef IOS
#  ifndef HAVE_POSIX_MEMALIGN
   /* Enable if your iOS/tvOS toolchain provides posix_memalign */
#    define HAVE_POSIX_MEMALIGN 1
#  endif
#endif

#endif /* LIBMPEG2_CONFIG_H */

