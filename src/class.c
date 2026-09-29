/*
 * This file is part of Compound library.
 * Copyright (C) 2024-2026  William Lee
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

/** @file class.c */

#include "../inc/class.h"

struct Class {
  AccessVisibility visibility;
  String *identifier;
  Class *super;
  Class *this;
  Array(Field) *fields;
  Array(Method) *methods;
  Constructor *constructor;
  Destructor *destructor;
  Method *Equals;
  Method *Literalise;
};

static inline String *GenerateArrayImplementations(Class *const inst)
{
  return append(
    nll,
    string("IMPL_ARRAY("),
    inst->identifier,
    string(")"),
    string(NL)
  );
}

static inline String *GenerateMethodDeclarations(Class *const inst)
{
  return lit(
    Array(Method),
    inst->methods,
    nll,
    nll,
    string(NL),
    yes,
    yes,
    yes,
    yes,
    yes,
    no,
    yes
  );
}

static inline String *GenerateMethodImplementations(Class *const inst)
{
  return lit(
    Array(Method),
    inst->methods,
    nll,
    nll,
    string(NL),
    yes,
    yes,
    yes,
    yes,
    yes,
    yes,
    no
  );
}

static String *GenerateTypedef(Class *const inst)
{
  if (!inst) {
    return nll;
  }

  return append(
    nll,
    string("typedef struct "),
    inst->identifier,
    string(" "),
    inst->identifier,
    string(";"),
    string(NL)
  );
}

static String *GenerateArrayDeclarations(Class *const inst)
{
  return append(nll, string("ARRAY("), inst->identifier, string(")"NL));
}

static String *GenerateStruct(Class *const inst)
{
  String *const indent = string("  ");
  String *const newline = string(NL);

  String *lit = append(
    nll,
    string("struct "),
    inst->identifier,
    string(" {"NL),
    lit(
      Array(Field),
      inst->fields,
      indent,
      Concat(String, newline, indent),
      newline,
      no,
      yes
    ),
    string("};"NL)
  );

  Delete(String, newline);
  Delete(String, indent);

  return lit;
}

static String *GenerateObjectEssentialDeclarations(Class *const inst)
{
  if (!inst) {
    return nll;
  }

  String *const lit_constructor = lit(
    Constructor, inst->constructor, no, no, yes, yes, yes, no, no
  );

  foutln(stderr, inst->identifier);
  return nll;

  char *const identifier_cstr = inst->identifier ? flatten(char, inst->identifier) : nll;
  char *const constructor_cstr = lit_constructor ? flatten(char, lit_constructor) : nll;

  const char *const identifier = identifier_cstr ? identifier_cstr : "";
  const char *const constructor = constructor_cstr ? constructor_cstr : "";

  String *const result = format(
    "%s *%s_Create%s;" NL
    "%s *%s_CopyOf(%s *const other);" NL
    "void %s_Delete(%s *const this);" NL
    "boolean %s_Equals(%s *const inst, %s *const other);" NL
    "String *%s_Literalise(%s *const inst);" NL
    NL,
    identifier, identifier, constructor,
    identifier, identifier, identifier,
    identifier, identifier,
    identifier, identifier, identifier,
    identifier, identifier
  );

  Deallocate(constructor_cstr);
  Deallocate(identifier_cstr);
  Delete(String, lit_constructor);

  return result;
}

static char *restrict const GENERATE_OBJECT_ESSENTIAL_IMPLEMENTATIONS =
  "%s *%s_Create%s" NL
  "{" NL
  "  %s *const this = Allocate(sizeof(%s));" NL
  "  if (!this) {" NL
  "    return nll;" NL
  "  }" NL
  NL
  "  %s" NL
  "}" NL
  NL
  "void %s_Delete(%s *const this)" NL
  "{" NL
  "  if (!this) {" NL
  "    return;" NL
  "  }" NL
  "  %s" NL
  NL
  "  Deallocate(this);" NL
  "}" NL
  NL
  "boolean %s_Equals(%s *const this, %s *const other)" NL
  "{" NL
  "  if (!this || !other) {" NL
  "    return false;" NL
  "  }" NL
  NL
  "  if (this == other) {" NL
  "    return true;" NL
  "  }" NL
  NL
  "  %s" NL
  "}" NL
  NL
  "String *%s_Literalise(%s *const this)" NL
  "{" NL
  "  if (!this) {" NL
  "    return nll;" NL
  "  }" NL
  NL
  "  %s" NL
  "}" NL
;

static String *GenerateObjectEssentialImplementations(Class *const inst)
{
  if (!inst) {
    return nll;
  }

  Array(Parameter) *constructor_params =
  Getter(Signature, Parameters,
    Getter(Function, Signature,
      Getter(Method, Function,
        Getter(Constructor, Method, inst->constructor)
      )
    )
  );

  String *lit_create_sig = lit(
    Constructor, inst->constructor, no, no, yes, yes, yes, no, no
  );
  String *lit_create_body = lit(
    Constructor, inst->constructor, no, no, no, no, no, yes, no
  );

  String *prefix_other = string("other->");
  String *sep_other = string(", other->");
  String *lit_copy_params = lit(
    Array(Parameter), constructor_params, prefix_other, sep_other, nll, no, yes
  );

  String *const lit_del_body = lit(
    Destructor, inst->destructor, no, no, no, no, no, yes, no
  );
  String *const lit_eq_body = lit(
    Method, inst->Equals, no, no, no, no, no, yes, no
  );
  String *const lit_lit_body = lit(
    Method, inst->Literalise, no, no, no, no, no, yes, no
  );

  const char *const identifier_cstr =
    inst->identifier ? flatten(char, inst->identifier) : "";
  const char *const create_sig_cstr =
    lit_create_sig ? flatten(char, lit_create_sig) : "";
  const char *const create_body_cstr =
    lit_create_body ? flatten(char, lit_create_body) : "";
  const char *const del_body_cstr =
    lit_del_body ? flatten(char, lit_del_body) : "";
  const char *const eq_body_cstr =
    lit_eq_body ? flatten(char, lit_eq_body) : "";
  const char *const lit_body_cstr =
    lit_lit_body ? flatten(char, lit_lit_body) : "";

  String *const lit_result = format(
    GENERATE_OBJECT_ESSENTIAL_IMPLEMENTATIONS,
    /* _Create */
    identifier_cstr, identifier_cstr, create_sig_cstr,
    identifier_cstr, identifier_cstr,
    create_body_cstr,
    /* _Delete */
    identifier_cstr, identifier_cstr,
    del_body_cstr,
    /* _Equals */
    identifier_cstr, identifier_cstr, identifier_cstr,
    eq_body_cstr,
    /* _Literalise */
    identifier_cstr, identifier_cstr,
    lit_body_cstr
  );

  Delete(String, lit_lit_body);
  Delete(String, lit_eq_body);
  Delete(String, lit_del_body);
  Delete(String, lit_copy_params);
  Delete(String, sep_other);
  Delete(String, prefix_other);
  Delete(String, lit_create_body);
  Delete(String, lit_create_sig);

  return lit_result;
}

static char *GenerateYearString(void)
{
  time_t timestamp = time(null);
  struct tm *timer = gmtime(&timestamp);
  const short len = 5 * sizeof(char);
  char *year = Allocate(len);
  if (!year) {
    return nll;
  }
  strftime(year, len, "%Y", timer);
  return year;
}

static String *GenerateLicenseBanner(void)
{
  return format(
    "/*"NL
    " * This file is part of Compound library."NL
    " * Copyright (C) 2024-%s  William Lee"NL
    " *"NL
    " * This library is free software; you can redistribute it and/or"NL
    " * modify it under the terms of the GNU Library General Public"NL
    " * License as published by the Free Software Foundation; either"NL
    " * version 2 of the License, or (at your option) any later version."NL
    " *"NL
    " * This library is distributed in the hope that it will be useful,"NL
    " * but WITHOUT ANY WARRANTY; without even the implied warranty of"NL
    " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU"NL
    " * Library General Public License for more details."NL
    " *"NL
    " * You should have received a copy of the GNU Library General Public"NL
    " * License along with this library; if not, see"NL
    " * <https://www.gnu.org/licenses/>."NL
    " */"NL
    ""NL,
    GenerateYearString()
  );
}

static String *GenerateHeaderContent(Class *const inst)
{
  if (!inst) {
    return null;
  }

  char *restrict const identifier_cstr = flatten(char, inst->identifier);
  String *format = format(
    "/** @file %s.h */"NL
    ""NL
    "#ifndef COMPOUND_CLASS_%s_H"NL
    "# define COMPOUND_CLASS_%s_H"NL
    ""NL
    "# include \"../inc/class.h\""NL
    ""NL
    "typedef Class %s;"NL
    "ARRAY(%s)"NL
    ""NL
    "typedef struct class_%s class_%s;"NL
    "  /* Methods (signature). */"NL
    "  %s"NL
    "  /* Constructor (signature). */"NL
    "  %s"NL
    "  /* Destructor (signature). */"NL
    "  %s"NL
    ""NL
    "#endif  /* COMPOUND_CLASS_%s_H */"NL,
    identifier_cstr,
    identifier_cstr,
    identifier_cstr,
    identifier_cstr,
    identifier_cstr,
    identifier_cstr,
    identifier_cstr,
    flatten(char, lit(Array(Method), inst->methods, null, null, null, yes, yes, yes, yes, yes, no, no)),
    flatten(char, lit(Constructor, inst->constructor, yes, yes, yes, yes, yes, no, no)),
    flatten(char, lit(Destructor, inst->destructor, yes, yes, yes, yes, yes, no, no)),
    identifier_cstr
  );

  Deallocate(identifier_cstr);

  return format;
}

static String *GenerateSourceContent(Class *const inst)
{
  if (!inst) {
    return null;
  }

  return Concat(String,
    GenerateLicenseBanner(),
    lit(
      Array(Method),
      inst->methods,
      null,
      string(NEWLINE),
      null,
      yes,
      yes,
      yes,
      yes,
      yes,
      yes,
      no
    )
  );
}

static boolean RecreateHeader(Stream *const header, Class *const inst)
{
  if (!header || !inst) {
    return false;
  }

  if (!Open(header)) {
    return false;
  }

  if (!Write(header, Concat(String, GenerateLicenseBanner(), GenerateHeaderContent(inst)))) {
    ig Close(header);
    return false;
  }

  return Close(header);
}

static boolean RecreateSource(Stream *const source, Class *const inst)
{
  if (!source || !inst) {
    return false;
  }

  if (!Open(source)) {
    return false;
  }

  if (!Write(source, Concat(String, GenerateLicenseBanner(), GenerateSourceContent(inst)))) {
    ig Close(source);
    return false;
  }

  return Close(source);
}

Class *Class_Create(
  const AccessVisibility visibility,
  String *const identifier,
  Class *const super,
  Array(Field) *const fields,
  Array(Method) *const methods,
  Constructor *const constructor,
  Destructor *const destructor,
  Method *const Equals,
  Method *const Literalise
) {
  if (!identifier || blank(identifier)) {
    return null;
  }

  Class *const inst = Allocate(sizeof(Class));
  if (!inst) {
    return null;
  }

  String *const CLASS_IDENTIFIER_STR = identifier;

  inst->visibility = visibility;
  inst->identifier = CopyOf(String, identifier);
  inst->super = super;
  inst->this = inst;
  inst->fields = fields ? fields : array(Field, 0);
  inst->methods = methods ? methods : array(Method, 0);
  inst->constructor = constructor;
  inst->destructor = destructor;
  if (Equals) {
    inst->Equals = Equals;
  } else {
    String *const returning = string("boolean");
    String *const identifier = string("Equals");
    Function *const DefaultEqualsFunction = function (
      returning,
      identifier,
      params_str(
        param_str(inst->identifer),
        param_str(inst->identifier)
      ), {
        return false;
      }
    );

    Method *const DefaultEqualsMethod = Create(
      Method,
      ACCESS_VISIBILITY_PUBLIC,
      inst->identifier,
      DefaultEqualsFunction
    );

    inst->Equals = DefaultEqualsMethod;

    Delete(String, identifier);
    Delete(String, returning);
  }

  if (Literalise) {
    inst->Literalise = Literalise;
  } else {
    String *const returning = string("boolean");
    String *const identifier = string("Literalise");
    Function *const DefaultLiteraliseFunction = function (
      returning,
      identifier,
      params_str(
        param_str(inst->identifer),
        param_str(inst->identifier)
      ), {
        return false;
      }
    );

    Method *const DefaultLiteraliseMethod = Create(
      Method,
      ACCESS_VISIBILITY_PUBLIC,
      inst->identifier,
      DefaultLiteraliseFunction
    );

    inst->Literalise = DefaultLiteraliseMethod;

    Delete(String, identifier);
    Delete(String, returning);
  }

  Delete(String, CLASS_IDENTIFIER_STR);

  return inst;
}

Class *Class_CopyOf(Class *const other)
{
  if (!other) {
    return null;
  }

  /* With sharing the same @name as well as the @predecessor, it is
   * not ideal to distinguish the duplication and the original instance using
   * conventional approaches.
   *
   * It is worth noticing that to be able to identify two instances of Class,
   * users are therefore needed to use UID, which, is effectively separated
   * logically from the fields embedded in the struct, and, can be utilised to
   * distinguish instances apart.
   *
   * With that said, Equals is recognising the @identifier for comparison over
   * the equality check on @name.
   */
  Class *const inst = Create(
    Class,
    other->visibility,
    Concat(String, other->identifier, string(" copy")),
    CopyOf(Class, other->super),
    CopyOf(Array(Field), other->fields),
    CopyOf(Array(Method), other->methods),
    CopyOf(Constructor, other->constructor),
    CopyOf(Destructor, other->destructor),
    CopyOf(Method, other->Equals),
    CopyOf(Method, other->Literalise)
  );
  if (!inst) {
    return null;
  }

  return inst;
}

void Class_Delete(Class *const inst)
{
  if (!inst) {
    return;
  }

  Delete(String, inst->identifier);
  erase(Array(Method), inst->methods);
  Delete(Array(Method), inst->methods);
  Delete(Constructor, inst->constructor);
  Delete(Destructor, inst->destructor);
  erase(Array(Field), inst->fields);
  Delete(Array(Field), inst->fields);
  Delete(Method, inst->Equals);
  Delete(Method, inst->Literalise);
  Deallocate(inst);
}

boolean Class_Equals(const Class *const inst, const Class *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  return Equals(String, inst->identifier, other->identifier)
      && inst->super == other->super
      && Equals(Array(Method), inst->methods, other->methods, Method_Equals);
}

String *Class_Literalise(
  Class *const inst,
  boolean need_member_definition
) {
  if (!inst) {
    return null;
  }

  String *lit = append(
    nll,
    GenerateTypedef(inst),
    GenerateArrayDeclarations(inst),
    GenerateStruct(inst),
    GenerateObjectEssentialDeclarations(inst),
    GenerateMethodDeclarations(inst)
  );

  if (need_member_definition) {
    lit = append(
      lit,
      GenerateObjectEssentialImplementations(inst),
      GenerateArrayImplementations(inst),
      GenerateMethodImplementations(inst)
    );
  }

  return lit;
}

boolean Class_Recreate(
  Stream *const header,
  Stream *const source,
  Class *const inst
) {
  if (!inst || !header || !source) {
    return false;
  }

  if (!Open(header)) {
    return false;
  }

  if (!Open(source)) {
    Close(header);
    return false;
  }

  boolean ret = RecreateHeader(header, inst) && RecreateSource(source, inst);

  ig Close(source);
  ig Close(header);

  return ret;
}

Class *Class_AddField(Class *const inst, Field *const field)
{
  if (!inst || !field) {
    return inst;
  }

  inst->fields = call(Array(Field), Insert, inst->fields, -1, field);

  return inst;
}

Class *Class_AddMethod(Class *const inst, Method *const method)
{
  if (!inst || !method) {
    return inst;
  }

  inst->methods = call(Array(Method), Insert, inst->methods, -1, method);

  return inst;
}

void Class_Inherit(Class *const inst, Class *const super)
{
  if (!inst || !super) {
    return;
  }

  inst->super = super;
  inst->fields = Append(Array(Field), inst->fields, super->fields);
  inst->methods = Append(Array(Method), inst->methods, super->methods);
  call(Constructor, Inherit, inst->constructor, super->constructor);
  call(Destructor, Inherit, inst->destructor, super->destructor);
}

void Class_OverrideMethod(Class *const inst, String *const method_identifier, String *const body_text)
{
  if (!inst || !method_identifier) {
    return;
  }

  Method *const found = Class_GetMethodByIdentifier(
    inst, method_identifier
  );
  if (!found) {
    return;
  }

  Body *const body = Getter(
    Function,
    Body,
    Getter(Method, Function, found)
  );

  Setter(Body, Text, body, body_text ? body_text : string(""));
}

void Class_SetConstructor(Class *const inst, Constructor *const constructor)
{
  if (!inst) {
    return;
  }

  Delete(Constructor, inst->constructor);

  inst->constructor = constructor;
}

void Class_SetDestructor(Class *const inst, Destructor *const destructor)
{
  if (!inst) {
    return;
  }

  Delete(Destructor, inst->destructor);

  inst->destructor = destructor;
}

Field *Class_GetFieldByIdentifier(
  Class *const inst,
  String *const field_identifier
) {
  if (!inst) {
    return nll;
  }

  refeach (Field, field, inst->fields, {
    if (Equals(String, field_identifier, Getter(Field, Identifier, field))) {
      return field;
    }
  })

  return nll;
}

Method *Class_GetMethodByIdentifier(
  Class *const inst,
  String *const method_identifier
) {
  if (!inst || !method_identifier) {
    return nll;
  }

  refeach (Method, method, inst->methods, {
    if (Equals(String, method_identifier, Getter(Method, Identifier, method))) {
      return method;
    }
  })

  return nll;
}

void _Class_EraseFields(Class *const inst)
{
  if (!inst) {
    return;
  }

  erase(Array(Field), inst->fields);
}

void _Class_EraseMethods(Class *const inst)
{
  if (!inst) {
    return;
  }

  erase(Array(Method), inst->methods);
}

IMPL_ARRAY(Class)
IMPL_ARRAY_LITERALISE_ARGS(
  Class,
  need_member_definition,
  boolean need_member_definition
)
