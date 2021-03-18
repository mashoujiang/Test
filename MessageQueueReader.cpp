#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
    key_t key;
    int msgid;
    msgbuf message;

    // 1. generate a key
    key = ftok("profile", 65);

    // 2. create a IPC
    msgid = msgget(key, 0666 | IPC_CREAT);

    // 3. read message from ipc
    msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Data Received is: %s \n", message.mtext);

    // 4. remove identifier
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}