#ifndef PROTO2_H__
#define PROTO2_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'g'

#define PATHMAX 1024
#define DATAMAX 1024

enum
{
    MSG_PATH=1,
    MSG_DATA,
    MSG_EOT
};

typedef struct msg_path_st
{
    long mtype;         /* must be MSG_PATH */
    char path[PATHMAX]; /* ASCIIZ 带尾0的串*/
}msg_path_t;

typedef struct mag_s2c_st
{
    long mtype;         /* must be MSG_DATA or MSG_EOT */
    int datalen;
        /*
            datalen>0   :data
            datalen=0   :eot
        */
    char data[DATAMAX];
}msg_data_t;


#endif