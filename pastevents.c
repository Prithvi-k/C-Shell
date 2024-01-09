#include "headers.h"

void load_past_events(FILE *fp, pastevents *store)
{
    fp = fopen(PATH_COMMANDS_STORE_FILE, "r+");
    if (fp == NULL)
    {
        fp = fopen(PATH_COMMANDS_STORE_FILE, "w+");
        return;
    }

    fread(store, sizeof(pastevents), 1, fp);
    fclose(fp);

    if (store->count == 0)
    {
        return;
    }
    else if (strcmp(store->events[store->count - 1], "exit") == 0)
    {
        return;
    }
    add_past_event("exit\n", fp, store);
    return;
}

void add_past_event(char *input, FILE *fp, pastevents *store)
{
    if (store->count > 0 && strcmp(input, store->events[store->count - 1]) == 0)
    {
        return;
    }

    // printf("%s %d\n", input, store->count);
    if (store->count == MAX_NUMBER_OF_COMMANDS_PAST_EVENTS)
    {
        for (int i = 0; i < MAX_NUMBER_OF_COMMANDS_PAST_EVENTS - 1; i++)
        {
            // printf("%d  ", i);
            strcpy(store->events[i], store->events[i + 1]);
        }
        // printf("here\n");
        strcpy(store->events[(MAX_NUMBER_OF_COMMANDS_PAST_EVENTS - 1)], input);
    }
    else
    {
        // printf("wow here\n");
        strcpy(store->events[store->count++], input);
    }
    // printf("final\n");
    fp = fopen(PATH_COMMANDS_STORE_FILE, "r+");
    fwrite(store, sizeof(pastevents), 1, fp);
    fclose(fp);
}

void print_past_events(FILE *fp, pastevents *store)
{
    for (int i = 0; i < store->count; i++)
    {
        printf("%s", store->events[i]);
    }
    return;
}

void past_events_purge(FILE *fp, pastevents *store)
{
    fp = fopen(PATH_COMMANDS_STORE_FILE, "w");
    if (fp == NULL)
    {
        return;
    }
    fclose(fp);
    // for (int i = 0; i < store->count; i++)
    // {
    //     free(store->events[i]);
    // }
    store->count = 0;
    return;
}

void past_events_execute(pastevents *store, int index, FILE *fp, char *home_dir, char *previous_dir, pid_t pid, char *prompt_parameter)
{
    if (index > store->count)
    {
        printf("No such event exists\n");
        return;
    }
    char *input = strdup(store->events[store->count - index]);
    if (strcmp(input, store->events[store->count - 1]) == 0 && index != 1)
    {
        return;
    }

    for (int i = (store->count - index); i < (store->count - 1); i++)
    {
        strcpy(store->events[i], store->events[i + 1]);
    }
    strcpy(store->events[store->count - 1], input);

    fp = fopen(PATH_COMMANDS_STORE_FILE, "r+");
    fwrite(store, sizeof(pastevents), 1, fp);
    fclose(fp);

    execute_command(input, home_dir, previous_dir, pid, fp, store, prompt_parameter);

    return;
}

void remove_last_event(pastevents *store)
{
    if (store->count == 0)
    {
        return;
    }
    store->count--;
    return;
}

void past_events_manager(char *input, FILE *fp, pastevents *pe, char *home_dir, char *previous_dir, pid_t pid, char *prompt_parameter)
{
    // printf("in: %s\n", input);
    if (input == NULL || strlen(input) == 0)
    {
        // printf("here\n");
        print_past_events(fp, pe);
        return;
    }
    else if (strstr(input, "execute"))
    {
        int index = 8;
        int index_to_execute = input[index] - '0';
        // printf("index: %d\n", index_to_execute);
        past_events_execute(pe, index_to_execute, fp, home_dir, previous_dir, pid, prompt_parameter);
        return;
    }
    else if (strstr(input, "purge"))
    {
        // printf("purging\n");
        past_events_purge(fp, pe);
        return;
    }
    else
    {
        printf("Invalid command\n");
        return;
    }
}
