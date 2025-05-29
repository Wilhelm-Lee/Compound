#include <Compound/class_method.h>

Status ClassMethod_Create(ClassMethod *inst,
                          const AccessQualifier perm,
                          const LifespanQualifier lfspn, const boolean virtual,
                          const String identity, const String content,
                          const String signature, ...)
{
  avail(inst);
  state(!length(signature), InvalidMethodSignature);
  state(!length(identity), InvalidMethodIdentity);
  
  inst->perm = perm;
  inst->lfspn = lfspn;
  inst->virtual = virtual;
  fail(call(String,, CopyOf) with (&inst->identity, identity));
  fail(call(String,, CopyOf) with (&inst->content, content));
  fail(call(String,, CopyOf) with (&inst->signature, signature));
  
  
}

Status ClassMethod_CopyOf(ClassMethod *inst, const ClassMethod other);
Status ClassMethod_Delete(ClassMethod *inst);
boolean ClassMethod_Equals(const ClassMethod method1, const ClassMethod method2);
Status ClassMethod_Override(ClassMethod *inst, const String content);

