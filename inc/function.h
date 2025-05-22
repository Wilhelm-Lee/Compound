#ifndef COMPOUND_FUNCTION_H
# define COMPOUND_FUNCTION_H

# include "Compound/array.h"
# include "Compound/signature.h"
# include "Compound/status.h"
# include "Compound/stack.h"
# include "Compound/string.h"
# include "Compound/literalisation.h"

typedef struct Function {
  /* Types and parametres. */
  String signature;
  
  /* Function name. */
  String identity;
  
  /* Function body. */
  String content;
} Function;

Status Function_Create(Function *inst, void *funcptr);
Status Function_CopyOf(Function *inst, const Function other);
Status Function_Delete(Function *inst);
boolean   Function_Equals(const Function func1, const Function func2);
/* Returns total bytes written into stream. */
size_t Function_Print(FILE *stream, const Function func);

# define funcdecl(rtn, name, param)                        \
  nameof(rtn name param)

# define function(rtn, name, param, body)                  \
  funcdecl(rtn, name, param) nameof(body)

# define funcptr(rtn, name, ...)                           \
  (rtn)(*name)(__VA_ARGS__)

// LITERALISATION(Function);
// ARRAY(Function);
// STACK(Function);

#endif  /* COMPOUND_FUNCTION_H */
