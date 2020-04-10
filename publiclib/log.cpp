#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <QtDebug>
#include "DateUtils.h"
#include "log.h"

static struct {
  void *udata;
  log_LockFn lock;
  FILE *fp;
  int level;
  int quiet;
  bool console;
} L;


static const char *level_names[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

static void lock(void)   {
  if (L.lock) {
    L.lock(L.udata, 1);
  }
}


static void unlock(void) {
  if (L.lock) {
    L.lock(L.udata, 0);
  }
}


void log_set_udata(void *udata) {
  L.udata = udata;
}


void log_set_lock(log_LockFn fn) {
  L.lock = fn;
}


void log_set_fp(const char * file ) {
	if (file != NULL)
	{
		if (L.fp != NULL)
		{
			fclose(L.fp);
		}
		FILE *fp = fopen(file, "a+");
		L.fp = fp;
	}
	else{
		if (L.fp != NULL)
		{
			fclose(L.fp);
			L.fp = NULL;
		}
	}
}


void log_set_level(int level) {
  L.level = level;
}

void log_set_console(bool console)
{
	L.console = console;
}

void log_set_quiet(int enable) {
  L.quiet = enable ? 1 : 0;
}


void log_log(int level, const char *func, int line, const char *fmt, ...) {
  if (level < L.level) {
    return;
  }

  /* Acquire lock */
  lock();

  /* Log to file */
  QString current_date = DateUtils::getTime();
  if (L.fp) {
    va_list args;
	if (fmt != NULL && strcmp(fmt,""))
	{
		fprintf(L.fp, "%s %-5s %s:%d ===> ", current_date.toStdString().c_str(), level_names[level], func, line);
		va_start(args, fmt);
		vfprintf(L.fp, fmt, args);
		va_end(args);
	}
	else{
		fprintf(L.fp, "%s %-5s %s:%d", current_date.toStdString().c_str(), level_names[level], func, line);
	}
	
    fprintf(L.fp, "\r\n");
    fflush(L.fp);
  }
  /* Log to console */
#ifdef QT_DEBUG
  if (L.console) {
	  QString szLogText;
	  va_list args;
	  szLogText = QString("%1 %2 %3 %4 ===> ").arg(current_date).arg(level_names[level]).arg(func).arg(line);
	  char szLogBuf[4096] = { 0 };
	  va_start(args, fmt);
	  _vsnprintf_s(szLogBuf, 4096, 4095, fmt, args);
	  va_end(args);
	  szLogText = szLogText + szLogBuf;
	  qDebug() << szLogText;
  }
#endif
  /* Release lock */
  unlock();
}
