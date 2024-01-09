#include "headers.h"

void print_relative_path(const char *full_path, const char *home_dir)
{
    // printf("full_path = %s\n", full_path);
    // printf("home_dir = %s\n", home_dir);

    if (strstr(full_path, home_dir) == full_path)
    {
        const char *relative_path = full_path + strlen(home_dir);
        if (relative_path[0] == '/')
        {
            relative_path++;
        }
        printf("./%s\n", relative_path);
    }
    else
    {
        printf("%s\n", full_path);
    }
}

void traverse_directory(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir)
{
    struct dirent **directory;
    // printf("path = %s\n", path);
    int number_of_files = scandir(path, &directory, NULL, alphasort);

    if (number_of_files < 0)
    {
        perror("ERROR: Scandir, directory traversal failed");
        return;
    }

    // printf("number_of_files = %d\n", number_of_files);

    for (int i = 0; i < number_of_files; i++)
    {
        if (strcmp(directory[i]->d_name, ".") == 0 || strcmp(directory[i]->d_name, "..") == 0)
        {
            continue;
        }

        char *file_name_only = strdup(directory[i]->d_name);
        file_name_only = strtok(file_name_only, ".");
        if (file_name_only == NULL)
        {
            continue;
        }

        if (strcmp(file_name_only, search_term) == 0)
        {
            // printf("%s\n", directory[i]->d_name);
            if (directory_flag == 1 && directory[i]->d_type != DT_DIR)
            {
                continue;
            }
            else if (file_flag == 1 && directory[i]->d_type != DT_REG)
            {
                continue;
            }
            char *full_path = (char *)calloc(PATH_MAX, sizeof(char *));
            strcat(full_path, path);
            strcat(full_path, "/");
            strcat(full_path, directory[i]->d_name);
            print_relative_path(full_path, home_dir);
        }

        if (directory[i]->d_type == DT_DIR)
        {
            char *new_path = (char *)calloc(PATH_MAX, sizeof(char *));
            strcat(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, directory[i]->d_name);

            traverse_directory(new_path, search_term, directory_flag, file_flag, home_dir);
        }
    }
}

char *get_file_traverse_directory(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir)
{
    struct dirent **directory;
    // printf("path = %s\n", path);
    int number_of_files = scandir(path, &directory, NULL, alphasort);

    if (number_of_files < 0)
    {
        perror("ERROR: Scandir, directory traversal failed");
        return "-1";
    }

    printf("number_of_files = %d\n", number_of_files);

    for (int i = 0; i < number_of_files; i++)
    {
        if (strcmp(directory[i]->d_name, ".") == 0 || strcmp(directory[i]->d_name, "..") == 0)
        {
            continue;
        }

        char *file_name_only = strdup(directory[i]->d_name);
        file_name_only = strtok(file_name_only, ".");
        if (file_name_only == NULL)
        {
            continue;
        }

        printf("name = %s\n", file_name_only);

        if (strcmp(file_name_only, search_term) == 0)
        {
            if (directory_flag == 1 && directory[i]->d_type != DT_DIR)
            {
                continue;
            }
            else if (file_flag == 1 && directory[i]->d_type != DT_REG)
            {
                continue;
            }
            char *full_path = (char *)calloc(PATH_MAX, sizeof(char));
            strcpy(full_path, path);
            strcat(full_path, "/");
            strcat(full_path, directory[i]->d_name);
            printf("!@!@full_path = %s\n", full_path);
            return full_path;
        }

        if (directory[i]->d_type == DT_DIR)
        {
            char *new_path = (char *)calloc(PATH_MAX, sizeof(char));
            strcpy(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, directory[i]->d_name);

            char *result = get_file_traverse_directory(new_path, search_term, directory_flag, file_flag, home_dir);

            free(new_path); // Free the memory after recursive call

            if (result != NULL)
            {
                return result;
            }
        }
    }
}

void count_traverse_directory(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir, int *count)
{
    struct dirent **directory;
    // printf("path = %s\n", path);
    int number_of_files = scandir(path, &directory, NULL, alphasort);

    if (number_of_files < 0)
    {
        perror("ERROR: Scandir, directory traversal failed");
        return;
    }

    // printf("number_of_files = %d\n", number_of_files);

    for (int i = 0; i < number_of_files; i++)
    {
        if (strcmp(directory[i]->d_name, ".") == 0 || strcmp(directory[i]->d_name, "..") == 0)
        {
            continue;
        }

        char *file_name_only = strdup(directory[i]->d_name);
        file_name_only = strtok(file_name_only, ".");
        if (file_name_only == NULL)
        {
            continue;
        }

        if (strcmp(file_name_only, search_term) == 0)
        {
            if (directory_flag == 1 && directory[i]->d_type != DT_DIR)
            {
                continue;
            }
            else if (file_flag == 1 && directory[i]->d_type != DT_REG)
            {
                continue;
            }
            *count += 1;
        }

        if (directory[i]->d_type == DT_DIR)
        {
            char *new_path = (char *)calloc(PATH_MAX, sizeof(char *));
            strcat(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, directory[i]->d_name);

            count_traverse_directory(new_path, search_term, directory_flag, file_flag, home_dir, count);
        }
    }
}

void print_file_contents(char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = fgetc(file)) != EOF)
    {
        putchar(c);
    }

    fclose(file);
}

char *get_full_path(char *path, char *home_dir)
{
    char *full_path = path + 2;
    strcat(full_path, home_dir);
    printf("full_path = %s\n", full_path);
    return full_path;
}

void alternate_function(char *path, char *search_term, int directory_flag, int file_flag, char *home_dir)
{
    int count = 0;
    count_traverse_directory(path, search_term, directory_flag, file_flag, home_dir, &count);

    // printf("count = %d\n", count);

    if (count == 1)
    {
        // printf("here\n");
        char *full_path = get_file_traverse_directory(path, search_term, directory_flag, file_flag, home_dir);
        printf("full_path = %s\n", full_path);

        struct stat file_stat;

        printf("file mode = %d\n", file_stat.st_mode);
        printf("directory_flag = %d, file_flag = %d\n", directory_flag, file_flag);

        if (S_ISDIR(file_stat.st_mode) && directory_flag && !file_flag)
        {
            if (file_stat.st_mode & S_IXUSR)
            {
                chdir(full_path);
            }
            else
            {
                printf("Missing permissions for task!\n");
            }
        }
        else if (S_ISREG(file_stat.st_mode) && file_flag && !directory_flag)
        {
            if (file_stat.st_mode & S_IRUSR)
            {
                print_file_contents(full_path);
            }
            else
            {
                printf("Missing permissions for task!\n");
            }
        }
        else
        {
            printf("ERROR: Invalid flags!\n");
        }
    }
    else
    {
        traverse_directory(path, search_term, directory_flag, file_flag, home_dir);
    }
}

void seek(char *input, char *home_dir, pastevents *store)
{
    int directory_flag = 0, file_flag = 0, e_flag = 0;
    char default_directory[PATH_MAX];

    // printf("i/p = %s %ld\n", input, strlen(input));

    if (input[0] == '-')
    {
        if (input[1] == 'e')
        {
            e_flag = 1;
            if (input[4] == 'd')
            {
                directory_flag = 1;
                input += 6;
                // second_flag_check = 0;
            }
            else if (input[4] == 'f')
            {
                file_flag = 1;
                input += 6;
                // second_flag_check = 0;
            }
            else
            {
                // remove_last_event(store);
                printf("Invalid flags!\n");
                return;
            }
        }
        else if (input[1] == 'd')
        {
            if (input[3] == '-')
            {
                // remove_last_event(store);
                printf("Invalid flags!\n");
                return;
            }

            directory_flag = 1;
            input += 3;
        }
        else if (input[1] == 'f')
        {
            if (input[3] == '-')
            {
                // remove_last_event(store);
                printf("Invalid flags!\n");
                return;
            }
            file_flag = 1;
            input += 3;
        }
        else
        {
            // remove_last_event(store);
            printf("Invalid flags!\n");
            return;
        }
    }

    // printf("directory_flag = %d, file_flag = %d\n", directory_flag, file_flag);
    // printf("input = %s %ld\n", input, strlen(input));

    const char delimiter[] = " \t";
    int number_of_tokens = 0;
    char *token = strtok(input, delimiter), *tokenised_input[3];

    while (token != NULL)
    {
        tokenised_input[number_of_tokens] = token;
        number_of_tokens++;
        token = strtok(NULL, delimiter);
    }

    char *search_term, *path = NULL, *relative_directory = NULL;

    // printf("number_of_tokens = %d\n", number_of_tokens);
    search_term = tokenised_input[0];

    if (number_of_tokens != 1)
    {
        path = tokenised_input[1];
    }
    else
    {
        if (getcwd(default_directory, PATH_MAX) != NULL)
        {
            path = default_directory;
        }
        else
        {
            perror("ERROR: getcwd");
            return;
        }
    }

    if (path == NULL || strlen(path) == 0 || strcmp(path, ".") == 0)
    {
        // printf("here for path\n");
        getcwd(path, PATH_MAX);
    }
    else if (strcmp(path, "~") == 0)
    {
        path = home_dir;
    }

    // printf("search: %s (%ld)\n", search_term, strlen(search_term));
    // printf("path: %s (%ld)\n", path, strlen(path));

    if (e_flag)
    {
        // printf("e_flag\n");
        alternate_function(path, search_term, directory_flag, file_flag, home_dir);
    }
    else
    {
        int count = 0;
        // printf("here %s\n", path);
        count_traverse_directory(path, search_term, directory_flag, file_flag, home_dir, &count);

        if (count == 0)
        {
            printf("No matches found!\n");
        }
        else
        {
            // printf("count = %d\n", count);
            traverse_directory(path, search_term, directory_flag, file_flag, home_dir);
        }
    }
}
