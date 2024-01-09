#ifndef __PROCLORE_H
#define __PROCLORE_H

void proclore(char *path, char *name, pid_t pid, pastevents *store);
void get_process_details(char *input_path, pid_t pid, char *home_dir, pastevents *store);
void print_relative_path_modified_for_proclore(const char *full_path, const char *home_dir);

#endif