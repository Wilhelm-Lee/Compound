#include <Compound/class_field.h>

Status ClassField_Create(ClassField *inst, const AccessQualifier perm,
                         const LifespanQualifier lfspn, const String signature,
                         const String identity, const String value)
{
  avail(inst);
  state(!length(signature), InvalidFieldSignature);
  state(!length(identity), InvalidFieldIdentity);
  
  inst->perm = perm;
  inst->lfspn = lfspn;
  fail(call(String,, CopyOf) with (&inst->signature, signature));
  fail(call(String,, CopyOf) with (&inst->identity, identity));
  fail(call(String,, CopyOf) with (&inst->value, value));
  
  RETURN(NormalStatus);
}

Status ClassField_CopyOf(ClassField *inst, const ClassField other)
{
  avail(inst);
  state(!length(other.signature), InvalidFieldSignature);
  state(!length(other.identity), InvalidFieldIdentity);
  
  inst->perm = other.perm;
  inst->lfspn = other.lfspn;
  fail(call(String,, CopyOf) with (&inst->signature, other.signature));
  fail(call(String,, CopyOf) with (&inst->identity, other.identity));
  fail(call(String,, CopyOf) with (&inst->value, other.value));
  
  RETURN(NormalStatus);
}

Status ClassField_Delete(ClassField *inst)
{
  avail(inst);
  
  inst->perm = 0;
  inst->lfspn = 0;
  fail(call(String,, Delete) with (&inst->signature));
  inst->signature = (String)EMPTY;
  fail(call(String,, Delete) with (&inst->identity));
  inst->identity = (String)EMPTY;
  fail(call(String,, Delete) with (&inst->value));
  inst->value = (String)EMPTY;
  
  RETURN(NormalStatus);
}

boolean ClassField_Equals(const ClassField field1, const ClassField field2)
{
  return (
    field1.perm == field2.perm
    && field1.lfspn == field2.lfspn
    && !(compare(field1.signature, field2.signature))
    && !(compare(field1.identity, field2.identity))
    && !(compare(field1.value, field2.value))
  );
}
