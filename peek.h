#ifndef __PEEK_H
#define __PEEK_H

void peek(char *input, char *home_dir, char *prev_dir, pastevents *store);
char *parent_path(const char *path);
int custom_strlen(char *string);

#endif