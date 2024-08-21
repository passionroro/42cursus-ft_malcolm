#!/bin/bash

set -e

if [[ $EUID > 0 ]]
    then echo "root privileges required. exiting."
fi

# namespaces creation
ip netns add client1
ip netns add client2

# bridge interface
ip link add name br0 type bridge
ip link set br0 up

# connecter enp0s3 to bridge
ip link set enp0s3 master br0

# Créer les paires veth
ip link add veth1 type veth peer name veth1_bridged
ip link add veth2 type veth peer name veth2_bridged

# Connecter un côté des veth au bridge
ip link set veth1_bridged master br0
ip link set veth2_bridged master br0

# Activer les interfaces veth côté bridge
ip link set veth1_bridged up
ip link set veth2_bridged up

ip link set veth1 netns client1
ip link set veth2 netns client2

ip netns exec client1 ip addr add 192.168.1.101/24 dev veth1
ip netns exec client2 ip addr add 192.168.1.102/24 dev veth2

ip netns exec client1 ip link set veth1 up
ip netns exec client2 ip link set veth2 up

echo "done."

# exemple: depuis client1, ping client2
# sudo ip netns exec client1 ping 192.168.1.102

# capturer les paquets ARP :
# sur l'hôte
#   sudo tcpdump -i br0 -n arp

# ou dans un espace de noms client
#   sudo ip netns exec client1 tcpdump -i veth1 -n arp