#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <enet/enet.h>
#include <string>
#include <windows.h>
#include <sstream>

#define IMPLEMENT_API
#include <hx/CFFI.h>

ENetPeer * peer = 0;

static int enet_init(void)
{
    int result = enet_initialize();

    if (result != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
    }
    else
    {
        printf ("Initialized ENet.\n");
    }
    atexit (enet_deinitialize);

    return result;
}

/**
        socket_bind : host : int32 -> port:int -> connections:int -> incoming:int32 -> outgoing:int32 -> bool
        <doc>Bind a UDPR socket for server usage on the given host and port, with max connections,
        incoming bandwidth limited to bytes per second or 0 for unlimited, outgoing also. Host may be
        val_type_null, in which case the binding is to ENET_HOST_ANY
        </doc>
**/
static ENetHost* enet_create_server(const char* ip, enet_uint16 port, size_t channels, size_t connections) {
        ENetAddress address;
        ENetHost *s;

        if (ip != NULL && ip != 0)
            address.host = enet_address_set_host(& address, ip);
        else
            address.host = ENET_HOST_ANY;
        address.host = ENET_HOST_ANY;
        address.port = port;

        s = enet_host_create(   &address,
                                connections,
                                channels,           /* number of clients and/or outgoing connections */
                                0,       /* amount of incoming bandwidth in Bytes/sec */
                                0);
        if(s == NULL)
            fprintf (stderr, "An error occurred while trying to create an ENet server host.\n");
        else
            printf("Created server.\n");

        return s;
}

static ENetHost* enet_create_client(const char* ip, enet_uint16 port, size_t channels, size_t connections)
{
        ENetHost * host=0;
        ENetAddress address;

        address.port = port;

        host = enet_host_create (0, // create a client host
                connections, // allow only 1 outgoing connection
                channels,
                0, // use 57600 / 8 for 56K modem with 56 Kbps downstream bandwidth
                0);// use 14400 / 8 for 56K modem with 14 Kbps upstream bandwidth

        if (!host) {
            fprintf(stderr,"An error occurred while trying to create an ENet client host.\n");
            exit (EXIT_FAILURE);
        }

        // connect to server:
        enet_address_set_host (&address, ip);
        peer = enet_host_connect (host, &address, channels, 0);
        peer->data=0; // use this as mark that connection is not yet acknowledged
        if (!peer) {
           fprintf(stderr,"No available peers for initiating an ENet connection.\n");
        }

        return host;
}

static void SendPacket(int Channel, char pack[11])
{
    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket * packet = enet_packet_create (pack,
                                              strlen (pack) + 1,
                                              ENET_PACKET_FLAG_RELIABLE);


    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by         */
    /* enet_host_broadcast (host, 0, packet);         */
    enet_peer_send (peer, Channel, packet);
    /* One could just use enet_host_service() instead. */
    //enet_host_flush (host);

}

static DWORD WINAPI poll(void* h)
{
    ENetHost * host = (ENetHost*)h;
    ENetEvent event;
    //event = (ENetEvent *) enet_malloc (sizeof (ENetEvent));

    while (1)
    {
        enet_host_service (host, &event, 1000);

        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
            std::stringstream ss;
            ss << event.peer-> address.host << ":" << event.peer-> address.port << "\n";

            printf(ss.str().c_str());

            fflush(stdout);
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE:

                printf("%s says %s on channel %u\n",
                    (char*)event.peer -> data,event.packet -> data,event.channelID);
                fflush(stdout);

            enet_packet_destroy (event.packet); // clean up the packet now that we're done using it
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("  host disconnected.\n");
            fflush(stdout);
        default:
            break;
        }

        //Sleep(10);
    }

    return 0;
}

static void poll2(ENetHost * host)
{
    ENetEvent event;

    enet_host_service (host, &event, 1000);

    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT: {
        printf("  A new connected has been established to %u:%u\n",
            event.peer -> address.host,event.peer-> address.port);
        fflush(stdout);
        break;
    }
    case ENET_EVENT_TYPE_RECEIVE:

            printf("%s says %s on channel %u\n",
                (char*)event.peer -> data,event.packet -> data,event.channelID);
            fflush(stdout);

        enet_packet_destroy (event.packet); // clean up the packet now that we're done using it
        break;

    case ENET_EVENT_TYPE_DISCONNECT:
        printf("  host disconnected.\n");
        fflush(stdout);
    default:
        break;
    }
}

static value enet_send_packet( ENetHost * h, const char* address, int port, enet_uint8 channel, const char* content )
{
    ENetPeer * peerx;

    ENetPacket * packet = enet_packet_create (
                                                content,
                                                strlen (content) + 1,
                                                ENET_PACKET_FLAG_RELIABLE);

    //for ( int count = 0; count < h->peerCount; count++)
    //{
        std::stringstream ss;
        ss << h->peerCount;

        printf(ss.str().c_str());

        peerx = & (h->peers[0]);
        enet_peer_send (peerx, channel, packet);
    //}

    enet_peer_send (peer, channel, packet);

    return 0;
}

int main()
{
    enet_init();

    ENetHost *server = enet_create_server(0, 1234, 1, 1);

    DWORD threadDescriptor;
    CreateThread(
        NULL,                   /* default security attributes.   */
        0,                      /* use default stack size.        */
        poll,          /* thread function name.          */
        server,        /* argument to thread function.   */
        0,                      /* use default creation flags.    */
        &threadDescriptor);     /* returns the thread identifier. */
    Sleep(1000);
    printf("\n");


    ENetHost *client = enet_create_client("", 1234, 1, 1);
    CreateThread(
        NULL,                   /* default security attributes.   */
        0,                      /* use default stack size.        */
        poll,          /* thread function name.          */
        client,        /* argument to thread function.   */
        0,                      /* use default creation flags.    */
        &threadDescriptor);     /* returns the thread identifier. */

    getch();
    //SendPacket(0,"Hello");
    enet_send_packet(client, "", 0, 0, "Hello there");
    getch();

    return 0;
}
