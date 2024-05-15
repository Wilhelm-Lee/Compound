#ifndef COMPOUND_UTILS_H
# define COMPOUND_UTILS_H

# include <time.h>
# include <string.h>
# include <stdlib.h>

# include <Compound/common.h>
# include <Compound/const.h>

# define DATETIME_FORMAT "%a %d %b %X %Z %Y"

int Utils_CalcDigits(long long n);
int Utils_CalcDateTimeLiteralisationLength(char *buff);

#endif  /* COMPOUND_UTILS_H */
