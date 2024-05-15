#ifndef COMPOUND_OBJECT_H
# define COMPOUND_OBJECT_H

# include <Compound/array.h>
# include <Compound/namespace.h>
# include <Compound/type.h>

typedef struct {  // interface
  Var *data;
  Array buffer;
  int  (*Literalise)(void);
  int  (*Correspond)(Var *other);
} Object;

# define OBJECT_IMPLEMENTATION Var *data; Array buffer;\
          int (*Literalise)(void); int (*Correspond)(Var *other);

// private final String IDENTITY = ":D";
// public static final void main(String[] args) {}

typedef enum {
  EXPOSURE_PUBLIC = 0,
  EXPOSURE_PROTECTED,
  EXPOSURE_PACKAGE,
  EXPOSURE_PRIVATE
} Exposure;

typedef struct {
  Type type;
  Name identity;
} Parameter;

typedef struct {
  Type type;
  Name identity;
  Array parameters;
} Function;

typedef struct {
  Exposure prefix;
  bool dynamic;
  bool constant;
  Type type;
  Name identity;
  Array type(Parameter) parameters;
} Member;

typedef struct {
  Exposure prefix;
  Name identity;
  Array type(Parameter) parameters;
} Constructor;

/* As for destructors, they don't have prefixes or parameters,
   because at the time you need destructors, you're already talking
   in a environment that is private, private to this class alone.
   And you also won't need any parameters, because to destruct, everything
   you store in this class is bound to be vanished, and whatever you do to
   modify them, the results are the same.
   However, if you'd like to change something else that is not belong to
   this class, why don't you just do them before calling destructors?
   Therefor, in real practices, you won't find anything meaningful to let
   a destructor having a prefix or a parameter.
   
   Summary.
   Prefixes are fixed to be "Private";
   Parameters are fixed to be void.
 */
typedef struct {
  Name identity;
} Destructor;

typedef struct {
  OBJECT_IMPLEMENTATION

  char *identity;

  Array type(Member type(Constructor)) constructors;  // Array<Member<Constructor>>
  Member type(Destructor) destructor;  // Member<Destructor>

  Array type(Member type(?)) fields;  // Array<Member<?>>
  Array type(Member type(?)) methods;  // Array<Member<?>>
} Class;

typedef enum {
  MEMBER_ACCESS_READ  = 0b001,
  MEMBER_ACCESS_WRITE = 0b010,
  MEMBER_ACCESS_EXEC  = 0b100,
} MemberAccessMode;

Status Extern(Class *inst, Class *extrn);
Status Implement(Class *inst, Class *impl);

Status Class_Create(Class *inst, char *identity);
Status Class_CopyOf(Class *inst, Class *other);
Status Class_Constr(Class *inst, Array buffer, int (*Literalise)(void),
                    int (*Correspond)(Var*));
Status Class_Delete(Class *inst);
Status Class_GetIdentity(Class *inst, char *store);
Status Class_GetObject(Class *inst, Object *store);
Status Class_GetFieldByIdentity(Class *inst, Name *identity, Member *store)
       throws(MemberNotFound);
Status Class_GetMethodByIdentity(Class *inst, Name *identity, Member *store)
       throws(MemberNotFound);
Status Class_GetMembers(Class *inst, Array *store);
Status Class_MemberAccess(Class *inst, MemberAccessMode mode, Member *store);
Status Class_AddMember(Class *inst, int index, Member *target);
Status Class_RemoveMember(Class *inst, int index);

#endif  /* COMPOUND_OBJECT_H */
