#ifndef STACK_H
# define STACK_H

# include <Compound/var.h>
# include <Compound/status.h>

typedef struct {
  Var *members;
  int len;
} Stack;

Status Stack_Create(Stack *inst, int len);
Status Stack_CopyOf(Stack *inst, Stack *other);
Status Stack_Push(Stack *inst, Var *item);
Status Stack_Pop(Stack *inst);
bool Stack_IsEmpty(Stack *inst);

#endif  /* STACK_H */
