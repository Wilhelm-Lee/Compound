#ifndef COMPOUND_CLASSES_INTEGER_H
# define COMPOUND_CLASSES_INTEGER_H

# include <Compound/class.h>

typedef Class Integer;

/*
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
*/

/* STATIC INSTANCE OPERATIONS. */
Status Integer_Create(Integer *inst, Var *value);
Status Integer_FromInt(Integer *inst, int *value);
Status Integer_FromUnsignedInt(Integer *inst, unsigned int *value);
Status Integer_ToInt(Integer *inst, int *store);
Status Integer_ToUnsignedInt(Integer *inst, unsigned int *store);
Status Integer_CopyOf(Integer *inst, Integer *other);
Status Integer_Delete(Integer *inst);
bool   Integer_Equal(Integer *inst, Integer *other);

/* DYNAMIC INSTANCE OPERATIONS. */
Status Integer_Generate(Integer *inst,
                        Array *structs          type(dynStruct),
                        Array *unions           type(DynUnion),
                        Array *enums            type(DynEnum),
                        Array *functionptrs     type(FunctionPointer),
                        Array *fields           type(Field),
                        Array *implementations  type(Var type(Interface)),
                        Var   *predecessor      type(Class));
Status Integer_Demolish(Integer *inst);
Status Integer_Literalise(Integer *inst, String *buff);

/* INSTANCE UTILITIES. */
Status IntegerUtils_Cast(Integer *inst, Integer *store, size_t sizeInBytes);

#endif  /* COMPOUND_CLASSES_INTEGER_H */
