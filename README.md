# ChaCha20
**ChaCha20 DCHP Message multicast sender**

A small program that can be compiled and ran on an openwrt router. Once configured it will send encrypted udp messages to the broadcast address with DHCP updates. 

## usage
chacha20 <operation> <mac> <ip> <hostname> <br />

Command Line options: <br />
--help show this help text <br />
--testMulticast send a multicast test message to configured address:port <br />
--testVectors Run a test with standard vectors & display the internals <br />
--testMessage Generate a test message <br />
