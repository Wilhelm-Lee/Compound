/* This demo is written by Google Gemini. */
#include <stdio.h>

#include "../inc/entry.h"

/* Using Main for the program entry as requested. */
int Main(void)
{
  /* Keep the compiler happy by ignoring unused Main arguments. */
  ignore args;
  ignore envs;

  /* === 1. STRING FORMATTING & CREATION === */
  /* Compound makes string building and formatting a breeze. */
  String *welcome = format("Welcome to %s! Running smoothly on Dev-C++!", "Compound");
  printf("%s\n\n", (char *)refbyte(welcome, 0));

  /* === 2. DYNAMIC ARRAYS & COMPOSITION === */
  /* No more manual reallocations! Compose lets you build populated arrays on the fly. */
  Array(String) *features = Compose(
    Array(String),
    string("Rich Strings"),
    string("Dynamic Arrays"),
    string("Macro-based OOP Syntax"),
    string("Automatic Loops")
  );

  if (!features) {
    return 1;
  }

  printf("Top %lld Features of Compound:\n", capacity(Array(String), features));
  printf("------------------------------\n");

  /* === 3. SEAMLESS ITERATION === */
  /* The 'refeach' macro provides a clean, readable foreach-style loop[cite: 1]. */
  refeach (String, feat, features, {
    printf(" -> %s\n", (char *)refbyte(feat, 0));
  })

  /* === 4. MACRO OOP MANIPULATION === */
  /* In-place reversal of the dynamic array using the 'reverse' macro[cite: 1]. */
  printf("\nLet's reverse that list!\n");
  reverse(Array(String), features);
  refeach (String, feat, features, {
    printf(" <- %s\n", (char *)refbyte(feat, 0));
  })

  /* === 5. ADVANCED STRING OPERATIONS === */
  String *code = string("Writing C code can be hard.");
  String *target = string("hard");
  String *replacement = string("fun and expressive");

  /*
   * Note: replace() automatically frees the original string internally,
   * so we must reassign the returned pointer back to our variable[cite: 1].
   */
  code = replace(&code, target, replacement, 0);
  printf("\nString Replacement Result:\n%s\n", (char *)refbyte(code, 0));

  /* === 6. BASIC TYPE ARRAYS & LOOP MACROS === */
  printf("\nWorking with basic integer arrays:\n");
  Array(int) *numbers = array(int, 5);

  /* Using the built-in 'loop' macro[cite: 1] */
  loop (i, capacity(Array(int), numbers)) {
    int value = (i + 1) * 10;
    /* Basic type arrays take a pointer to the value when setting[cite: 1]. */
    set(Array(int), numbers, i, &value);
  }

  /* Using 'iterate' for basic array indexing[cite: 1] */
  iterate (Array(int), i, numbers, {
    printf("[%lld]: %d  ", i, get(Array(int), numbers, i));
  })
  printf("\n");

  /* === 7. PROPER MEMORY CLEANUP === */
  /* Deallocating our independent String instances[cite: 1]. */
  Delete(String, welcome);
  Delete(String, target);
  Delete(String, replacement);
  Delete(String, code);

  /* 'erase' safely deallocates inner Object elements before Deleting the Array container[cite: 1]. */
  erase(Array(String), features);
  Delete(Array(String), features);

  /* Basic type arrays don't need erase(), just Delete()[cite: 1]. */
  Delete(Array(int), numbers);

  printf("\nDemo completed! Memory properly cleaned up.\n");
  return 0;
}
