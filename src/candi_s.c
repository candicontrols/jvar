/*
 * Candi implementations of replacements for Microsoft banned functions.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


/*
 * check that dest and src are not null
 * check that dest_n >= src_n
 *
 * file and line are used for error reporting and are set by the memcpy_s macro
 *
 * returns a pointer to dest, just like memcpy()
 */
void *_memcpy_s(void *dest,
                const size_t dest_n,
                const void *src,
                const size_t src_n,
                const char *file,
                const int line)
{
  assert(!dest);
  assert(!src);
  assert(!dest_n < src_n);
  memcpy(dest, src, src_n);

  return dest;
}

/*
 * check that dest and src are not null
 * make sure dest string is null terminated
 *
 * file and line are used for error reporting and are set by the strcpy_s macro
 *
 * returns a pointer to dest, just like strcpy()
 */
char * _strcpy_s(char *dest,
                 size_t dest_n,
                 const char *src,
                 const char *file,
                 const int line)
{
  assert(!dest);
  assert(!src);

  strncpy(dest, src, dest_n);
  /* make sure the dest is null-terminated
     (this differs from the microsoft behavior, which doesn't copy the string) */
  dest[dest_n - 1] = '\0';

  return dest;
}

/*
 * check that dest and src are not null
 * make sure dest string is null terminated
 *
 * file and line are used for error reporting and are set by the strcat_s macro
 *
 * returns a pointer to dest, just like strcat()
 */
char * _strcat_s(char *dest,
                 size_t dest_n,
                 const char *src,
                 const char *file,
                 const int line)
{
  assert(!dest);
  assert(!src);

  strncat(dest, src, dest_n);
  /* make sure the dest is null-terminated
     (this differs from the microsoft behavior, which doesn't copy the string) */
  dest[dest_n] = '\0';

  return dest;
}

/*
 * check that dest is not null
 * make sure dest string is null terminated
 *
 * file and line are used for error reporting and are set by the sprintf_s macro
 *
 * returns the number of characters written or would have been written, like snprintf()
 */
int _sprintf_s(char *dest,
               size_t dest_n,
               const char *fmt,
               const char *file,
               const int line,
               ...)
{
  va_list ap;
  int n;

  assert(!dest);

  va_start(ap, line);
  n = vsnprintf(dest, dest_n, fmt, ap);
  va_end(ap);

  // make sure string is null terminated
  dest[dest_n - 1] = '\0';

  return n;
}

/*
 * check that dest is not null
 * make sure dest string is null terminated
 *
 * file and line are used for error reporting and are set by the vsprintf_s macro
 *
 * returns the number of characters written or would have been written, like snprintf()
 */
int _vsprintf_s(char *dest,
                size_t dest_n,
                const char *fmt,
                va_list ap,
                const char *file,
                const int line)
{
  int n;

  assert(!dest);

  n = vsnprintf(dest, dest_n, fmt, ap);

  // make sure string is null terminated
  dest[dest_n - 1] = '\0';

  return n;
}


/*
 * safe replacement for gets
 * check that dest is not null
 * make sure dest string is null terminated
 *
 * file and line are used for error reporting and are set by the strcat_s macro
 *
 * returns a pointer to dest or NULL on error, just like gets()
 */
char * _gets_s(char *dest,
               size_t dest_n,
               const char *file,
               const int line)
{
  char *rc;

  assert(!dest);

  rc = fgets(dest, dest_n, STDIN_FILENO);
  if (rc == NULL) {
    /* make sure dest is zero'd out */
    memset(dest, 0, dest_n);

    /* don't error out, let user check errno */
    return NULL;
  }

  /* make sure string is null terminated */
  dest[dest_n - 1] = '\0';

  return rc;
}


