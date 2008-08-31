/*
 * $Id$
 */

#ifndef INCLUDE_GLOBAL_H
#define INCLUDE_GLOBAL_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <arpa/inet.h>	
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>


#include <pthread.h>

#include <libircclient.h>

#include "config.h"
#include "log.h"
#include "ircmud.h"

extern CConfig gCfg;
extern CLog	gLog;


#endif /* INCLUDE_GLOBAL_H */
