# langtons_ant.c
<p>An implementation of Langton's Ant using the C 2018 standard.
</p>

### Some properties of Langton's Ant
<p>
	<ol>
		<li> 
		The Ant is denoted by a '*' char that is analogous to a read/write head.
		</li>
		<li> 
		Memory cells are represented by a two-dimensional char array. Each cell is like a square on a chessboard. Each cell can either contain a ' ', or a '#'. We call these two states "black" and "white." 
		</li>
		<li>
		The Ant can move forward, change orientation by 90 degrees (clockwise or counterclockwise), and be seated on either a black or a white square.
		</li>
		<li>
		The Ant's next move is entirely dependent upon its current state. A history of the ant's previous states is not necessary to predict its next move.
		</li>
	</ol>
</p>


<p>
	Because our program is composed of a read/write head (1) that can execute a discrete number of operations (3)  upon a geographic arrangement of discrete memory cells (2), Langton's Ant is a Turning Machine. 
</p>

<p>
	Furthermore, because the Ant has no notion of history (4), and there are a discrete number of head-states (by combining 2, and 3), Langton's Ant can be modeled by automata. This can be implemented with the help of a doubly-linked list.
</p>