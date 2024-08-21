# `ft_malcolm`

Usage

```sh
./ft_malcolm [source_ip] [source_mac] [target_ip] [target_mac]
```

- `source_ip` : l'adresse IP qu'on va imiter **(l'imposteur)**
- `source_mac` : l'adresse mac qu'on pretend avoir
- `target_ip` : l'adresse IP de la machine qu'on veut tromper **(la cible)**
- `target_mac` : l'adresse MAC de cette machine cible

Example
```sh
./ft_malcolm "2.2.2.2" "22:22:22:22:22:22" "1.1.1.1" "11:11:11:11:11:11"
```

```sh
sudo ./ft_malcolm 10.12.255.255 ff:bb:ff:ff:ee:ff 10.12.10.22 10:dd:b1:ff:ff:ff
```

Le programme se met à l'écoute sur le réseau. Il attend que la machine cible pose une question spécifique : "Qui a cette adresse IP que je cherche ?"

Quand la bonne question est posée, notre programme répond en mentant : "C'est moi qui ai cette adresse IP, et voici mon adresse MAC". Mais en réalité, ce n'est pas la vraie adresse MAC !

La machine cible croit cette fausse information et la sauvegarde.

Résultat : la cible pense communiquer avec la vraie machine, mais en fait, elle parle à notre imposteur.

## Notes pour trop tard

Warning ! `handle_error` returns an integer and does not exit (for now)

## Useful doc

RFC 826 defines the ARP protocol and its operation.

RFC 7042 discusses the security considerations of ARP, including ARP spoofing.

`arping` - send ARP ping to host
`netdiscover` - get info about network
`ip a` / `ip link show` - get info about network interaces

## ARP what ?

ARP (Address Resolution Protocol) is used to find a MAC address using an IP address within a local network.

ARP operates at the link layer (Layer 2) of the OSI model, which means it works within a single network segment.

The requesting host **broadcasts** an **ARP request** to all devices on the local network. The request essentially says, "Who has IP address 10.1.56.128? Tell 10.1.56.100". Only the host with the matching IP address responds directly to the requester with its MAC address.

## Vulnerability 

No Authenticity Checking: ARP doesn't verify if the sender of an ARP reply is actually the legitimate owner of the IP address.

Trust in Replies: Hosts implicitly trust and update their ARP cache based on any ARP reply they receive, even unsolicited ones : trust is assumed without verification.

Cache Overwriting: New ARP information overwrites old entries without verification.

### Man-in-the-Middle (MitM) attack

A MitM attack is when an attacker inserts themselves between two communicating parties, intercepting and potentially altering the communication without the knowledge of the original parties.

## Real-World example (using namespaces)

Simulating two clients on the same VM using network namespaces.

A network namespace is a logical copy of the network stack from the host system. Network namespaces are useful for setting up containers or virtual environments. Each namespace has its own IP addresses, network interfaces, routing tables, and so forth.

### Packages

```sh
sudo apt-get install iproute2 net-tools tcpdump
```

### Setup

```sh
# Create two network namespaces
sudo ip netns add client1
sudo ip netns add client2

# Create a virtual ethernet pair
sudo ip link add veth1 type veth peer name veth2

# Move each veth to its namespace
sudo ip link set veth1 netns client1
sudo ip link set veth2 netns client2

# Configure IP addresses
sudo ip netns exec client1 ip addr add 192.168.1.1/24 dev veth1
sudo ip netns exec client2 ip addr add 192.168.1.2/24 dev veth2

# Bring up the interfaces
sudo ip netns exec client1 ip link set veth1 up
sudo ip netns exec client2 ip link set veth2 up
```

```sh
# Get IP / MAC address of a client
sudo ip netns exec client1 ip a
```

### Listening to communications

`tcpdump` can be used to capture and analyze the network traffic, including ARP requests and responses.

```sh
# In one terminal, start capturing on veth1
sudo ip netns exec client1 tcpdump -i veth1 -n arp

# In another terminal, ping from client1 to client2
sudo ip netns exec client1 ping 192.168.1.2
```

**Example**

```sh
# Listen ARP requests/responses on veth1
sudo ip netns exec client1 tcpdump -i veth1 -n arp -v
```

```sh
# Ping client 2
sudo ip netns exec client1 ping 192.168.1.2 -c 4
```

```sh
# Output
tcpdump: listening on veth1, link-type EN10MB (Ethernet), snapshot length 262144 bytes
16:05:18.107461 ARP, Ethernet (len 6), IPv4 (len 4), Request who-has 192.168.1.2 tell 192.168.1.1, length 28
16:05:18.107484 ARP, Ethernet (len 6), IPv4 (len 4), Request who-has 192.168.1.1 tell 192.168.1.2, length 28
16:05:18.107491 ARP, Ethernet (len 6), IPv4 (len 4), Reply 192.168.1.1 is-at 66:1b:01:8f:79:a5, length 28
16:05:18.107495 ARP, Ethernet (len 6), IPv4 (len 4), Reply 192.168.1.2 is-at 12:63:52:20:d0:f3, length 28

4 packets captured
4 packets received by filter
0 packets dropped by kernel
```

## Listening to ARP requests

## Creating ARP packets

```txt
Ethernet Header
  |- Destination MAC Address
  |- Source MAC Address
  |- EtherType (ARP: 0x0806)

ARP Header
  |- Hardware Type (Ethernet: 1)
  |- Protocol Type (IPv4: 0x0800)
  |- Hardware Address Length (MAC address length: 6)
  |- Protocol Address Length (IPv4 address length: 4)
  |- Operation (ARP Reply: 2)
  |- Sender MAC Address
  |- Sender IP Address
  |- Target MAC Address
  |- Target IP Address

Payload (if any)
```
