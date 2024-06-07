#ifndef COMPOUND_CATLOG_H
# define COMPOUND_CATLOG_H

# include <Compound/common.h>
# include <Compound/status.h>

typedef enum {
	CATLOG_LEVEL_ALL,  // Least the value, most the information.
	CATLOG_LEVEL_MINOR,
	CATLOG_LEVEL_NORMAL,
	CATLOG_LEVEL_MAJOR,
	CATLOG_LEVEL_CRITICAL,
	CATLOG_LEVEL_FATAL,
	CATLOG_LEVEL_DEBUG,
	CATLOG_LEVEL_NONE
} CatlogLevel;

DEFSTATUS(CatCannotSendMessage, 1,
  "Cat had trouble while sending your message.",
  STATUS_ERROR, &ReadWriteError);

DEFSTATUS(CatMessageTooLong, 1,
  "Cat cannot finish reading your loooong message before deadline.",
  STATUS_ERROR, &MaximumLengthExceeded);

DEFSTATUS(CatCannotFindFile, 1,
  "Cat cannot have your file found.",
  STATUS_ERROR, &FileNotFound);

DEFSTATUS(CatHasInsufficientPermission, 1,
  "Cat cannot access your file with its given permission.",
  STATUS_ERROR, &InsufficientAccessPermission);

DEFSTATUS(CatCannotOpenFile, 1,
  "Cat cannot open your file because fopen returned NULL.",
  STATUS_ERROR, &ReadWriteError);

typedef struct {
	time_t time;
	CatlogLevel level;
	char *initiator;
	char *content;
} CatlogMsg;

Status CatlogMsg_Create(CatlogMsg *inst, CatlogLevel level,
                        char *initiator, char *msg);
Status CatlogMsg_CopyOf(CatlogMsg *inst, CatlogMsg *other);
bool   CatlogMsg_Equals(CatlogMsg *inst, CatlogMsg *other);

typedef struct {
	CatlogMsg msg;
	FILE *dst;
	bool successful;
	struct timespec elapsed;
} CatlogSender;

Status CatlogSender_Create(CatlogSender *inst, CatlogMsg *msg, FILE *dst);
Status CatlogSender_CopyOf(CatlogSender *inst, CatlogSender *other);
Status CatlogSender_Send(CatlogSender *inst);
bool   CatlogSender_Equals(CatlogSender *inst, CatlogSender *other);
Status CatlogUtils_OpenFile(FILE **fileptr, const char *filepath,
                            const char const *restrict mode);
Status CatlogUtils_CloseFile(FILE **fileptr);

#endif  /* COMPOUND_CATLOG_H */
