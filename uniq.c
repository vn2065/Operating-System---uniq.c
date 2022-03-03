#include "types.h"
#include "stat.h"
#include "user.h"


char buf[1000];
char *f;
char *dupl;
int *nline;
int dup_count = 0;


int compareArrays(char *a, char *b,int n,int iflag)
{
    int i;
    for(i = 0; i < n; i++)
    {
        if (*(a + i) != *(b + i))
        {
            if(iflag == 1)
            {
                if(*(a + i) - *(b + i) != 32 && *(b + i) - *(a + i) != 32)
                    return 0;
            }
            else
            {
                return 0;
            }
        }
    }
    return 1;
}
int uniq(int fd, char *name, int cflag, int dflag, int iflag)
{
    int i, n;
    int l = -1, dp = -1;
    int c = 0, tc = 0, ac = 0;
    char *pl;
    char *cl;
   
    while((n = read(fd, buf, sizeof(buf))) > 0)
    {
        f = (char*) malloc(n);
        dupl=(char*) malloc(n);
        nline = (int*) malloc(n);
        pl = (char*) malloc(n);
        cl = (char*) malloc(n);
        
        for(i = 0; i < n; i++)
        {
            c++;
            
            if(buf[i] == '\n')
            {
                if(compareArrays(pl, cl, ac, iflag) == 1)
                {
                    *(nline + l) = *(nline + l) + 1;
                    if(dp == 0)
                    {
                        int j;
                        for(j = 0; j < ac; j++)
                        {
                            *(dupl + dup_count) = *(pl + j);
                                                        dup_count++;
                        }
                        *(dupl + dup_count) = '\n';
                        dup_count++;
                        dp = 1;
                    }
                    ac = 0;
                }
                else
                {
                    int j;
                    l++;
                    *(nline + l) = 1;
                    dp = 0;
            
                    for(j=0;j<ac;j++)
                    {
                        *(pl + j) = *(cl + j);
                        *(f + tc) = *(cl + j);
                        tc++;
                    }
                    *(f + tc) = '\n';
                    tc++;
                    ac = 0;
                }
            }
            else
            {
                *(cl+ac) = buf[i];
                ac++;
            }
        }
        
        free(cl);
        free(pl);
    }
    if(n < 0)
    {
        printf(1, "uniq: error\n");
        exit();
    }
    
    return tc;
}
int
main(int argc, char *argv[])
{
    int fd, i;
    int fo;
    int cflag, dflag, iflag, nflags;
    int count;
    fo = cflag = dflag = iflag = nflags = 0;
 
    if(argc <= 1)
    {
        count=uniq(0, "", cflag, dflag, iflag);
    }
    else
    {
        for(i = 1; i < argc; i++)
        {
            if(strcmp(argv[i], "-c") == 0)
            {
                if(fo > 0)
                {
                    printf(1, "Incorrect usage\n");
                    exit();
                }
                cflag = 1;
                nflags++;
            }
            if(strcmp(argv[i], "-d") == 0)
            {
                if(fo > 0)
                {
                    printf(1, "Incorrect usage\n");
                    exit();
                }
                dflag = 1;
                nflags++;
            }
            if(strcmp(argv[i],"-i") == 0)
            {
                if(fo > 0)
                {
                    printf(1,"Incorrect usage\n");
                    exit();
                }
                iflag = 1;
                nflags++;
            }
            if((fd = open(argv[i], 0)) >= 0 )
            {
                fo = i;
            }
        
        }
        if(fo == 0 && (nflags + 1) != argc)
        {
            printf(1, "File does not exist\n");
            exit();
        }
        else
        {
            if(fo == 0)
            fd = 0;
        }
                if(cflag == 1 && dflag == 1)
        {
            printf(1, "-c and -d cannot cannot be processed together\n");
            exit();
        }
    
        count = uniq(fd, argv[fo], cflag, dflag, iflag);
        close(fd);
    }
    int l = 0;
    if(dflag == 1)
    {
        for(i = 0; i < dup_count; i++)
        {
            printf(1, "%c", *(dupl + i));
        }
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            if (cflag == 1)
            {
                if(i == 0)
                {
                    printf(1, "%d ", *(nline + l));
                    l++;
                }
                if(i > 0 && *(f + i - 1) == '\n')
                {
                    printf(1, "%d ", *(nline + l));
                    l++;
                }
                printf(1, "%c", *(f + i));
            }
            else
            {
                printf(1, "%c", *(f + i));
            }
        }
    }
   
    free(f);
    exit();
}