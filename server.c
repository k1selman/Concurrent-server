#include <stdio.h>
#include <stdbool.h>
#include <enet/enet.h>

int main(int argc, char **argv)
{
    /* Before using ENet, we have to initialize the library */
    if(enet_initialize() != 0)
    {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    /* When we exit the program, deinitialize ENet */
    atexit(enet_deinitialize);

    ENetEvent event;
    ENetAddress address;
    ENetHost *server;

    /*
    Bind server to default host.
    We can specify a host by enet_address_set_host(&address, "1.2.3.4");
    */
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    /*
    enet_host_create takes 5 parameters:
    address to bind the server host to,
    3 allows up to 3 clients and/or outgoing connection,
    1 allows up to 1 channel to be used,
    0 assumes any amount of incoming bandwith,
    0 assumes any amount of outcoming bandwith,
    */
    server = enet_host_create(&address, 3, 1, 0, 0);

    if(server == NULL)
    {
        fprintf(stderr, "An error occured while trying to connec to  ENet server host.\n");
        exit(EXIT_FAILURE);
    }

    /* Game loop */
    while(true)
    {
        ENetEvent event;
        /* Wait for 1 second for an event */
        while(enet_host_service(server, &event, 1000) > 0)
        {
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A new client connected from %x:%u.\n",
                    event.peer -> address.host,
                    event.peer -> address.port);
                break;
                case ENET_EVENT_TYPE_RECEIVE:
                    printf("A packet of length %u containing %s was received from %s on channel %u. \n",
                    event.packet -> dataLength,
                    event.packet -> data,
                    event.peer -> data,
                    event.channelID);
                    /* Quick packet cleanup since we are not using it anymore*/
                    enet_packet_destroy(event.packet);
                break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%s disconnected.\n", event.peer -> data);
                    /* Reset the peer's client info. */
                    event.peer -> data = NULL;
            }
        }
    }

    enet_host_destroy(server);

    return 0;
}
