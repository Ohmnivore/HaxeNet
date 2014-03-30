#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <enet/enet.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#define IMPLEMENT_API
#include <hx/CFFI.h>

DEFINE_KIND(k_host);
DEFINE_KIND(k_peer);
DEFINE_KIND(k_event);

typedef enet_uint16 ENetPeerHandle;

static value enet_init(void)
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

    return alloc_int(result);
}

/**
        socket_bind : host : int32 -> port:int -> connections:int -> incoming:int32 -> outgoing:int32 -> bool
        <doc>Bind a UDPR socket for server usage on the given host and port, with max connections,
        incoming bandwidth limited to bytes per second or 0 for unlimited, outgoing also. Host may be
        val_type_null, in which case the binding is to ENET_HOST_ANY
        </doc>
**/
static value enet_create_server( value ip, value port, value channels, value connections)
{
        val_check(connections, int);
        val_check(channels, int);
        val_check(port, int);
        if (!val_is_null(ip))
            val_check(ip, string);

        ENetAddress address;
        ENetHost *s;

        address.port = (enet_uint16)val_int(port);
        if (!val_is_null(ip))
            address.host = enet_address_set_host(& address, val_string(ip));
        else
            address.host = ENET_HOST_ANY;

        s = enet_host_create(   &address,
                                (size_t)val_int(connections),
                                (size_t)val_int(channels),           /* number of clients and/or outgoing connections */
                                0,       /* amount of incoming bandwidth in Bytes/sec */
                                0);
        if(s == NULL)
            fprintf (stderr, "An error occurred while trying to create an ENet server host.\n");
        else
            printf("Created server.\n");
        value v = alloc_abstract(k_host, s);
        return v;
}

static value enet_create_client( value Ip, value Port, value Channels, value Connections)
{
        val_check(Connections, int);
        val_check(Channels, int);
        val_check(Port, int);
        val_check(Ip, string);

        size_t connections = (size_t)val_int(Connections);
        size_t channels = (size_t)val_int(Channels);
        enet_uint16 port = (enet_uint16)val_int(Port);
        const char* ip = (const char*)val_string(Ip);

        ENetHost * host=0;
        ENetPeer * peer = 0;
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

        value v = alloc_abstract(k_host, host);
        return v;
}

static value enet_poll(value h, value timeout)
{
    val_check(timeout, number);
    val_is_kind(h, k_host);

    ENetEvent *event;
    int res;

    enet_uint32 tout = (enet_uint32)(val_number(timeout)*1000);

    event = (ENetEvent *) enet_malloc (sizeof (ENetEvent));

    // Wait up to timeout milliseconds for an event.

    res = enet_host_service ((ENetHost *)val_data(h), event, tout);

    if (event->type == ENET_EVENT_TYPE_CONNECT)
        printf("Connected!\n");
    if (event->type == ENET_EVENT_TYPE_DISCONNECT)
        printf("Disconnected!\n");

    return alloc_abstract(k_event, event);
}

static value enet_event_type(value e)
{
    val_is_kind(e, k_event);

    ENetEvent *event;

    event = (ENetEvent *)val_data(e);

    return alloc_int(event->type);
}

static value enet_event_channel(value e)
{
    val_is_kind(e, k_event);

    ENetEvent *event;
    enet_uint8 ch;

    event = (ENetEvent *)val_data(e);
    ch = event->channelID;

    if (ch == NULL)
        return alloc_int(-1);
    else
        return alloc_int(event->channelID);
}

static value enet_event_message(value e)
{
    val_is_kind(e, k_event);

    ENetEvent *event;
    unsigned char *str;

    event = (ENetEvent *)val_data(e);

    std::stringstream ss;
    ss << event->packet->data;

    return alloc_string(ss.str().c_str());
}

static value enet_event_peer(value e)
{
    val_is_kind(e, k_event);

    ENetEvent *event;
    event = (ENetEvent *)val_data(e);

    std::stringstream ss;
    ss << event-> peer-> address.host << ":" << event-> peer-> address.port;

    return alloc_string(ss.str().c_str());
}

static value enet_disconnect_peer( value host, value addnport, value force )
{
    val_check(addnport, string);
    val_check(force, bool);
    val_is_kind(host, k_host);

    ENetPeer * peerx;
    ENetHost * h = (ENetHost *)val_data(host);

    for ( int count = 0; count < h->peerCount; count++)
    {
        peerx = & (h->peers[count]);

        std::stringstream ss;
        ss << peerx->address.host << ":" << peerx->address.port;

        if (ss.str() == val_string(addnport))
        {
            if (val_bool(force) == true)
            {
                enet_peer_reset (peerx);
            }

            else
            {
                enet_peer_disconnect (peerx, 0);
            }
        }
    }
}

static value enet_send_packet( value host, value addnport, value Channel, value content, value flags )
{
    val_check(addnport, string);
    val_check(Channel, int);
    val_check(content, string);
    val_check(flags, int);
    val_is_kind(host, k_host);

    ENetPeer * peerx;
    ENetHost * h = (ENetHost *)val_data(host);

    const char* msg = val_string(content);
    enet_uint8 channel = (enet_uint8)val_int(Channel);

    enet_uint32 fl = (enet_uint32)val_int(flags);

    ENetPacket * packet = enet_packet_create (
                                                msg,
                                                strlen (msg) + 1,
                                                fl);

    for ( int count = 0; count < h->peerCount; count++)
    {
        peerx = & (h->peers[count]);

        std::stringstream ss;
        ss << peerx->address.host << ":" << peerx->address.port;

        if (ss.str() == val_string(addnport))
        {
            enet_peer_send (peerx, channel, packet);

            return(alloc_int(peerx->roundTripTime));
        }
    }
}

static void enet_destroy_event( value e )
{
    val_is_kind(e, k_event);

    ENetEvent *event;
    event = (ENetEvent *)val_data(e);

    if(event->packet != NULL)
        enet_packet_destroy (event->packet);
}

static value enet_send_oob(value h, value ip, value port, value data)
{
        val_check_kind(h, k_host);
        val_check(data, string);
        val_check(port, int);
        val_check(ip, string);

        ENetAddress address;
        ENetHost *host;
        ENetBuffer buf;

        host = (ENetHost *)val_data(h);

        address.port = (enet_uint16)val_int(port);
        enet_address_set_host(&address, val_string(ip));

        buf.data = (char*)val_string(data);
        buf.dataLength = val_strlen(data);

        enet_socket_send (host->socket, &address, &buf, 1);
        return val_true;
}

DEFINE_PRIM(enet_init, 0);
DEFINE_PRIM(enet_create_server, 4);
DEFINE_PRIM(enet_create_client, 4);
DEFINE_PRIM(enet_poll, 2);

DEFINE_PRIM(enet_event_type, 1);
DEFINE_PRIM(enet_event_channel, 1);
DEFINE_PRIM(enet_event_message, 1);
DEFINE_PRIM(enet_event_peer, 1);
DEFINE_PRIM(enet_destroy_event, 1);

DEFINE_PRIM(enet_send_packet, 5);
DEFINE_PRIM(enet_send_oob, 4);
DEFINE_PRIM(enet_disconnect_peer, 3);
