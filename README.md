Omnet++ code to simulate a simple rate controller for BFT broadcast.

New heuristic: utility scores only increase. The leader chooses the next intermediate node(s) for broadcast in a weighted round-robin (weigthed-rr) fashion, proportional to the utility scores. No probe messages are passed around, and AIMD relies only on ACK sequence numbers. 
