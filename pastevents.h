#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

void past_events_execute(pastevents *store, int index, FILE *fp, char *home_dir, char *previous_dir, pid_t pid, char *prompt_parameter);
void past_events_purge(FILE *fp, pastevents *store);
void print_past_events(FILE *fp, pastevents *store);
void add_past_event(char *input, FILE *fp, pastevents *store);
void load_past_events(FILE *fp, pastevents *store);
void past_events_manager(char *input, FILE *fp, pastevents *pe, char *home_dir, char *previous_dir, pid_t pid, char *prompt_parameter);
void remove_last_event(pastevents *store);

#endif