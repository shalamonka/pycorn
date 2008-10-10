/*
 * Copyright 2008 Torne Wuff
 *
 * This file is part of Pycorn.
 *
 * Pycorn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <sys/iosupport.h>
#include "pycorn_mach.h"

int serial_write(struct _reent *r, int fd, const char* ptr, int len)
{
  int i;
  for (i = 0; i < len; ++i)
  {
    while((FFLSR & LSR_TEMT) == 0);
    FFTHR = ptr[i];
  }
  return len;
}

int serial_read(struct _reent *r, int fd, char* ptr, int len)
{
  if (len == 0)
    return 0;

  while((FFLSR & LSR_DR) == 0);
  ptr[0] = FFRBR;
  if (ptr[0] == '\r')
    ptr[0] = '\n';
  serial_write(NULL, 0, ptr, 1);
  return 1;
}

const devoptab_t dotab_serial =
{
  "serial",
  0,
  NULL,
  NULL,
  serial_write,
  serial_read
};

void serial_init() __attribute__((constructor));

void serial_init()
{
  devoptab_list[STD_IN] = &dotab_serial;
  devoptab_list[STD_OUT] = &dotab_serial;
  devoptab_list[STD_ERR] = &dotab_serial;
}