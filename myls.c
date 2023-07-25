#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

// alpha sort function;
int alphasort(const struct dirent **a, const struct dirent **b)
{
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

void ls(const char *dir, bool iCommand, bool lCommand, bool RCommand)
{
    struct dirent **entry;
    struct stat fileInfoBuffer;
    struct tm *tm;
    DIR *d = opendir(dir);
    int n;
    if (d == NULL)
    {
        printf("Error opening directory\n");
        exit(0);
    }

    n = scandir(dir, &entry, NULL, NULL);
    if (n < 0)
    {
        printf("Error");
        exit(0);
    }
    else
    {
        while (n--)
        {
            //get rid of hidden files        
            if (entry[n]->d_name[0] == '.')
            {
                continue;
            }
            if(iCommand){
                printf("%ld\t", entry[n]->d_ino); //print inode number
            }
            if(lCommand){
                if(stat(dir, &fileInfoBuffer) == 0){
                    struct passwd *pw = getpwuid(fileInfoBuffer.st_uid);
                    struct group *gr = getgrgid(fileInfoBuffer.st_gid);
                    char accessTime[256];
                    strftime(accessTime, sizeof(accessTime), "%b %d %Y %H:%M", localtime(&fileInfoBuffer.st_mtime));
                    printf("permissions...\t%ld\t%s\t%s\t%ld\t%s\t", fileInfoBuffer.st_nlink, pw->pw_name, gr->gr_name, fileInfoBuffer.st_size, accessTime); //print long format (permissions, hardLinks, userName, groupName, fileSize, dateAccessed, fileName )
                }
            }
            if(RCommand){
                printf("%ld ", entry[n]->d_ino); //currently just prints inode number
            }
            printf("%s \n", entry[n]->d_name);

            free(entry[n]);
        }
    }
    free(entry);
}

int main(int argc, char *argv[])
{

    bool iCommand = false;
    bool lCommand = false;
    bool RCommand = false;

    // if no parameter
    if (argc == 1)
    {
        ls(".", 0, 0, 0);
    }
    else
    {
        //if a single argument and any number of files
        if(argv[1][0] == '-' && argv[2][0] != '-'){
            // if first character of argument is a command -> starts with "-"
            if(strcmp(argv[1],"-i") == 0)
            {
                //printf("-i");
                iCommand = true;
            }
            if(strcmp(argv[1],"-l") == 0)
            {
                lCommand = true;
            }
            if(strcmp(argv[1],"-R") == 0)
            {
                RCommand = true;
            }
            for (int i = 2; i < argc; i++)
            {
                // if first character of argument is a file path
                if (argv[i][0] != '-')
                {
                    printf("%s:\n", argv[i]);
                    ls(argv[i], iCommand, lCommand, RCommand);
                    printf("\n");
                    
                }
            }

        } 
        //if no arguments and any number of files
        else if(argv[1][0] != '-'){
            for (int i = 1; i < argc; i++)
            {
                // if first character of argument is a file path
                if (argv[i][0] != '-')
                {
                    printf("%s:\n", argv[i]);
                    ls(argv[i], 0, 0, 0);
                    printf("\n");
                    
                }
            }
        }

        

    }

    return 0;
}
