Pathfinder: Design/Implement your node objects (actors/actresses):

1q. What information does your node need to contain?
1a. The nodes need to contain the actor name and which movies they are a part of. They also need to contain the movies information such as 
the title year of realease as well as the actors that played a part in the movie.

2q.  How will you connect actors (nodes), relationships (edges), and movies to each other that allows efficient traversal of the graph without needlessly copying whole objects around? 
2a. I connected the actors to actors through the use of a movie as the edge that connects the actors. This was done by only having one copy of each actor
as well as one copy of a movie as well. This way there are no repeat movies or actor nodes. I also have a movies field in the actor node as well as a actors field in a movies node so each node has a reference to another and remain connceted. Thus by going from actor to actor is as simple as following thier movie field and then moving from the movieNodes actor field to the next actor until a result is found.

3q. Do you want to have a data structure for edges or merely represent them as connections between two nodes?
3a. We don't have a data strucutre for edges, but we do have a class which represents them as a connection between two nodes.

4q.Note that (priority_queue) it does not support an update_priority operation (how can you get around that?
4a. We get around that by adding elements as they are required, as in the case of Dijkstra's algorithm for finding shortest path 

5q. Think about what happens if you insert the same key twice into the heap, but with a lower priority. Which one gets popped first? 
5a. The one with a lower priority gets popped first.

6q. When you pop a key-priority pair, how do you know if it is valid/up-to-date or not?
6a. We know because of the 'done' field, whether or not we have visted/dequeued that pair before.