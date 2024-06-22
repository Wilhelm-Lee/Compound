#ifndef FUNCTION_H
# define FUNCTION_H

# include <Compound/string.h>

typedef Var Type;

typedef struct {
  attr(registered 1)  Type type;
  
  attr(nullity false)  String identity;
  
  attr(alignwith 1)
  attr(optional true)  Type value;
} Parameter;

typedef void * Block;

typedef struct {
  Type returns;
  Array type(Parameter) params;
  Var type(Block) body;
} Function;

Status Function_Create(Function *inst, Type returns,
                       Array type(Parameter) params, Var type(Block) body);
Status Function_CopyOf(Function *inst, Function *other);
Status Function_Delete(Function *inst);
Status Function_Literalise(Function *inst, String *buff);
Status Function_Overwrite(Function *inst, Function *other);
bool   Function_Equal(Function *inst, Function *other);
bool   FunctionUtils_IsVariadic(Function *inst);

#endif  /* FUNCTION_H */
