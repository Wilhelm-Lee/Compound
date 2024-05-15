#ifndef COMPOUND_SECURITY_REGISTRY_H
# define COMPOUND_SECURITY_REGISTRY_H

# include <Compound/memman.h>

/*
    By calculating SHA256, we can identify whether the original data has been
  modified maliciously or not.
    However, In current particular cases, it is contradictive that we would
  wish to use the same approach to validate and protect the data that we used
  for validating and protecting other data.
    Thus, it is not practical to use current existing methods to create
  a way that is 100% perfect for data safety and computation security.
    What we actually need is to ensure that our data from either struct or
  union wouldn't be modifies easily, not in C, but in Classify, the language
  that rely on this very project, Compound.
 */

typedef struct {
  Memory data;
  char SHA256[256];
} RegItem;  // 276 Bytes

typedef union {
  RegItem items[256];
} RegTable256;  // 4416 Bytes | 4.3125 KiB

typedef union {
  RegItem items[128];
} RegTable128;  // 2208 Bytes | 2.15625 KiB

typedef union {
  RegItem items[64];
} RegTable64;  // 1104 Bytes | 1.078125 KiB

typedef union {
  RegItem items[32];
} RegTable32;  // 552 Bytes

typedef union {
  RegItem items[16];
} RegTable16;  // 276 Bytes

typedef union {
  RegItem items[8];
} RegTable8;  // 138 Bytes

bool Registry_Validate(const RegItem const *item, const RegResult const *result);
bool Registry_Calculate(const RegItem const *item, const RegResult const *result);
bool RegistryTable64_



#endif  /* COMPOUND_SECURITY_REGISTRY_H */
