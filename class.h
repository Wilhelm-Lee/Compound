#ifndef COMPOUND_CLASS_H
# define COMPOUND_CLASS_H

# include <Compound/object.h>

typedef struct {} _Interface;

typedef struct {
  String identity;
  
  Array type(DynStruct) structs;
  Array type(DynUnion) unions;
  Array type(DynEnum) enums;
  Array type(FunctionPointer) functionptrs;
  Array type(Field) fields;
  
  // Interface[] *implementations
  Array type(Var type(Interface)) *implementations;
  // Class *predecessor
  Var type(Class) *predecessor;
} Class;

Status Class_Create(Class *inst, Array type(DynStruct) structs,
                    Array type(DynUnion) unions,
                    Array type(DynEnum) enums,
                    Array type(FunctionPointer) functionptrs,
                    Array type(Field) fields);
Status Class_CreateEmpty(Class *inst);
Status Class_CopyOf(Class *inst, Class *other);
Status Class_Implement(Class *inst, _Interface *interface);
Status Class_Extern(Class *inst, Class *)

#endif  /* COMPOUND_CLASS_H */
