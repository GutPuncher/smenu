/* **************************** */
/* Memory management functions. */
/* **************************** */

/* Created by Kevin Locke (from numerous canonical examples).        */
/*                                                                   */
/* Adapted for use by smenu.                                         */
/*                                                                   */
/* I hereby place this file in the public domain.  It may be freely  */
/* reproduced, distributed, used, modified, built upon, or otherwise */
/* employed by anyone for any purpose without restriction.           */
/* """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""" */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "config.h"
#include "xmalloc.h"

/* ================== */
/* Customized malloc. */
/* ================== */
void *
xmalloc(size_t size)
{
  void * allocated;
  size_t real_size;

  real_size = (size > 0) ? size : 1;
  allocated = malloc(real_size);
  if (allocated == NULL)
  {
    fprintf(stderr,
            "Error: Insufficient memory (attempt to malloc %lu bytes)\n",
            (unsigned long int)size);

    exit(EXIT_FAILURE);
  }

  return allocated;
}

/* ================== */
/* Customized calloc. */
/* ================== */
void *
xcalloc(size_t n, size_t size)
{
  void * allocated;

  n         = (n > 0) ? n : 1;
  size      = (size > 0) ? size : 1;
  allocated = calloc(n, size);
  if (allocated == NULL)
  {
    fprintf(stderr,
            "Error: Insufficient memory (attempt to calloc %lu bytes)\n",
            (unsigned long int)size);

    exit(EXIT_FAILURE);
  }

  return allocated;
}

/* =================== */
/* Customized realloc. */
/* =================== */
void *
xrealloc(void * p, size_t size)
{
  void * allocated;

  allocated = realloc(p, size);
  if (allocated == NULL && size > 0)
  {
    fprintf(stderr,
            "Error: Insufficient memory (attempt to xrealloc %lu bytes)\n",
            (unsigned long int)size);

    exit(EXIT_FAILURE);
  }

  return allocated;
}

/* ==================================== */
/* strdup implementation using xmalloc. */
/* ==================================== */
char *
xstrdup(const char * p)
{
  char * allocated;

#ifdef HAVE_STRDUP
  allocated = strdup(p);

  if (allocated == NULL)
  {
    fprintf(stderr, "Error: Insufficient memory for strdup.\n");

    exit(EXIT_FAILURE);
  }
#else
  allocated = xmalloc(strlen(p) + 1);
  strcpy(allocated, p);
#endif

  return allocated;
}

/* =================================================== */
/* strndup implementation using xmalloc.               */
/* This version guarantees that there is a final '\0'. */
/* =================================================== */
char *
xstrndup(const char * str, size_t len)
{
  char * p;

#ifdef HAVE_STRNDUP
  p = strndup(str, len);

  if (p == NULL)
  {
    fprintf(stderr, "Error: Insufficient memory for strndup.\n");

    exit(EXIT_FAILURE);
  }
#else
  p = memchr(str, '\0', len);

  if (p)
    len = p - str;

  p = xmalloc(len + 1);
  memcpy(p, str, len);
  p[len] = '\0';
#endif

  return p;
}
