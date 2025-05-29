#ifndef COMPOUND_SIGNATURE_H
# define COMPOUND_SIGNATURE_H

# include "string.h"

typedef struct {
  String type;
  String param;
} Signature;

Status Signature_Create(Signature *inst, const String expression);
Status Signature_CopyOf(Signature *inst, const Signature other);
Status Signature_Delete(Signature *inst);

Status Signature_Parse(Signature *store, const String expression);

# define append(cstr)\
{\
  String append = string(cstr);\
  ig concat(write, append);\
  ig call(String,, Delete) with (&append);\
}

# define appendch(ch)\
  ig call(Array, byte, Insert) with (&write->data, last(write->data), ch);

# define replace(opt, cstr)                                \
  at(opt, append(cstr))

# define keep(opt)                                         \
  at(opt, appendch(opt))

#endif  /* COMPOUND_SIGNATURE_H */
