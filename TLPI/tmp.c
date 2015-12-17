#include<stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include<malloc.h>
#include<string.h>
#include <sys/types.h>
#include<errno.h>



int removedir(char path[500])
{
    int counter =1;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    struct stat eStat;
    pdir = opendir(path);
    char x[500];

    if ( pdir == NULL)
    {
        return -1;
    }

    //strcpy(x,path);
    while ( (pent=readdir(pdir)) != NULL )
    {

        // printf("\n %s\n" ,pent->d_name);
        if((strcmp((pent->d_name),".")==0)||(strcmp((pent->d_name),"..")==0))
        {	
            printf("Continuing from %s, dname: %s\n",path,pent->d_name);	
            continue;

        }
        else
        {
            printf("Entering first non . or .. file%s , dname: %s\n",path,pent->d_name);
            strcpy(x,path);
            printf("Current X :%s\n",x);
            path=strcat(path,"/");
            path=strcat(path,pent->d_name);

            if(stat(path, &eStat))
            {

                printf("ERROR: %s... Meaning it can be a file(Most certainly)\n", strerror(errno));
                //unlink(path);
                //path=x;

            }
            else{


                if(S_ISDIR(eStat.st_mode))
                { 

                    printf("\n go to directory and remove %s\n",path);
                    removedir(path);
                    strcpy(path,x);
                    //rmdir(path);

                }
                else {

                    unlink(path);
                    printf("Unlinked %s\n",path);
                    strcpy(path,x);
                    printf("After Unlinking now path = : %s\n",path);
                    //strcpy(path,x);
                }	


            }


        } 




    } 


    //closedir (pdir);



    if (!rmdir(path)) return -1; // delete the directory */



}






int main()

{

    int ret;

    char *path_dir=malloc(500);
    char path[500];
    printf("enter Directory name\n");

    scanf("%s",path);

    strcpy(path_dir,path);

    ret=removedir(path_dir);

    printf("\n %d", ret);

    return 111;

    //else return -1;

}
