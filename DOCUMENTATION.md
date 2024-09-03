# `ft_malcolm` documentation

## Parsing

lame. i'm not talking about this.

## Initialize the socket

Next, our goal is to listen to communications on a specific interface.

Sockets allow programs to send and receive data over a network. In our case, we're using sockets to both capture incoming ARP packets and send crafted ARP packets.

1. Open a socket that is suitable for ARP requests.
   ```c
   // AF_PACKET - address family
   // SOCK_RAW - raw packets
   // ETH_P_ARP - ethernet protocol type for ARP (0x0806)
   socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))
   ```
2. Find an available and suitable interface. See `getifaddrs`, `freeifaddrs`
   ```c
   IFF_UP          // interface is up
   IFF_LOOPBACK    // is (not) a loopback net
   IFF_RUNNING     // has resources allocated
   AF_INET         // internetwork: UDP, TCP, etc.
   ```
3. Bind the socket, so that the socket only receives packets from the chosen interface, and only receives ARP packets. Without binding, the socket would receive ARP packets from all interfaces.

## Processing packets

Once everything is ready for listening, we can start intercepting packets ! But not every packets. From the subject : "Your program will have to wait for an **ARP request** sent on the **broadcast** by the **target**, requesting the **source IP**."

```c
if (is_broadcast_request(&malcolm->packet) &&
    is_request(&malcolm->packet) &&
    is_from_target(malcolm, &malcolm->packet) &&
    is_requesting_source(malcolm, &malcolm->packet))
```

If we hear a packet that passes through these filters, it means that we intercepted an arp request from the target requesting the source. We can finally send a spoofed arp reply !

## Crafting the spoofed ARP reply

> Now is a good time to learn about `wireshark` and `ettercap`. Basic example in [this](https://www.youtube.com/watch?v=A7nih6SANYs) video.

### Structure of an arp packet

RFC describes well the structure of an arp packet. If follows this structure :

1. The Ethernet Header

```c
// /usr/include/linux/if_ether.h

struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	__be16		h_proto;		/* packet type ID field	*/
} __attribute__((packed));
```

2. The ARP Header

```c
// /usr/include/linux/if_arp.h

struct arphdr {
    __be16		ar_hrd;		/* format of hardware address	*/
    __be16		ar_pro;		/* format of protocol address	*/
    unsigned char	ar_hln;		/* length of hardware address	*/
    unsigned char	ar_pln;		/* length of protocol address	*/
    __be16		ar_op;		/* ARP opcode (command)		*/

#if 0
    /*
    *	 Ethernet looks like this : This bit is variable sized however...
    */
    unsigned char		ar_sha[ETH_ALEN];	/* sender hardware address	*/
    unsigned char		ar_sip[4];		/* sender IP address		*/
    unsigned char		ar_tha[ETH_ALEN];	/* target hardware address	*/
    unsigned char		ar_tip[4];		/* target IP address		*/
#endif

};
```

### Padding matters

ARP packets are transmitted within Ethernet frames. Ethernet frames have a minimum size of 64 bytes.

- 14 bytes for the Ethernet header
- 46 bytes for the payload (minimum)
- 4 bytes for the Frame Check Sequence (FCS)

When crafting an ARP Reply, we create a 14-byte Ethernet header and a 28-byte ARP packet. That adds up to 42 bytes, which doesn't meet the requirements to fit within an ethernet frame.

So to have a 64 bytes packet, we need to add padding. The padding should bring the total frame size (excluding the FCS) to 60 bytes, allowing the 4-byte FCS to complete the 64-byte minimum.

```c
char buffer[ETH_FRAME_MIN];

memset(buffer, 0, ETH_FRAME_MIN);
memcpy(buffer, spoofed_packet.eth, sizeof(struct ethhdr));
memcpy(buffer + sizeof(struct ethhdr), spoofed_packet.arp, sizeof(t_arphdr));
```

Boom ! This should be all to send the spoofed packet to the target. The arp cache of the target should be updated !!

What did we achieve ? Now, the target believes that the mac address (`source_mac`) of the given ip (`source_ip`) is `source_ip`, even though it's not !

![congrats.jpg](./assets/congrats.jpg)
