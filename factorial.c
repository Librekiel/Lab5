#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <getopt.h>
#include <stdbool.h>

pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;

int result=1;

struct fact_part
{
    int k;
    int start;
    int end;
    int mod;
};

void *ThreadFactorial(void* args)
{
    struct fact_part *fargs = (struct fact_part *)args;
    int mod=fargs->mod;
    int endp=0; int add=0;
    if (fargs->start <= fargs->k)
    {
        if (fargs->end >= fargs->k)
        {
            endp=fargs->k+1;
        }
        else
        {
            endp=fargs->end;
        }
        for (int i=fargs->start; i<endp; i++)
        {
            pthread_mutex_lock(&mutex_1);
            if (i%mod==0)
            {
                add=i/mod;
            }
            else
            {
                add=i%mod;
            }
            result=(result*add);
            pthread_mutex_unlock(&mutex_1);
        }
    }

}

int main(int argc, char **argv)
{
  int k = -1;
  int pnum = -1;
  int mod = -1;
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            if (k<0)
            {
                printf("Error: k<0\n");
                return 0;
            }
            break;
          case 1:
            pnum = atoi(optarg);
            if (pnum<=0)
            {
                printf("Error: pnum<=0\n");
                return 0;
            }
            break;
          case 2:
            mod = atoi(optarg);
            if (mod<=0)
            {
                printf("Error: mod<0\n");
                return 0;
            }
            break;

          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
  
  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }
  
  if (k==-1 || pnum ==-1 || mod==-1)
  {
    printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n", argv[0]);
    return 1;
  }
  
  pthread_t threads[pnum];
  struct fact_part res[pnum];
  int step=(k-k%pnum)/pnum+1;
  for (int i=0; i<pnum; i++)
  {
      res[i].k=k;
      res[i].start=i*step+1;
      res[i].end=(i+1)*step+1;
      res[i].mod=mod;
      if (pthread_create(&threads[i], NULL, ThreadFactorial, (void *)&res[i])) {
        printf("Error: pthread_create failed!\n");
        return 1;
      }
  }
  for (int i = 0; i < pnum; i++) {
    pthread_join(threads[i], NULL);
  }
  
  printf("!%d mod %d = %d, step was %d\n",k,mod,result,step);
    
  return 0;
}