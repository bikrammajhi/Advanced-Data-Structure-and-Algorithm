#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 

enum COLOR
{ Red, Black };

typedef struct graph_node {
    int vertex;
    int weight;
    struct graph_node *next;
} graph_node;

typedef struct tree_node
{
    int data;
    struct tree_node *right;
    struct tree_node *left;
    struct tree_node *parent;
    enum COLOR color;
    int distance;        // Shortest distance from source node
    int parent_vertex;   // Parent node for path reconstruction
    graph_node *neighbors;  // List of neighboring nodes and edge weights
} tree_node;


typedef struct red_black_tree
{
  tree_node *root;
  tree_node *NIL;
} red_black_tree;

tree_node *new_tree_node (int data)
{
  tree_node *n = malloc (sizeof (tree_node));
  n->left = NULL;
  n->right = NULL;
  n->parent = NULL;
  n->data = data;
  n->color = Red;

  return n;
}

red_black_tree *new_red_black_tree ()
{
  red_black_tree *t = malloc (sizeof (red_black_tree));
  tree_node *nil_node = malloc (sizeof (tree_node));
  nil_node->left = NULL;
  nil_node->right = NULL;
  nil_node->parent = NULL;
  nil_node->color = Black;
  nil_node->data = 0;
  t->NIL = nil_node;
  t->root = t->NIL;

  return t;
}

void left_rotate (red_black_tree * t, tree_node * x)
{
  tree_node *y = x->right;
  x->right = y->left;
  if (y->left != t->NIL)
    {
      y->left->parent = x;
    }
  y->parent = x->parent;
  if (x->parent == t->NIL)
    {				//x is root
      t->root = y;
    }
  else if (x == x->parent->left)
    {				//x is left child
      x->parent->left = y;
    }
  else
    {				//x is right child
      x->parent->right = y;
    }
  y->left = x;
  x->parent = y;
}

void right_rotate (red_black_tree * t, tree_node * x)
{
  tree_node *y = x->left;
  x->left = y->right;
  if (y->right != t->NIL)
    {
      y->right->parent = x;
    }
  y->parent = x->parent;
  if (x->parent == t->NIL)
    {				//x is root
      t->root = y;
    }
  else if (x == x->parent->right)
    {				//x is left child
      x->parent->right = y;
    }
  else
    {				//x is right child
      x->parent->left = y;
    }
  y->right = x;
  x->parent = y;
}

void insertion_fixup (red_black_tree * t, tree_node * z)
{
  while (z->parent->color == Red)
    {
      if (z->parent == z->parent->parent->left)
	{			//z.parent is the left child

	  tree_node *y = z->parent->parent->right;	//uncle of z

	  if (y->color == Red)
	    {			//case 1
	      z->parent->color = Black;
	      y->color = Black;
	      z->parent->parent->color = Red;
	      z = z->parent->parent;
	    }
	  else
	    {			//case2 or case3
	      if (z == z->parent->right)
		{		//case2
		  z = z->parent;	//marked z.parent as new z
		  left_rotate (t, z);
		}
	      //case3
	      z->parent->color = Black;	//made parent black
	      z->parent->parent->color = Red;	//made parent red
	      right_rotate (t, z->parent->parent);
	    }
	}
      else
	{			//z.parent is the right child
	  tree_node *y = z->parent->parent->left;	//uncle of z

	  if (y->color == Red)
	    {
	      z->parent->color = Black;
	      y->color = Black;
	      z->parent->parent->color = Red;
	      z = z->parent->parent;
	    }
	  else
	    {
	      if (z == z->parent->left)
		{
		  z = z->parent;	//marked z.parent as new z
		  right_rotate (t, z);
		}
	      z->parent->color = Black;	//made parent black
	      z->parent->parent->color = Red;	//made parent red
	      left_rotate (t, z->parent->parent);
	    }
	}
    }
  t->root->color = Black;
}

void insert (red_black_tree * t, tree_node * z)
{
  tree_node *y = t->NIL;	//variable for the parent of the added node
  tree_node *temp = t->root;

  while (temp != t->NIL)
    {
      y = temp;
      if (z->data < temp->data)
	temp = temp->left;
      else
	temp = temp->right;
    }
  z->parent = y;

  if (y == t->NIL)
    {				//newly added node is root
      t->root = z;
    }
  else if (z->data < y->data)	//data of child is less than its parent, left child
    y->left = z;
  else
    y->right = z;

  z->right = t->NIL;
  z->left = t->NIL;

  insertion_fixup (t, z);
}

void rb_transplant (red_black_tree * t, tree_node * u, tree_node * v)
{
  if (u->parent == t->NIL)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

tree_node *minimum (red_black_tree * t, tree_node * x)
{
  while (x->left != t->NIL)
    x = x->left;
  return x;
}

void rb_delete_fixup (red_black_tree * t, tree_node * x)
{
  while (x != t->root && x->color == Black)
    {
      if (x == x->parent->left)
	{
	  tree_node *w = x->parent->right;
	  if (w->color == Red)
	    {
	      w->color = Black;
	      x->parent->color = Red;
	      left_rotate (t, x->parent);
	      w = x->parent->right;
	    }
	  if (w->left->color == Black && w->right->color == Black)
	    {
	      w->color = Red;
	      x = x->parent;
	    }
	  else
	    {
	      if (w->right->color == Black)
		{
		  w->left->color = Black;
		  w->color = Red;
		  right_rotate (t, w);
		  w = x->parent->right;
		}
	      w->color = x->parent->color;
	      x->parent->color = Black;
	      w->right->color = Black;
	      left_rotate (t, x->parent);
	      x = t->root;
	    }
	}
      else
	{
	  tree_node *w = x->parent->left;
	  if (w->color == Red)
	    {
	      w->color = Black;
	      x->parent->color = Red;
	      right_rotate (t, x->parent);
	      w = x->parent->left;
	    }
	  if (w->right->color == Black && w->left->color == Black)
	    {
	      w->color = Red;
	      x = x->parent;
	    }
	  else
	    {
	      if (w->left->color == Black)
		{
		  w->right->color = Black;
		  w->color = Red;
		  left_rotate (t, w);
		  w = x->parent->left;
		}
	      w->color = x->parent->color;
	      x->parent->color = Black;
	      w->left->color = Black;
	      right_rotate (t, x->parent);
	      x = t->root;
	    }
	}
    }
  x->color = Black;
}

void rb_delete (red_black_tree * t, tree_node * z)
{
  tree_node *y = z;
  tree_node *x;
  enum COLOR y_orignal_color = y->color;
  if (z->left == t->NIL)
    {
      x = z->right;
      rb_transplant (t, z, z->right);
    }
  else if (z->right == t->NIL)
    {
      x = z->left;
      rb_transplant (t, z, z->left);
    }
  else
    {
      y = minimum (t, z->right);
      y_orignal_color = y->color;
      x = y->right;
      if (y->parent == z)
	{
	  x->parent = z;
	}
      else
	{
	  rb_transplant (t, y, y->right);
	  y->right = z->right;
	  y->right->parent = y;
	}
      rb_transplant (t, z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
  if (y_orignal_color == Black)
    rb_delete_fixup (t, x);
}

void inorder (red_black_tree * t, tree_node * n)
{
  if (n != t->NIL)
    {
      inorder (t, n->left);
    //   printf ("%d\t", n->data);
      printf("%d(%s)\t", n->data, n->color == Red ? "RED" : "BLACK");
      inorder (t, n->right);
    }
}

void preorder(red_black_tree *t, tree_node *n) {
    if (n != t->NIL) {
        printf("%d(%s)\t", n->data, n->color == Red ? "RED" : "BLACK");
        preorder(t, n->left);
        preorder(t, n->right);
    }
}

void postorder(red_black_tree *t, tree_node *n) {
    if (n != t->NIL) {
        postorder(t, n->left);
        postorder(t, n->right);
        printf("%d(%s)\t", n->data, n->color == Red ? "RED" : "BLACK");
    }
}


tree_node *searchNode(tree_node *root, int key) {
    while (root != NULL && root != NULL) {
        if (key == root->data)
            return root;
        else if (key < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

// Dijkstra's algorithm implementation
void shortestPathDijkstra(red_black_tree *t, tree_node *nodes[], int n, int source, int destination) {
    int distance[n + 1];
    int parent[n + 1];
    int visited[n + 1];

    for (int i = 1; i <= n; i++) {
        distance[i] = INT_MAX;
        parent[i] = -1;
        visited[i] = 0;
    }

    distance[source] = 0;

    while (1) {
        int minDistance = INT_MAX;
        int u = -1;

        // Find the vertex with the minimum distance
        for (int i = 1; i <= n; i++) {
            if (!visited[i] && distance[i] < minDistance) {
                minDistance = distance[i];
                u = i;
            }
        }

        if (u == -1 || u == destination) {
            break;
        }

        visited[u] = 1;

        graph_node *neighbor = nodes[u]->neighbors;
        while (neighbor != NULL) {
            int v = neighbor->vertex;
            int weight = neighbor->weight;

            if (!visited[v] && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                parent[v] = u; // Store the index of the parent node in the nodes array
            }

            neighbor = neighbor->next;
        }
    }

    // Update the distances and parent information in your Red-Black Tree nodes
    for (int i = 1; i <= n; i++) {
        tree_node *node = searchNode(t->root, i);
        if (node != NULL) {
            node->distance = distance[i];
            node->parent_vertex = parent[i]; // Store the index of the parent node
        }
    }
}


// Display the updated distances and parent information in the Red-Black Tree
void displayDistancesAndParents(red_black_tree *t, tree_node *nodes[], int numVertices, int source) {
    printf("Distance from source to each node:\n");
    for (int i = 1; i <= numVertices; i++) {
        tree_node *node = searchNode(t->root, i);

        if (node->distance == INT_MAX) {
            printf("Distance from source to %d: ∞\n", i);
        } else {
            printf("Distance from source to %d: %d\n", i, node->distance);
        }
    }
}



int main() {
    red_black_tree *t = new_red_black_tree();
    int numVertices = 6; // Change this to the actual number of vertices

    tree_node *nodes[numVertices + 1]; // Declare the nodes array

    // Create and insert tree nodes for each vertex
    for (int i = 1; i <= numVertices; i++) {
        tree_node *pointer = new_tree_node(i); // Assuming vertices are labeled from 1 to numVertices
        insert(t, pointer);
        nodes[i] = pointer; // Store the node in the nodes array
    }

    // Assuming you have the graph edges and weights
    // Initialize graph edges and weights for each vertex
    graph_node *edges[numVertices + 1]; // Initialize the edges array

    // Example: Add edges for vertex 1
    graph_node *edge1 = malloc(sizeof(graph_node));
    edge1->vertex = 2;
    edge1->weight = 5;
    edge1->next = NULL;
    edges[1] = edge1;

    graph_node *edge2 = malloc(sizeof(graph_node));
    edge2->vertex = 3;
    edge2->weight = 3;
    edge2->next = NULL;
    edges[1]->next = edge2;

    // Add edges for vertex 2
    graph_node *edge3 = malloc(sizeof(graph_node));
    edge3->vertex = 4;
    edge3->weight = 2;
    edge3->next = NULL;
    edges[2] = edge3;

    // Add edges for vertex 3
    graph_node *edge4 = malloc(sizeof(graph_node));
    edge4->vertex = 4;
    edge4->weight = 7;
    edge4->next = NULL;
    edges[3] = edge4;

    // Add edges for vertex 4
    graph_node *edge5 = malloc(sizeof(graph_node));
    edge5->vertex = 5;
    edge5->weight = 1;
    edge5->next = NULL;
    edges[4] = edge5;
    

    graph_node *edge6 = malloc(sizeof(graph_node));
    edge6->vertex = 6;
    edge6->weight = 3;
    edge6->next = NULL;
    edges[4]->next = edge6;

    // Add edges for vertex 5
    graph_node *edge7 = malloc(sizeof(graph_node));
    edge7->vertex = 5;
    edge7->weight = 1;
    edge7->next = NULL;
    edges[5] = edge7;



    // Attach the edges array to the corresponding tree nodes
    for (int i = 1; i <= numVertices; i++) {
        nodes[i]->neighbors = edges[i];
    }

    // Call the shortestPathDijkstra function
    int source = 1 ; // Change this to the desired source vertex
    int destination = 6; // Change this to the desired destination vertex
    shortestPathDijkstra(t, nodes, numVertices, source, destination);

    // Display the updated distances and parent information in the Red-Black Tree
    displayDistancesAndParents(t, nodes, numVertices, source);
    return 0;
}
