#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 100

int main()
{
    key_t key;
    int msgid;
    msgbuf message;

    // 1. generate a key
    key = ftok("profile", 65);

    // 2. create ipc
    msgid = msgget(key, 0666 | IPC_CREAT);

    // 3. fill in message
    message.mtype = 1;
    printf("Write Data: ");
    fgets(message.mtext, MAX, stdin);

    // 4. send message
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Data send is : %s \n", message.mtext);
    return 0;
}