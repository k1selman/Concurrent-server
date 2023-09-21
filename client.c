#include <stdio.h>
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

    /* Construct a client */
    ENetHost *client;

    /*
    enet_host_create parameters: ENetAddress *address, size_t peerCount, size_t channelLimit, enet_uint32 incomingBandwith, enet_uint32 outgoingBandwith
    NULL as an address creates a client host,
    1 as peerCount allows only 1 outgoing connection
    1 as channelLimit allows 1 channel to be used
    0 as incomingBandwith assumes any amount of incoming bandwith
    0 as outgoingBandwith assumes any amount of outcoming bandwith
    */
    client = enet_host_create(NULL, 1, 1, 0, 0);

    /* enet_host_create returns NULL on failure*/
    if(client == NULL)
    {
        fprintf(stderr, "An error occured while trying to create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;

    /* Connect to HereGoesTheServer:1234  */
    enet_address_set_host(&address, "HereGoesTheServer:1234");
    address.port = 1234;

    /* 
    Initialize the connection and allocate the channel 
    
    enet_host_connect parameters: ENetHost *host, const ENetAddress *address, size_t channelCount, enet_uint32 data
    host seeks the connection,
    address is the connection destination,
    channelCount is the number of channels to allocate,
    data is user data supplied to the receiving host
    
    */
    peer = enet_host_connect(client, &address, 1, 0);

    if(peer == NULL)
    {
        fprintf(stderr, "No available peers for initiating an ENet connection.\n");
    }

    /* Wait for 5 seconds for the connection attemp to succed */
    if(enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts("Connection to HereGoesTheServer:1234 succeeded.\n");
    }
    else
    {
    /* 
    Either the 5 seconds are up, or disconnect event was received.
    Reset the peer in the event 5 seconds had run out without any significant event.
    */
        enet_peer_reset(peer);
        puts("Connection to HereGoesTheServer:1234 failed.\n");
    }
}