#ifndef COMPOUND_LINK_H
# define COMPOUND_LINK_H

# include <stdint.h>

enum {
  LINK_ROLE_HEAD = 0,
  LINK_ROLE_BODY,
  LINK_ROLE_TAIL = INTMAX_MAX,
};

# define Link(type)  type##Link

# define DEFINE_LINK(type)                                 \
  struct type##Link {                                      \
    type data;                                             \
    struct type##Link *prev;                               \
    struct type##Link *next;                               \
    struct type##Link *head;                               \
    struct type##Link *tail;                               \
    int role;                                              \
    /* Deem @tail->next as @head. */                       \
    boolean looped;                                           \
    /* Swap @prev and @next when iterating. */             \
    boolean reverse;                                          \
  }

# define TYPEDEF_LINK(type)                                \
  typedef DEFINE_LINK(type)  type##Link;

# define FUNC_LINK(type)                                   \
  Status type##Link_Create(Link *inst, const type data, const type##Link *prev,\
                           const type##Link *next, const int role,\
                           const boolean looped, const boolean reverse);\
  Status type##Link_CopyOf(Link *inst, const Link other);  \
  Status type##Link_Delete(Link *inst);                    \
  Status type##Link_DeleteAll(Link *inst);                 \
  Status type##Link_GetIdx(const type##Link array, const size_t index, type *data);\
  Status type##Link_SetIdx(type##Link *inst, const size_t index, const type data);\
  Status type##Link_Insert(type##Link *inst, const size_t index, const type data);\
  Status type##Link_Remove(type##Link *inst, const size_t index);\
  Status type##Link_Concat(type##Link *inst, const size_t index, const type##Link other);\
  Status type##Link_Subtra(type##Link *inst, const size_t off, const size_t len);\
  int    type##Link_Search(const type##Link lnk, const char *item);\
  boolean   type##Link_Equals(const type##Link lnk1, const type##Link lnk2);

# define GLOBAL_LINK(type)                                 \
  static type##Link *Global##type##Link = NULL;            \
  static inline void type##Link_SetGlobal(type##Link *lnk) \
  {                                                        \
    Global##type##Link = lnk;                              \
  };

# define LITERALISATION_LINK(type)

# define LINK(type)                                        \
  TYPEDEF_LINK(type)                                       \
  FUNC_LINK(type)                                          \
  GLOBAL_LINK(type)

# define IMPL_LINK(type)



#endif  /* COMPOUND_LINK_H */
