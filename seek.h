#ifndef __SEEK_H
#define __SEEK_H

void traverse_directory(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir);
void seek(char *input, char *home_dir, pastevents *store);
void alternate_function(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir);
void print_file_contents(char *file_path);
void count_traverse_directory(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir, int *count);
char *get_file_traverse_directory(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir);
void print_relative_path(const char *full_path, const char *home_dir);
void change_directory(char *path, char *home_dir);

#endif