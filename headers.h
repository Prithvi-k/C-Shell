#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>

#define HOST_NAME_LENGTH 255
#define PATH_MAX 4096
#define MAX_NUMBER_OF_COMMANDS 100
#define MAX_NUMBER_OF_COMMANDS_PAST_EVENTS 15
#define PATH_COMMANDS_STORE_FILE "path_commands_store.txt"

#define COLOR_RESET "\x1B[0m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_BLUE "\x1B[34m"

typedef struct pastevents
{
    int count;
    char events[MAX_NUMBER_OF_COMMANDS_PAST_EVENTS][PATH_MAX];
} pastevents;

#include "prompt.h"
#include "command_execution.h"
#include "warp.h"
#include "peek.h"
#include "seek.h"
#include "proclore.h"
#include "system_commands.h"
#include "pastevents.h"

#endif