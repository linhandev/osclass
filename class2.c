int producer(void * args)
{
  int id=*((int* )args);
  int i;
  for(i=0;i<10;i++)
  {
    sleep(i+1);
    sem_wait(&warehouse);
    pthread_mutex_lock(&mutex);
    if(id==0)
      strcpy(buffer[bp],"aaa\0");
    else
      strcpy(buffer[bp],"bbb\0");
    bp++;
    printf("producer %d produce %s in %d\n",id,buffer[bp-1],bp-1);
    pthread_mutex_unlock(&mutex);
    sem_post(&product);
  }
  printf("producer %d is over! \n",id);
}
int consumer(void * args)
{
  int id=*((int* )args);
  int i;
  for(i=0;i<10;i++)
  {
    sleep(10-i);
    sem_wait(&product);
    ptheread_mutex_lock(&mutex);
    bp--;
    printf("consumer %d get %s in %d\n",id,buffer[bp],bp);






  }
}
