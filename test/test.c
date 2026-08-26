#include "../inc/body.h"
#include "../inc/class.h"
#include "../inc/constructor.h"
#include "../inc/destructor.h"
#include "../inc/entry.h"
#include "../inc/field.h"
#include "../inc/preprocessor.h"

// to test:
// String *elem_type##Array_Literalise(
//   Array(elem_type) *const inst,
//   String *const prefix,
//   String *const separator,
//   String *const suffix
// )

/* This header includes everything generated.
 * Before the generation, it is suppose to be empty, making no difference.
 */
#include "../user/header.h"

typedef Class Command;
ARRAY(Command)
LITERALISE_ARGS(Command, boolean want_fancy, boolean need_member_definition)
ALIAS_ARRAY_CLASS(Command)

int Main(void)
{
  Class *const c_Command = Create(
    Class,
    ACCESS_PUBLIC,
    string("Command"),
    null,
    Compose(
      Array(Field),
      field(Command, private, String *, identifier, null),
      field(Command, private, Array(Command) *, subcommands, null)
    ),
    Compose(
      Array(Method),
      method(Command, public, String *, GetIdentifier, {
        return this->identifer;
      }, null),
      method(Command, public, Array(Command) *, GetSubcommands, {
        return this->subcommands;
      }, null)
    ),
    constructor(Command, public, {
      if (!identifier) {
        return null;
      }

      this->identifier = identifier;
      this->subcommands = subcommands;

      return this;
    }, param(String *const, identifier),
       param(Array(Command) *const, subcommands)
    ),
    destructor(Command, {
      if (!this) {
        return;
      }

      Delete(String, this->identifier);
      Delete(Array(Command), this->subcommands);
    })
  );

  Array(Command) *const commands = array(Command, 10);
  refrefeach (Command, cmd, commands, {
    *cmd = Create(
      Class, ACCESS_PUBLIC, string("Command"), null,
      Compose(
        Array(Field),
        Create(
          Field,
          ACCESS_PUBLIC,
          Create(Signature, string("register llong"), string("counter"), null),
          string("0")
        ),
        Create(
          Field,
          ACCESS_PUBLIC,
          Create(Signature, string("Class *"), string("c_Cat"), null),
          string(nameof(
            Create(Class, ACCESS_PUBLIC, "Cat", null, null, null, null, null)
          ))
        )
      ),
      Compose(
        Array(Method),
        Create(
          Method,
          ACCESS_PUBLIC,
          Create(
            Function,
            Create(Signature, null, null, null),
            Create(Body, null, string(""))
          )
        )
      ),
      Create(Constructor, null, null),
      Create(Destructor, null, null)
    );
  })

  puts(flatten(char, lit(Array(Command), commands, null, string(NEWLINE), null, no, yes)));

  // puts(flatten(char, lit(Class, c_Command, no, no)));
  // puts("");
  // puts("");
  // puts("");
  // puts(flatten(char, lit(Class, c_Command, no, yes)));

  Delete(Class, c_Command);

  return 0;
}
