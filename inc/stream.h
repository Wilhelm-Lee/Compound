#ifndef COMPOUND_STREAM_H
# define COMPOUND_STREAM_H

# include "Compound/array.h"
# include "Compound/arrays.h"
# include "Compound/common.h"
# include "Compound/platform.h"
# include "Compound/status.h"
# include "Compound/string.h"

# define Stream(type)  type##Stream

# define DEFINE_STREAM(type)\
  typedef struct {                                         \
    type source;                                           \
    String buffer;                                         \
    boolean opened;                                           \
  } Stream(type);

# define FUNC_STREAM(type)\
  Status type##Stream_Create(Stream(type) *inst, const type source);\
  Status type##Stream_CopyOf(Stream(type) *inst, const Stream(type) other);\
  Status type##Stream_Delete(Stream(type) *inst);          \
  Status type##Stream_Open(Stream(type) *inst);            \
  Status type##Stream_Close(Stream(type) *inst);           \
  Status type##Stream_Read(Stream(type) *inst, const size_t maxbyte, String *buff);\
  Status type##Stream_Write(Stream(type) *inst, const size_t maxbyte, String *buff);\
  Status type##Stream_Flush(Stream(type) *inst);

# define GLOBAL_STREAM(type)                               \
  static type##Stream *Global##type##Stream = NULL;        \
  static inline void type##Stream_SetGlobal(type##Stream *stream)\
  {                                                        \
    Global##type##Stream = stream;                         \
  };

# define STREAM(type)                                      \
  FUNC_STREAM(type)                                        \
  GLOBAL_STREAM(type)

# define opened(stream)                                    \
  ((stream).opened)



#endif  /* COMPOUND_STREAM_H */
