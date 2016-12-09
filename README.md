# README #

## Communications Node ##
Write a simple base "Communications Node" (CN) for a distributed communications system
running across multiple machines on a mixed (fast, slow, wired, WiFi, etc...) "local" network. In
its most basic form, each CN should have a unique ID and should come up and continuously
enumerate all the other nodes it can see on its network:

* Update every 10-20 seconds
* List all other CNs on the reachable LAN (don’t worry about firewalls, NAT, etc…)
* Show round trip comms latency to every other node
* Show an estimate of "to" and "from" bandwidth with each other CN
* Implementation should allow an arbitrary number of CNs
* Implementation should allow multiple CNs to smoothly coexist on one machine

C or C++ with standard TCP and UDP socket calls is probably going to be the fastest route to
success on this task but we will accept alternative approaches.
The best solution will not require manual setting of IP addresses, port numbers, or node IDs --
these should be set automatically.

If time allows, implement some additional interesting features. For example:

* Provide statistics over time on performance, latencies, etc...
* Have nodes share connectivity information - find partially reachable nodes?
* Display a graphical map of node connectivity, link performance (ncurses?)
* Allow remote control, shutdown of nodes.
* Think of other fun/creative features!?

Test your code, run it on a variety of machines, and provide an example listing of the output