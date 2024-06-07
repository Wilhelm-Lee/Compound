#include <Compound/stack.h>

Status Stack_Create(Stack *inst, int len)
{
  // /* Skip once len is negative. */
  // state((len < 0), InvalidParameter);
  
  // /* Initialise before access. */
  // if (!inst) {
  //   *inst = (Stack) {
  //     .members = NULL,
  //     .len = len
  //   };
    
  //   /* Assign for each member. */
  //   inst->members = malloc(len * sizeof(Var));
  //   for (register int i = 0; i < len; i++) {
  //     inst->members[i] = (Var) {
  //       .addr = NULL,
  //       .sz = 0
  //     };
  //   }
  // }
  
  return apply(NormalStatus);
}
Status Stack_CopyOf(Stack *inst, Stack *other);
Status Stack_Push(Stack *inst, Var *item);
Status Stack_Pop(Stack *inst);
bool Stack_IsEmpty(Stack *inst);
