#ifndef COMPOUND_NAMESCOPE_H
# define COMPOUND_NAMESCOPE_H

# include <Compound/array.h>
# include <Compound/name.h>

typedef struct {
  Array(Name) name_data;
  
} NameScope;

# define NameScope(T) NameScope

Status NameScope_Create(NameScope *inst);
Status NameScope_CopyOf(NameScope *inst, NameScope *other);
Status NameScope_CreateName(NameScope *inst, Name *buff);
Status NameScope_RemoveName(NameScope *inst, Name idx);
Status NameScope_UpdateLatest(NameScope *inst, Name idx);
bool   NameScope_Equals(NameScope *inst, NameScope *other);
bool   NameScope_IsAvailable(NameScope *inst, Name idx);

/* Universal Attribute NameScope. (U.A.N.) */
NameScope(Attribute) UniversalAttributeNameScope;
NameScope MemoryAllocationRegistry;

#endif  /* COMPOUND_NAMESCOPE_H */
