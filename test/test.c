#include "../inc/body.h"
#include "../inc/class.h"
#include "../inc/constructor.h"
#include "../inc/destructor.h"
#include "../inc/entry.h"
#include "../inc/field.h"
#include "../inc/preprocessor.h"

// to test:

/* This header includes everything generated.
 * Before the generation, it is suppose to be empty, making no difference.
 */
#include "../user/header.h"

int Main(void)
{
  Class *const c_Command = class(public, Command, {
    field(private, String *, identifier, null);
    field(private, Array(Command) *, subcommands, null);

    constructor(public, {
      if (!identifier) {
        return null;
      }

      this->identifier = identifier;
      this->subcommands = subcommands;

      return this;
    }, param(String *const, identifier),
       param(Array(Command) *const, subcommands)
    );

    destructor(private, {
      if (!this) {
        return;
      }

      Delete(String, this->identifier);
      Delete(Array(Command), this->subcommands);
    });

    method(public, String *, GetIdentifier, {
      return this->identifer;
    }, param(void));

    method(public, Array(Command) *, GetSubcommands, {
      return this->subcommands;
    }, param(void));
  });

  // Command *cmd = new(Command, ...);

  Delete(Class, c_Command);

  return 0;
}
