#include <stdio.h>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netinet/in.h>

#include "proto/message_define.pb.h"

// #include "common/mess_type.h"
#include "common/ret_value.h"

unsigned char send_buffer[10244];
unsigned char recv_buffer[10244];

int sock;

ssp::RegReq regReq;
ssp::RegRsp regRsp;

int ProtoInit() {
    // regReq.set_ver(1);
    regReq.set_mess_type(401); // Registration message
    regReq.set_user_name("hank");
    regReq.set_password("12345678");
    return 0;
}

int SetReqMessType(const char *mess_type) {
    send_buffer[0] = mess_type[0];
    send_buffer[1] = mess_type[1];
    send_buffer[2] = mess_type[2];
    return 0;
}

int SendRegReq() {
    SetReqMessType("401");
    regReq.SerializeToArray(send_buffer + 3, 10240);
    printf("%s\n", send_buffer + 3);
    int size_buffer = sizeof(send_buffer);
    int ret = send(sock, send_buffer, size_buffer, 0);
    if (ret > 0) {
        return 0;
    }
    return -1;
}

int mess_type;

int GetMessType() {
    return ((recv_buffer[0] - '0') * 100 + (recv_buffer[1] - '0') * 10 + recv_buffer[2] - '0');
}

int RecvRegRsp() {
    int ret = recv(sock, recv_buffer, 10244, 0);
    printf("ret: %d\n", ret);
    GetMessType();
    printf("mess_type: %d\n", mess_type);
    if (mess_type != 402) {
        return -1;
    }
    // The response received at this point is 402
    regRsp.ParseFromArray(recv_buffer + 3, 10240);
    printf("regRsp:{ret: %d}\n", regRsp.ret());
    if (regRsp.ret() < 0) {
        printf("error\n");
        return -1;
    }
    return 0;
}

int ClientSocketInit() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8999);
    int conn_ret = connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (conn_ret < 0) {
        printf("connect_ret: %d\n", conn_ret);
        return conn_ret;
    }
    return 0;
}

int main() {
    int client_on = 1;
    int n = 0;
    int ret;
    ClientSocketInit();
    while (client_on) {
        switch (n) {
            case 0: 
                ProtoInit();
                n = 1; // Sends packets to the server
                break;
            case 1: 
                ret = SendRegReq();
                printf("%d\n", ret);
                if (ret == 0) {
                    n = 2; // The packet is successfully sent and received from the server
                }
                break;
            case 2: 
                printf("%d\n", n);
                ret = RecvRegRsp();
                printf("res%d", ret);
                if (ret == 0) {
                    n = 1; // The packet is successfully received and sent to the server
                }
                else {
                    if (ret == -2) {
                        client_on = 0;
                    }
                }
                break;
            default:
                client_on = 0;
                break;
        }
    }
}