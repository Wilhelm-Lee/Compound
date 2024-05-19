#ifndef COMPOUND_STRING_H
# define COMPOUND_STRING_H

# include <complex.h>
# include <wchar.h>

# include <Compound/char.h>
# include <Compound/array.h>

typedef enum {
  ASCII,
  UTF8,
  /* etc. */
} StringEncoding;

typedef Array(Char) String;

/* Elementary. */
Status String_Create(String *inst, int len);
Status String_CopyOf(String *inst, String *other);
Status String_Delete(String *inst);
Status String_GetIdx(String *inst, Char *item, int index);
Status String_SetIdx(String *inst, Char *item, int index);
Status String_Literalise(String *inst, String *store);
bool   String_Equals(String *arr1, String *arr2);

/* Extensional. */
Status StringUtils_FromInteger(String *inst, int value);
Status StringUtils_FromShortInteger(String *inst, short int value);
Status StringUtils_FromLongInteger(String *inst, long int value);
Status StringUtils_FromLongLongInteger(String *inst, long long int value);
Status StringUtils_FromFloat(String *inst, float value);
Status StringUtils_FromDouble(String *inst, double value);
Status StringUtils_FromLongDouble(String *inst, long double value);
Status StringUtils_FromComplexInteger(String *inst, _Complex int value);
Status StringUtils_FromComplexShortInteger(String *inst, _Complex short int value);
Status StringUtils_FromComplexLongInteger(String *inst, _Complex long int value);
Status StringUtils_FromComplexLongLongInteger(String *inst, _Complex long long value);
Status StringUtils_FromComplexFloat(String *inst, _Complex float value);
Status StringUtils_FromComplexDouble(String *inst, _Complex double value);
Status StringUtils_FromComplexLongDouble(String *inst, _Complex long double value);
Status StringUtils_FromUnsignedInteger(String *inst, unsigned int value);
Status StringUtils_FromUnsignedShortInteger(String *inst, unsigned short int value);
Status StringUtils_FromUnsignedLongInteger(String *inst, unsigned long int value);
Status StringUtils_FromUnsignedLongLongInteger(String *inst, unsigned long long int value);
Status StringUtils_FromUnsignedComplexInteger(String *inst, unsigned _Complex int value);
Status StringUtils_FromUnsignedComplexShortInteger(String *inst, unsigned _Complex short int value);
Status StringUtils_FromUnsignedComplexLongInteger(String *inst, unsigned _Complex long int value);
Status StringUtils_FromUnsignedComplexLongLongInteger(String *inst, unsigned _Complex long long value);
Status StringUtils_FromAddress(String *inst, void *store);
Status StringUtils_FromCharBuff(String *inst, char const *buff, int base);
Status StringUtils_FromWideCharBuff(String *inst, wchar_t const *wbuff, int base);
Status StringUtils_ToInteger(String *inst, int *store);
Status StringUtils_ToShortInteger(String *inst, short int *store);
Status StringUtils_ToLongInteger(String *inst, long int *store);
Status StringUtils_ToLongLongInteger(String *inst, long long int *store);
Status StringUtils_ToFloat(String *inst, float *store);
Status StringUtils_ToDouble(String *inst, double *store);
Status StringUtils_ToLongDouble(String *inst, long double *store);
Status StringUtils_ToComplexInteger(String *inst, _Complex int *store);
Status StringUtils_ToComplexShortInteger(String *inst, _Complex short int *store);
Status StringUtils_ToComplexLongInteger(String *inst, _Complex long int *store);
Status StringUtils_ToComplexLongLongInteger(String *inst, _Complex long long *store);
Status StringUtils_ToComplexFloat(String *inst, _Complex float *store);
Status StringUtils_ToComplexDouble(String *inst, _Complex double *store);
Status StringUtils_ToUnsignedInteger(String *inst, unsigned int *store);
Status StringUtils_ToUnsignedShortInteger(String *inst, unsigned short int *store);
Status StringUtils_ToUnsignedLongInteger(String *inst, unsigned long int *store);
Status StringUtils_ToUnsignedLongLongInteger(String *inst, unsigned long long int *store);
Status StringUtils_ToUnsignedComplexInteger(String *inst, unsigned _Complex int *store);
Status StringUtils_ToUnsignedComplexShortInteger(String *inst, unsigned _Complex short int *store);
Status StringUtils_ToUnsignedComplexLongInteger(String *inst, unsigned _Complex long int *store);
Status StringUtils_ToUnsignedComplexLongLongInteger(String *inst, unsigned _Complex long long *store);
Status StringUtils_ToAddress(String *inst, void **store);
Status StringUtils_ToCharBuff(String *inst, char const *buff, int base);
Status StringUtils_ToWideCharBuff(String *inst, wchar_t const *wbuff, int base);
Status StringUtils_Format(String *inst, const String *__restrict fmt, ...);
Status StringUtils_Tokenise(String *inst, const String *delim, String *store);
Status String_Encode(String *inst, StringEncoding encoding)
       throws(UnsupportedEncoding  EncodingError  DecodingError);
Status String_Decode(String *inst, StringEncoding encoding);
       throws(UnsupportedEncoding  EncodingError  DecodingError);
int    StringUtils_Compare(String *a, String *b);

static Status StringConversionPrecisionError = {
  .description = "Unpreventable precision loss was found during conversion "
                 "between string and raw data.",
  .characteristic = STATUS_ERROR,
  .prev = &ImprecisionError
};

typedef Array(int)              InfiniteInteger;
typedef Array(double)           InfiniteFloatingPoint;
typedef Array(_Complex double)  InfintieComplex;


#endif  /* COMPOUND_STRING_H */
