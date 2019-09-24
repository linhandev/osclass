#ifdef  HAVE_CONFIG_H
#include  <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#define MAPSIZE 100
int maxsize;

struct map  //存储资源表结构
{
    int m_addr;
    int m_size;
};
struct map map[MAPSIZE];  //存储资源表
//BF存储分配函数
int BF_malloc(struct map *mp,int size)
{
    register int a,s;
    register struct map *bp,*bpp;
    for(bp = mp; bp->m_size; bp++)
    {
       if (bp->m_size >= size)
        {
            a = bp->m_addr;
            s = bp->m_size;
            for(bpp = bp; bpp->m_size; bpp++)
            {   //最佳适应
                if(bpp->m_size >= size && bpp->m_size < s)
                {
                    a = bpp->m_addr;
                    s = bpp->m_size;
                    bp = bpp;
                }
            }

            bp->m_addr += size;
            if ((bp->m_size -= size) == 0)
                do
                {
                    bp++;
                    (bp-1)->m_addr = bp->m_addr;
                }
                while((bp-1)->m_size = bp->m_size);
            return(a);
        }
    }
    return(-1);
}

//WF存储分配函数
int WF_malloc(struct map *mp,int size)
{
    register int a,s;
    register struct map *bp,*bpp;
    for(bp = mp; bp->m_size; bp++)
    {
        if (bp->m_size >= size)
        {
            a = bp->m_addr;
            s = bp->m_size;
            for(bpp = bp; bpp->m_size; bpp++)
            {   //最坏适应
                if(bpp->m_size > s)
                {
                    a = bpp->m_addr;
                    s = bpp->m_size;
                    bp = bpp;
                }
            }
            bp->m_addr += size;
            if ((bp->m_size -=size) == 0)
                do
                {
                    bp++;
                    (bp-1)->m_addr = bp->m_addr;
                }
                while((bp-1)->m_size = bp->m_size);
            return(a);
        }
    }
    return(-1);
}

//存储释放函数
void mfree(struct map *mp,int aa,int size)
{
  // struct map temp;
  // int i=0,j=0;
  // for(i=0;map[i].m_size!=0;i++)
  //  for(j=i;map[j].m_size!=0;j++)
  //    if(map[i].m_addr>map[j].m_addr)
  //      temp=map[i],map[i]=map[j],map[j]=temp;
    register struct map *bp;
    register int t;
    register int a;
    a = aa;

    for(bp = mp; bp->m_addr<=a && bp->m_size != 0; bp++)
        ;
    printf("%d,%d\n",bp->m_addr,bp->m_size);
    printf("需要释放%d %d\n",aa,size);
    int flag=1;
    if(aa+size>maxsize)
    {
      printf("释放空间过长，超过了总空间\n");
      size=maxsize-aa;
    }

    if( !(aa+size<(bp-1)->m_addr) && aa<(bp-1)->m_addr+(bp-1)->m_size )
    {
      printf("释放起点过低，包含了上一个空间%d %d一部分\n",(bp-1)->m_addr,(bp-1)->m_size);
      if((bp-1)->m_addr+(bp-1)->m_size>=aa+size)
        printf("调整free起点超过终点，区间被完全包含\n"),flag=0;
      int end=aa+size;
      aa=(bp-1)->m_addr+(bp-1)->m_size;
      size=end-aa;
    }


    if((aa+size > bp->m_addr) && bp->m_size!=0 && flag)
    {
      printf("释放过长，到了下一个空区间%d %d\n",bp->m_addr,bp->m_size);
      size=bp->m_addr-aa;
    }
    printf("调整过的释放的区域为%d %d\n",aa,size);

    if(flag==0)
    {
      printf("不需要操作，退出\n");
      return;
    }

    a = aa;

    if(bp>mp && (bp-1)->m_addr+(bp-1)->m_size==a)
    {  //与前合并
         (bp-1)->m_size += size;
         if (a+size == bp->m_addr)
         {   //前后合并
              (bp-1)->m_size += bp->m_size;
              while (bp->m_size)
              {
                  bp++;
                  (bp-1)->m_addr = bp->m_addr;
                  (bp-1)->m_size = bp->m_size;
              }
         }
    }
    else
    {
         if (a+size == bp->m_addr && bp->m_size)
         {   //与后合并
              bp->m_addr -= size;
              bp->m_size += size;
         }
         else if (size)
              do
              {   //无合并
                   t = bp->m_addr;
                   bp->m_addr = a;
                   a = t;
                   t = bp->m_size;
                   bp->m_size = size;
                   bp++;
              }
              while (size = t);
    }
    // for(bp=mp;;bp++)
    //   printf("%d,%d\n",bp->m_addr,bp->m_size);

}
void init()
{
    struct map *bp;
    int addr,size;
    int i=0;
    bp=map;
    printf("Please input starting addr and total size:");
    scanf("%d %d",&addr,&size);
    maxsize=size;
    getchar();
    bp->m_addr=addr;
    bp->m_size=size;
    (++bp)->m_size=0;  //表尾
}

void show_map()
{
     //system("clear");  //清屏

     struct map *bp;
     bp=map;
     printf("\nCurrent memory map...\n");
     printf("Address\t\tSize\n");
     while(bp->m_size!=0)
     {
        printf("<%d\t\t%d>\n",bp->m_addr,bp->m_size);
        bp++;
     }
     printf("\n");
}
main()
{
    int a,s;
    char c;
    int i;
    init();
    printf("please input, b for BF, w for WF:");
    scanf("%c",&c);
    getchar();
    do
    {
      show_map(); //显示存储资源表

      printf("Please input,1 for request,2 for release,0 for exit:");
      scanf("%d",&i);
      getchar();
      switch(i)
      {
          case 1:
              printf("Please input size:");
              scanf("%d", &s);
              getchar();
              if(c=='b')  //BF
                  a=BF_malloc(map,s),printf("---bf-----\n");
              else  //WF
                  a=WF_malloc(map,s),printf("---wf-----\n");
              if(a==-1)
                  printf("request can't be satisfied\n");
              else
                  printf("alloc memory at address:%d,size:%d\n",a,s);
              break;
          case 2:
              printf("Please input addr and size:");
              scanf("%d,%d",&a,&s);
              getchar();
              mfree(map,a,s);
              break;
          case 0:
              exit(0);
      }
    }
    while(1);
}
