CS401
DP Circuit-Tree
Shane Lopez


Unfortunately I was unable to complete my implementation on time, though within my .cpp I have left comments (nearer the bottom)
explaining my thought process and the way in which my algorithm is intended to work. I believe that if I had had just a little 
more time, I would have gotten it to work, although in truth I am not so sure my algorithm is optimal; this fact stems, I believe, 
from the lack of comparison of aggregated delays with respect to both neighboring inputs; I check to see if the left delay is within a 
certain range, act if it is, and proceed to update my info and evaluate the right neighbor in the same fashion. I have a sneaking 
suspicion that it would be better advised to evaluate both neighbors before making a gate coupling, but as mentioned, time does not 
allow for those tweaks at present. However, I hope that my comments and coding will count for something! :D


Building
	$ make


Running
	$./gtree <input file> <gate delay>

In Essence

	Say gtree starts with:

		3
		1              <--- I0
		4	       <--- I1
		3	       <--- I2
	
	in the input file fed to it. It will scan through, the file and parse that data, here corresponding to 3 inputs, 
	with arrival times 1, 4, and 3 respectively. Given also a delay value of 1 for simplicity, gtree will scan through
	the arrival times looking for values within +/- 1 of eachother (they must be direct neighbors), and pairing those, 
	the idea being that smaller differences in pairings will yield smaller delay;

	After the first iteration, gtree will have paired I1 and I2, like so:

		1
		
		4 ---
		      > 5
		3 ---

	
	After a few more iterations with an increasing range, it will pair I0 and the pairing {I1, I2}:
	
		1 -----------
		              \
		4 ---         / 6
		      > 5 ---
		3 ---
	

	and the final tree, as well as it's delay, will be reported. As can be seen with n inputs it 
	takes n-2 pairings (= 1 in this case) to complete the tree, as only two entities will remain 
	to be linked by a node.
	
	
	 
