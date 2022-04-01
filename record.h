#ifndef RECORD_H
#define RECORD_H

int record_open(const char *filename, const char *mode);
int record_add(const char);
int record_close(void);
char record_get(void);

#endif
