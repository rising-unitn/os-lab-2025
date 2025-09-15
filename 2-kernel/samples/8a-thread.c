#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 
#include <string.h>

void *my_function(void *arg) {
  printf("Hello from the thread\n");
  pthread_exit((void *) "hello everyone");
}

int main() {
    pthread_t tid;
    pthread_attr_t attr;
    void *retval;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, my_function, NULL);
    pthread_join(tid, &retval);
    printf("Thread returned %s\n", (char *) retval);
    return 0;
}
