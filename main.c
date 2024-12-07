#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include "client.h"

int main() {
    //definire stucturi pentru adrese;
    struct sockaddr_in adresa_mea, adresa_client;
    memset(&adresa_mea, 0, sizeof(adresa_mea));

    adresa_mea.sin_family = AF_INET; //ipv4
    adresa_mea.sin_port = htons(22116); //portul echipei
    adresa_mea.sin_addr.s_addr = INADDR_ANY; //accesibil pe oricare dintre adrese

    signal(SIGCHLD, SIG_IGN); // Ignora procesele zombie

    //socker port server
    int socket_server = socket(AF_INET, SOCK_STREAM, 0); //ipv4, HTTP
    if (socket_server < 0) {
        printf("Eroare la crearea socket-ului\n");
        return 1;
    }
    //bind
    if (bind(socket_server, (struct sockaddr*)&adresa_mea, sizeof(adresa_mea)) < 0) {
        printf("Bind error\n");
        close(socket_server);
        return 1;
    }
    //listen pe socket
    if (listen(socket_server, 10) < 0) {
        printf("Eroare la ascultare\n");
        close(socket_server);
        return 1;
    }
    //loop care asteapta conexiuni
    while (1) {
        printf("Astept conexiuni...\n");
	//socket client
        socklen_t lungime_client = sizeof(adresa_client);
        int socket_client = accept(socket_server, (struct sockaddr*)&adresa_client, &lungime_client);
        
        if (socket_client < 0) {
            printf("Eroare la conectare ipv4\n");
            continue;
        }

        printf("Conexiune acceptata de la: %s\n", inet_ntoa(adresa_client.sin_addr));

        pid_t pid = fork();
        if (pid < 0) {
            printf("Eroare la fork\n");
            close(socket_client);
            continue;
        } else if (pid == 0) {
            close(socket_server);
	//loop apelare client
            while (1) {
                char buffer[4096] = {0};
                ssize_t primit = recv(socket_client, buffer, sizeof(buffer) - 1, 0);

                if (primit <= 0) {
                    printf("Client deconectat sau eroare\n");
                    close(socket_client);
                    exit(0);
                }

           //verificare functie
                if (strcmp(buffer, "02#") == 0) {
                    send(socket_client, "Comanda #02 acceptata!\n", 23, 0);
                    client(socket_client); // apeleaza functia client pentru a primi HTML
                } else {
                    send(socket_client, "Comanda neimplementata!\n", 23, 0);
                }
            }
        } else {
            close(socket_client);
        }
    }

    close(socket_server);
    return 0;
}

