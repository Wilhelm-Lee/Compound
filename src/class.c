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
};

Class *Class_Create(
  const Access access,
  String *const identifier,
  Class *const super,
  Array(Field) *const fields,
  Array(Method) *const methods,
  Constructor *const constructor,
  Destructor *const destructor
) {
  if (!identifier || blank(identifier)) {
    return null;
  }

  Class *const inst = Allocate(1, sizeof(Class));
  if (!inst) {
    return null;
  }

  inst->access = access;
  inst->identifier = identifier;
  inst->super = super;
  inst->this = inst;
  inst->methods = methods;
  inst->fields = fields;
  inst->constructor = constructor;
  inst->destructor = destructor;

  return inst;
}

Class *Class_CopyOf(Class *const other)
{
  if (!other) {
    return NULL;
  }

  Class *const inst = Create(
    Class,
    other->access,
    other->identifier,
    other->super,
    other->fields,
    other->methods,
    other->constructor,
    other->destructor
  );
  if (!inst) {
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
  inst->identifier = concat(inst->identifier, string(" copy"));

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

String *Class_Literalise(
  Class *const inst,
  boolean want_fancy,
  boolean need_member_definition
) {
  if (!inst) {
    return null;
  }

  String *const str_NEWLINE = string(NEWLINE);
  String *const str_indent = string("  ");
  String *const str_comma_space = string(", ");

  String *lit = null;

  if (want_fancy) {
    return string("fancy");
  } else {
    lit = append(
      string("Class *"),
      inst->identifier,
      string(" = class("),
      lit(Access, inst->access),
      str_comma_space,
      inst->identifier,
      str_comma_space,
      string("{"),
      append(
        lit(Array(Field), inst->fields, null, null, str_NEWLINE, need_member_definition, yes),
        lit(Array(Method), inst->methods, null, str_NEWLINE, null, need_member_definition),
        lit(Constructor, inst->constructor, need_member_definition),
        lit(Destructor, inst->destructor, need_member_definition)
      ),
      string("});")
    );
  }

  // String *ret = append(lit(Access, inst->access), string(" "),inst->identifier);

  // if (inst->super) {
  //   ret = append(string(" : "), inst->super->identifier);
  // }


  // ret = append(
  //   ret,
  //   string(" {"NEWLINE),
  //   str_indent, lit(Array(Field), inst->fields, null, null, null), str_NEWLINE,
  //   str_indent, lit(Array(Method), inst->methods, null, null, null),str_NEWLINE,
  //   str_indent, lit(Constructor, inst->constructor), str_NEWLINE,
  //   str_indent, lit(Destructor, inst->destructor), str_NEWLINE,
  //   string("}")
  // );

  Delete(String, str_comma_space);
  Delete(String, str_indent);
  Delete(String, str_NEWLINE);

  return lit;
}

String *_GenerateYearString(void)
{
  time_t timestamp = time(NULL);
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
    flatten(char, lit(Array(Method), inst->methods, null, null, null, no)),
    flatten(char, lit(Constructor, inst->constructor, no)),
    flatten(char, lit(Destructor, inst->destructor, no)),
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

  return concat(
    _GenerateLicenseBanner(),
    lit(Array(Method), inst->methods, null, string(NEWLINE), null, yes)
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
      char, concat(_GenerateLicenseBanner(), _GenerateHeaderContent(inst))
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
      char, concat(_GenerateLicenseBanner(), _GenerateSourceContent(inst))
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

IMPL_ARRAY(Class)
IMPL_ARRAY_LITERALISE_CONFIGS(
  Class,
  want_fancy,
  need_member_definition,
  boolean want_fancy,
  boolean need_member_definition
)
