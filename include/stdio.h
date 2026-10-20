#ifndef _NANOC_STDIO_H
#define _NANOC_STDIO_H

#include <stdarg.h>

typedef unsigned long size_t;
typedef long fpos_t;
typedef struct __nanoc_FILE FILE;

#define EOF (-1)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define BUFSIZ 8192
#define FILENAME_MAX 4096
#define L_tmpnam 20
#define TMP_MAX 238328
#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int remove(const char *filename);
int rename(const char *oldname, const char *newname);
FILE *tmpfile(void);
char *tmpnam(char *name);

int fclose(FILE *stream);
int fflush(FILE *stream);
FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
void setbuf(FILE *stream, char *buffer);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);

int fprintf(FILE *stream, const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int printf(const char *format, ...);
int scanf(const char *format, ...);
int snprintf(char *buffer, size_t size, const char *format, ...);
int sprintf(char *buffer, const char *format, ...);
int sscanf(const char *buffer, const char *format, ...);
int vfprintf(FILE *stream, const char *format, va_list args);
int vfscanf(FILE *stream, const char *format, va_list args);
int vprintf(const char *format, va_list args);
int vscanf(const char *format, va_list args);
int vsnprintf(char *buffer, size_t size, const char *format, va_list args);
int vsprintf(char *buffer, const char *format, va_list args);

int fgetc(FILE *stream);
char *fgets(char *buffer, int size, FILE *stream);
int fputc(int ch, FILE *stream);
int fputs(const char *text, FILE *stream);
int getc(FILE *stream);
int getchar(void);
char *gets(char *buffer);
int putc(int ch, FILE *stream);
int putchar(int ch);
int puts(const char *text);
int ungetc(int ch, FILE *stream);

size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream);

int fgetpos(FILE *stream, fpos_t *position);
int fseek(FILE *stream, long offset, int origin);
int fsetpos(FILE *stream, const fpos_t *position);
long ftell(FILE *stream);
void rewind(FILE *stream);

void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
void perror(const char *message);

int fileno(FILE *stream);
FILE *fdopen(int fd, const char *mode);
FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);
int getdelim(char **line, size_t *capacity, int delimiter, FILE *stream);
int getline(char **line, size_t *capacity, FILE *stream);
int asprintf(char **buffer, const char *format, ...);
int vasprintf(char **buffer, const char *format, va_list args);
int dprintf(int fd, const char *format, ...);
int vdprintf(int fd, const char *format, va_list args);

#endif
