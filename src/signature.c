#include <Compound/signature.h>

Status Signature_Create(Signature *inst, const String expression)
{
  avail(inst);

  fail(call(String,, Create) with (&inst->type, 1, sizeof(char)));
  fail(call(String,, Create) with (&inst->param, 1, sizeof(char)));

  // fail(call(Signature,, Parse) with (inst, expression));
  
  RETURN(NormalStatus);
}

Status Signature_CopyOf(Signature *inst, const Signature other);

Status Signature_Delete(Signature *inst)
{
  avail(inst);
  
  fail(call(String,, Delete) with (&inst->type));
  inst->type = (String)EMPTY;
  fail(call(String,, Delete) with (&inst->param));
  inst->param = (String)EMPTY;
  
  RETURN(NormalStatus);
}

Status Signature_Parse(Signature *store, const String expression)
{
  avail(store);
  state(!length(expression), EmptyString);

  module(braces, '{', '}');
  String *write = &store->type;
  parse(fallback(expression), {
    ig printf("Step %d"NEWLINE
              "%s"NEWLINE  // Signature.
              "%*c^"NEWLINE  // Indicator.
              "%s"NEWLINE,  // Production.

              (int)(app - fmt),
              fallback(expression),
              (int)(app - fmt),
              ' ',
              fallback(store->type));
    
    keep(',')
    keep('*')
    keep('.')

    // update(braces)
    at(braces.opt_enter, {
      braces.level += 1;
    })
    
    at(braces.opt_leave, {
      braces.level -= 1;
    })
    
    if (inside(braces)) {
      appendch(CURRENT)
      continue;
    }
    
    skip(' ')
    
    at(':', {
      write = &store->param;
    })
    
    at('b', append("boolean"))
    at('c', append("char"))
    at('i', append("int"))
    at('f', append("float"))
    at('d', append("double"))
    at('u', append("unsigned"))
    at('v', append("void"))
    at('w', append("wchar_t"))
    at('E', append("enum "))
    at('S', append("struct "))
    at('U', append("union "))
    at('r', append(" restrict "))
    at('p', append("*"))
    at('l', append(" long "))
    at('s', append(" short "))
  })
  
  RETURN(NormalStatus);
}

# undef append
