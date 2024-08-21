Display the detected interface and run the program

/* Get the list of all network interfaces and find an active one
with an assigned IP address, which will be the interface we can use */

getifaddrs, freeifaddrs

/*
    Create a raw socket using socket to capture ARP packets on the network interface.
    Use a loop to continuously read packets from the socket using recvfrom.
    Parse the received packets to extract the necessary information, such as IP and MAC addresses.
    Compare the source IP and MAC addresses of each packet with the desired values.
    Process the packets that match the desired IP and MAC addresses
*/

    // Create raw socket for capturing ARP packets,
	// and save the file descriptor

En fait c'est simple on a 3 agents :
- Une sorte de serveur qui ecoute les requetes sur un reseau. Il va se concentrer sur les requetes ARP. Il doit ecouter sur la bonne interface
- 2 clients qui communiquent entre eux sur une interface

Configuration actuelle : une machine virtuelle qui fait tourner le programme (qui ecoute les requetes)

1. Comment est-ce que 2 clients peuvent communiquer ?
2. En ce qui concerne le serveur, je trouve complique de chercher dans tous les .h et les manuels ! Donne moi un exemple de :
    - Quels flags utiliser pour socket pour ouvrir un raw socket qui permet d'ecouter des requetes ARP ? 
    - Donne moi un exemple simple pour capturer les communications sur une interface (getifaddrs)

