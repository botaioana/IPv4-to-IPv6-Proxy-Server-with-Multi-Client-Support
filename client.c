#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_SERVER 80

int client(int socket_client_ipv4) {
    int socket_fd;
    struct sockaddr_in6 adresa_server;
    const char *nume_gazda = "::1";

    // Creare socket
    socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        printf("Eroare la crearea socket-ului\n");
        send(socket_client_ipv4, "Eroare: Nu se poate crea socket-ul.\n", 36, 0);
        return 1;
    }

    // Configurare adresa server
    memset(&adresa_server, 0, sizeof(adresa_server)); 
    adresa_server.sin6_family = AF_INET6;
    adresa_server.sin6_port = htons(PORT_SERVER);
    if (inet_pton(AF_INET6, nume_gazda, &adresa_server.sin6_addr) <= 0) {
        printf("Adresa IPv6 este invalida\n");
        close(socket_fd);
        send(socket_client_ipv4, "Eroare: Adresa IPv6 invalida.\n", 30, 0);
        return 1;
    }

    // Conectare la server
    if (connect(socket_fd, (struct sockaddr *)&adresa_server, sizeof(adresa_server)) < 0) {
        printf("Eroare la conectare\n");
        close(socket_fd);
        send(socket_client_ipv4, "Eroare: Nu se poate conecta la server.\n", 40, 0);
        return 1;
    }

    // Trimitere cerere HTTP/1.0
    const char *cerere = "GET / HTTP/1.0\r\nHost: [::1]\r\n\r\n";
    if (send(socket_fd, cerere, strlen(cerere), 0) == -1) {
        printf("Eroare la trimiterea cererii\n");
        close(socket_fd);
        send(socket_client_ipv4, "Eroare: Cererea nu a putut fi trimisa.\n", 38, 0);
        return 1;
    }

    // Receptionare raspuns si scriere in fisier
    char raspuns[4096];
    ssize_t lungime;
    FILE *fisier = fopen("site.html", "wb");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului\n");
        close(socket_fd);
        send(socket_client_ipv4, "Eroare: Nu se poate deschide fisierul.\n", 39, 0);
        return 1;
    }

    while ((lungime = recv(socket_fd, raspuns, sizeof(raspuns), 0)) > 0) {
        fwrite(raspuns, 1, lungime, fisier);
        send(socket_client_ipv4, raspuns, lungime, 0);
    	printf("%s\n", raspuns);
    }

    if (lungime < 0) {
        printf("Eroare la receptionarea datelor\n");
        send(socket_client_ipv4, "Eroare: receptionarea datelor esuata.\n", 42, 0);
    } else {
        printf("Clientul a inchis conexiunea\n");
    }

    fclose(fisier);
    close(socket_fd);
    return 0;
}

