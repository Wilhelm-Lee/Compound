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
  Access access;
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

Class *Class_Create(
  const Access access,
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

  Class *const inst = Allocate(1, sizeof(Class));
  if (!inst) {
    return null;
  }

  String *const CLASS_IDENTIFIER_STR = CopyOf(String, identifier);

  inst->access = access;
  inst->identifier = identifier;
  inst->super = super;
  inst->this = inst;
  inst->fields = fields ? fields : array(Field, 0);
  inst->methods = methods ? methods : array(Method, 0);
  inst->constructor = constructor;
  inst->destructor = destructor;
  inst->Equals = Equals ? Equals : Create(
    Method,
    ACCESS_PUBLIC,
    inst->identifier,
    Create(
      Function,
      Create(
        Signature,
        string("boolean"),
        string("Equals"),
        params_str(
          param_str(append(inst->identifier, string(" *")), string("this")),
          params_str(append(inst->identifier, string(" *")), string("other"))
        )
      ),
      Create(
        Body,
        null,
        append(string("return Equals("), inst->identifier, string(", this, other);"))
      )
    )
  );
  inst->Literalise = Literalise ? Literalise : Create(
    Method,
    ACCESS_PUBLIC,
    inst->identifier,
    Create(
      Function,
      Create(
        Signature,
        string("boolean"),
        string("Literalise"),
        params_str(
          param_str(append(inst->identifier, string(" *")), string("this"))
        )
      ),
      Create(
        Body,
        null,
        append(string("return lit("), inst->identifier, string(", this);"))
      )
    )
  );

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
    other->access,
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

boolean Class_Equals(const Class *const obj1, const Class *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return
    Equals(String, obj1->identifier, obj2->identifier) &&
    obj1->super == obj2->super &&
    Equals(Array(Method), obj1->methods, obj2->methods, Method_Equals);
}

String *_Class_GenerateTypedef(Class *const inst)
{
  if (!inst) {
    return nll;
  }

  return append(
    string("typedef struct "),
    inst->identifier,
    string(" "),
    inst->identifier,
    string(";"),
    string(NL)
  );
}

String *_Class_GenerateArrayDeclarations(Class *const inst)
{
  return append(string("ARRAY("), inst->identifier, string(")"NL));
}

String *_Class_GenerateStruct(Class *const inst)
{
  String *const indent = string("  ");
  String *const newline = string(NL);

  String *lit = append(
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

String *_Class_GenerateObjectEssentialDeclarations(Class *const inst)
{
  String *const indent = string("  ");
  String *const newline = string(NL);

  String *lit = append(
    inst->identifier, string(" *"), inst->identifier, string("_Create"), lit(Constructor, inst->constructor, no, no, yes, yes, yes, no, no), string(";"NL),
    inst->identifier, string(" *"), inst->identifier, string("_CopyOf("), inst->identifier, string(" *const other);"NL),
    string("void "), inst->identifier, string("_Delete("), inst->identifier, string(" *const this);"NL),
    string("boolean "), inst->identifier, string("_Equals("), inst->identifier, string(" *const obj1, "), inst->identifier, string(" *const obj2);"NL),
    string("String *"), inst->identifier, string("_Literalise("), inst->identifier, string(" *const inst);"NL),
    newline
  );

  Delete(String, newline);
  Delete(String, indent);

  return lit;
}

String *_Class_GenerateObjectEssentialImplementations(Class *const inst)
{
  String *const indent = string("  ");
  String *const newline = string(NL);

  /* Extract constructor parameters safely */
  Array(Parameter) *constructor_params = Getter(
    Signature, Parameters,
    Getter(
      Function, Signature,
      Getter(
        Method, Function,
        Getter(Constructor, Method, inst->constructor)
      )
    )
  );

  String *lit = append(
    inst->identifier, string(" *"), inst->identifier, string("_Create"), lit(Constructor, inst->constructor, no, no, yes, yes, yes, no, no), newline,
    string("{"NL),
    string("  "), inst->identifier, string(" *const this = Allocate(1, sizeof("), inst->identifier, string("));"NL),
    string("  if (!this) {"NL),
    string("    return nll;"NL),
    string("  }"NL),
    newline,
    string("  "), lit(Constructor, inst->constructor, no, no, no, no, no, yes, no), newline,
    string("}"NL),
    newline,
    string(""), inst->identifier, string(" *"), inst->identifier, string("_CopyOf("), inst->identifier, string(" *const other)"NL),
    string("{"NL),
    string("  if (!other) {"NL),
    string("    return nll;"NL),
    string("  }"NL),
    newline,
    string("  return Create("), inst->identifier, string(", "), lit(Array(Parameter), constructor_params, string("other->"), string(", other->"), nll, no, yes), string(");"NL),
    string("}"NL),
    newline,
    string("void "), inst->identifier, string("_Delete("), inst->identifier, string(" *const this)"NL),
    string("{"NL),
    string("  if (!this) {"NL),
    string("    return;"NL),
    string("  }"NL),
    string("  "), lit(Destructor, inst->destructor, no, no, no, no, no, yes, no), newline,
    newline,
    string("  Deallocate(this);"NL),
    string("}"NL),
    newline,
    string("boolean "), inst->identifier, string("_Equals("), inst->identifier, string(" *const this, "), inst->identifier, string(" *const other)"NL),
    string("{"NL),
    string("  if (!this || !other) {"NL),
    string("    return false;"NL),
    string("  }"NL),
    newline,
    string("  if (this == other) {"NL),
    string("    return true;"NL),
    string("  }"NL),
    string(""NL),
    string("  "), lit(Method, inst->Equals, no, no, no, no, no, yes, no), newline,
    string("}"NL),
    newline,
    string("String *"), inst->identifier, string("_Literalise("), inst->identifier, string(" *const this)"NL),
    string("{"NL),
    string("  if (!this) {"NL),
    string("    return nll;"NL),
    string("  }"NL),
    string(""NL),
    string("  "), lit(Method, inst->Literalise, no, no, no, no, no, yes, no), newline,
    string("}"NL),
    newline
  );

  Delete(String, newline);
  Delete(String, indent);

  return lit;
}

static inline String *_Class_GenerateArrayImplementations(Class *const inst)
{
  return append(string("IMPL_ARRAY("), inst->identifier, string(")"), string(NL));
}

static inline String *_Class_GenerateMethodDeclarations(Class *const inst)
{
  return lit(Array(Method), inst->methods, nll, string(NL), nll, yes, yes, yes, yes, yes, no, yes);
}

static inline String *_Class_GenerateMethodImplementations(Class *const inst)
{
  return lit(Array(Method), inst->methods, nll, string(NL), nll, yes, yes, yes, yes, yes, yes, no);
}

String *Class_Literalise(
  Class *const inst,
  boolean want_fancy,
  boolean need_member_definition
) {
  if (!inst) {
    return null;
  }

  String *const str_indent = string("  ");
  String *const str_comma_space = string(", ");
  String *const str_semicolon = string(";");

  String *lit = null;

  if (want_fancy) {
    return string("fancy");
  }

  lit = append(
    _Class_GenerateTypedef(inst),
    _Class_GenerateArrayDeclarations(inst),
    _Class_GenerateStruct(inst),
    _Class_GenerateObjectEssentialDeclarations(inst),
    _Class_GenerateMethodDeclarations(inst)
  );

  if (need_member_definition) {
    lit = append(
      lit,
      _Class_GenerateObjectEssentialImplementations(inst),
      _Class_GenerateArrayImplementations(inst),
      _Class_GenerateMethodImplementations(inst)
    );
  }

  Delete(String, str_semicolon);
  Delete(String, str_comma_space);
  Delete(String, str_indent);

  return lit;
}

String *_GenerateYearString(void)
{
  time_t timestamp = time(null);
  struct tm *timer = gmtime(&timestamp);
  char year[5];
  strftime(year, sizeof(year), "%Y", timer);

  return string(year);
}

String *_GenerateLicenseBanner(void)
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
    flatten(char, _GenerateYearString())
  );
}

String *_GenerateHeaderContent(Class *const inst)
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
    ""NL
    "  %s/* Methods (signature). */"NL
    "  %s/* Constructor (signature). */"NL
    "  %s/* Destructor (signature). */"NL
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

String *_GenerateSourceContent(Class *const inst)
{
  if (!inst) {
    return null;
  }

  return Concat(String,
    _GenerateLicenseBanner(),
    lit(Array(Method), inst->methods, null, string(NEWLINE), null, yes, yes, yes, yes, yes, yes, no)
  );
}

boolean _Class_RecreateHeader(FILE *const header, Class *const inst)
{
  if (!header || !inst) {
    return false;
  }

  return fprintf(
    header,
    "%s",
    flatten(
      char, Concat(String, _GenerateLicenseBanner(), _GenerateHeaderContent(inst))
    )
  );
}

boolean _Class_RecreateSource(FILE *const header, Class *const inst)
{
  if (!header || !inst) {
    return false;
  }

  return fprintf(
    header,
    "%s",
    flatten(
      char, Concat(String, _GenerateLicenseBanner(), _GenerateSourceContent(inst))
    )
  );
}

boolean Class_Recreate(
  FILE *const header,
  FILE *const source,
  Class *const inst
) {
  if (!inst || !header || !source) {
    return false;
  }

  return _Class_RecreateHeader(header, inst) &&
         _Class_RecreateSource(source, inst);
}

Class *Class_AddField(Class *const inst, Field *const field)
{
  if (!inst || !field) {
    return inst;
  }

  inst->fields = call(Array(Field), Insert, inst->fields, -1, field);
  _Field_SetNumericalIdentifier(
    ref(Array(Field), inst->fields, -1),
    Length(Array(Field), inst->fields) - 1
  );

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
    ret;
  }

  inst->super = super;
  inst->fields = Append(Array(Field), inst->fields, super->fields);
  inst->methods = Append(Array(Method), inst->methods, super->methods);
  fout(stderr, lit(Array(Method), inst->methods, nll, string(NL), string(NL), yes, yes, yes, yes, yes, yes, yes));
  call(Constructor, Inherit, inst->constructor, super->constructor);
  call(Destructor, Inherit, inst->destructor, super->destructor);
}

inline void Class_SetConstructor(Class *const inst, Constructor *const constructor)
{
  if (!inst) {
    return;
  }

  Delete(Constructor, inst->constructor);

  inst->constructor = constructor;
}

inline void Class_SetDestructor(Class *const inst, Destructor *const destructor)
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
  if (!inst) {
    return nll;
  }

  refeach (Method, field, inst->fields, {
    if (Equals(String, method_identifier, Getter(Method, Identifier, field))) {
      return field;
    }
  })

  return nll;
}

IMPL_ARRAY(Class)
IMPL_ARRAY_LITERALISE_CONFIGS(
  Class,
  want_fancy,
  need_member_definition,
  boolean want_fancy,
  boolean need_member_definition
)
