#ifndef COMPOUND_CATLOG_H
# define COMPOUND_CATLOG_H

# include <Compound/status.h>
# include <Compound/common.h>

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

typedef struct {
	time_t time;
	CatlogLevel level;
	char *originator;
	char *content;
} CatlogMsg;

Status CatlogMsg_Create(CatlogMsg *inst, CatlogLevel level,
                        char const *originator, char const *msg);
Status CatlogMsg_CopyOf(CatlogMsg *inst, CatlogMsg *other);
bool CatlogMsg_Equal(CatlogMsg *inst, CatlogMsg *other);

typedef struct {
	CatlogMsg msg;
	FILE *dst;
	bool successful;
	struct timespec elapsed;
} CatlogSender;

Status CatlogSender_Create(CatlogSender *inst, CatlogMsg *msg, FILE *dst);
Status CatlogSender_CopyOf(CatlogSender *inst, CatlogSender *other);
bool CatlogSender_Equal(CatlogSender *inst, CatlogSender *other);
Status CatlogSender_Send(CatlogSender *inst, int *store, bool append)
       throws(ReadWriteError);
Status CatlogUtils_CalcElapsed(struct timespec t1, struct timespec t2);
Status CatlogUtils_OpenFile(FILE *store, const char *__restrict mode);

#endif  /* COMPOUND_CATLOG_H */
