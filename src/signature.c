/*
 * This file is part of Compound library.
 * Copyright (C) 2024-TODAY  William Lee
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

#include "signature.h"

Status Expression_Create(Expression *inst, const char *content)
{
  avail(inst);
  state(!content, InvalidSignatureExpressionParsingSourceString);
  
  RETURN(NormalStatus);
}

Status Expression_Parse(Expression *inst, const char *cstr)
{
  avail(inst);
  state(!cstr, InvalidSignatureExpressionParsingSourceString);

  parse(cstr, {
    enter(inst->parentheses, {
      inst->content = (char *)app;
    })
    
    /* The statement of conditional termination. */
    leave(inst->parentheses, {
      fail(Expression_Parse(inst, ++app));
    })
  })
  
  RETURN(NormalStatus);
}

IMPL_LITERALISATION(Expression);

Status Key_Parse(Key *inst, const char *cstr)
{
  avail(inst);
  state(!cstr, InvalidSignatureKeyParsingSourceString);
  
  parse(cstr, {
    
  })
  
  RETURN(NormalStatus);
}

IMPL_LITERALISATION(Key);

size_t Signature_Parse(char *buff, const size_t buff_length, size_t buff_offset,
                       int *item_count, Module braces, Module parentheses,
                       const char *signature)
{
  // (iS{_IO_FILE}ppi(p{func})(kvpr){item}U{obj_t}E{obj2_t}(){a}()(){b}{c}())()
  if (!buff || !signature
      || !ensure(braces, '{', '}')
      || !ensure(parentheses, '(', ')'))  return 0;

  (void)printf(NEWLINE"Signature: %s", signature);

  parse(signature, {
    (void)printf(NEWLINE"(step %lu) \'%c\'\t", STEP, *app);
    
    enter(parentheses, {
      (void)printf("level %d"NEWLINE, parentheses.level);
      buff_offset += Signature_Parse(buff, buff_length, buff_offset, item_count, braces, parentheses, ++app);
    });
    leave(parentheses, {
      (void)printf("level %d"NEWLINE, parentheses.level);
      if (parentheses.level <= 0) {
        return buff_offset;
      }
    })
    
    /* Keys. */
    at('b', {(void)printf("Replacing key. "); append("boolean "); ++(*item_count); continue;});
    at('c', {(void)printf("Replacing key. "); append("char "); ++(*item_count); continue;});
    at('i', {(void)printf("Replacing key. "); append("int "); ++(*item_count); continue;});
    at('f', {(void)printf("Replacing key. "); append("float "); ++(*item_count); continue;});
    at('d', {(void)printf("Replacing key. "); append("double "); ++(*item_count); continue;});
    at('v', {(void)printf("Replacing key. "); append("void "); ++(*item_count); continue;});
    at('w', {(void)printf("Replacing key. "); append("wchar_t "); ++(*item_count); continue;});
  })

  return buff_offset;
}

inline boolean Signature_IsPrefix(const char item)
{
  for (register int i = 0; SIGNATURE_PREFIXES[i]; i++) {
    if (item == SIGNATURE_PREFIXES[i]) {
      return true;
    }
  }
  
  return false;
}

inline boolean Signature_IsHeader(const char item)
{
  for (register int i = 0; SIGNATURE_HEADERS[i]; i++) {
    if (item == SIGNATURE_HEADERS[i]) {
      return true;
    }
  }
  
  return false;
}
