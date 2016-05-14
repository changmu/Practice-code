#include "myLinux.h"
#include "pub.h"

/* big endian */
USER_LIST client_list;

void do_login(MESSAGE& msg, int sock, struct sockaddr_in *cliaddr);
void do_logout(MESSAGE& msg, int sock, struct sockaddr_in *cliaddr);
void do_sendlist(int sock, struct sockaddr_in *cliaddr);

void do_login(MESSAGE& msg, int sock, struct sockaddr_in *cliaddr)
{
        USER_INFO user;
        strcpy(user.username, msg.body);
        user.ip = cliaddr->sin_addr.s_addr;
        user.port = cliaddr->sin_port;

        /* look up user */
        USER_LIST::iterator it;
        for (it = client_list.begin(); it != client_list.end(); ++it) {
                if (strcmp(it->username, user.username) == 0)
                        break;
        }

        if (it == client_list.end()) { /* not found */
                printf("a user login: [%s]<->[%s:%hu]\n", user.username, inet_ntoa(cliaddr->sin_addr), ntohs(user.port));
                client_list.push_back(user);

                /* response for login succeed */
                MESSAGE reply_msg;
                memset(&reply_msg, 0, sizeof(reply_msg));
                reply_msg.cmd = htonl(S2C_LOGIN_OK);

                sendto(sock, &reply_msg, sizeof(reply_msg), 0, (struct sockaddr *) cliaddr, sizeof(*cliaddr));

                /* send online users' num to client */
                int count = htonl((int) client_list.size());
                sendto(sock, &count, sizeof(count), 0, (struct sockaddr *) cliaddr, sizeof(*cliaddr));

                /* send online list to client */
                printf("sending user list information to: [%s]<->[%s:%hu]\n", user.username, inet_ntoa(cliaddr->sin_addr), ntohs(user.port));
                for (it = client_list.begin(); it != client_list.end(); ++it) {
                        sendto(sock, &*it, sizeof(USER_INFO), 0, (struct sockaddr *) cliaddr, sizeof(*cliaddr));
                }

                /* send online info to other clients */
                for (it = client_list.begin(); it != client_list.end(); ++it) {
                        if (strcmp(it->username, msg.body) == 0)
                                continue;

                        struct sockaddr_in peeraddr;
                        memset(&peeraddr, 0, sizeof(peeraddr));
                        peeraddr.sin_family = AF_INET;
                        peeraddr.sin_port = it->port;
                        peeraddr.sin_addr.s_addr = it->ip;

                        msg.cmd = htonl(S2C_SOMEONE_LOGIN);
                        memcpy(msg.body, &user, sizeof(user));

                        int ret = sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr *) &peeraddr, sizeof(peeraddr));
                        if (ret < 0)
                                ERR_EXIT("sendto");
                }

        } else { /* found the same name */
                printf("user [%s] has already logined\n", msg.body);

                MESSAGE reply_msg;
                memset(&reply_msg, 0, sizeof(reply_msg));
                reply_msg.cmd = htonl(S2C_ALREADY_LOGINED);
                sendto(sock, &reply_msg, sizeof(reply_msg), 0, (struct sockaddr *) cliaddr, sizeof(*cliaddr));
        }
}

void do_logout(MESSAGE& msg, int sock, struct sockaddr_in *cliaddr)
{
        printf("@a user logout:[%s]<->[%s:%d]\n", msg.body, inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port));

        USER_LIST::iterator it;
        for (it = client_list.begin(); it != client_list.end(); ++it) {
                if (strcmp(it->username, msg.body) == 0)
                        break;
        }

        if (it != client_list.end())
                client_list.erase(it);

        /* tell others that someone has logout */
        for (it = client_list.begin(); it != client_list.end(); ++it) {
                if (strcmp(it->username, msg.body) == 0)
                        continue;

                struct sockaddr_in peeraddr;
                memset(&peeraddr, 0, sizeof(peeraddr));
                peeraddr.sin_family = AF_INET;
                peeraddr.sin_port = it->port;
                peeraddr.sin_addr.s_addr = it->ip;

                msg.cmd = htonl(S2C_SOMEONE_LOGOUT);

                int ret = sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr *) &peeraddr, sizeof(peeraddr));
                if (ret < 0)
                        ERR_EXIT("sendto");
        }
}

void chat_srv(int sock)
{
        struct sockaddr_in cliaddr;
        socklen_t clilen;
        int n;
        MESSAGE msg;
        while (1) {
                memset(&msg, 0, sizeof(msg));
                clilen = sizeof(cliaddr);
                n = recvfrom(sock, &msg, sizeof(msg), 0, (struct sockaddr *) &cliaddr, &clilen);
                if (n < 0) {
                        if (errno == EINTR)
                                continue;
                        ERR_EXIT("recvfrom");
                }

                int cmd = ntohl(msg.cmd);
                switch(cmd) {
                case C2S_LOGIN:
                        do_login(msg, sock, &cliaddr);
                        break;
                case C2S_LOGOUT:
                        do_logout(msg, sock, &cliaddr);
                        break;
                case C2S_ONLINE_USER:
                        do_sendlist(sock, &cliaddr);
                        break;
                default:
                        break;
                }
        }
}

void do_sendlist(int sock, struct sockaddr_in *cliaddr)
{
        MESSAGE msg;
        msg.cmd = htonl(S2C_ONLINE_USER);
        sendto(sock, (char *) &msg, sizeof(msg), 0, (struct sockaddr *) cliaddr, sizeof(*cliaddr));
        
        int count = htonl((int) client_list.size());
        sendto(sock, (char *) &count, sizeof(count), 0, (struct sockaddr *) cliaddr, sizeof(*cliaddr));

        for (USER_LIST::iterator it = client_list.begin(); it != client_list.end(); ++it) {
                sendto(sock, &*it, sizeof(*it), 0, (struct sockaddr *) cliaddr, sizeof(*cliaddr));
        }
}

int main(int argc, char *argv[])
{
        int sock;
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
                ERR_EXIT("socket");

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        int ret = bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));
        if (ret < 0)
                ERR_EXIT("bind");

        chat_srv(sock);

        return 0;
}
