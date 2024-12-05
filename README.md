# ADT BST

- To compile and run the tests in your own console (being inside the general folder containing the files):

```bash
bash .tests/test.sh
```

## Introduction

The idea of ​​the project was to be able to provide the user with the implementation of a Binary Search Tree, with all its corresponding functions and in a 100% functional way.

In particular, the idea of ​​dynamic memory management (ensuring not to lose any byte of memory in the process) and the correct management of pointers, so as not to have invalid memory accesses, or reference losses in the process.

The end user would be provided with the compiled .h and .o so that they can use the library with the knowledge of all the pre and post conditions of the functions, but obviously without the implementation code.

Additionally, in the repository there is my own test library, with which in the example file I test the operation of the Binary Search Tree.

I use this library with the idea of ​​visually making it easier for anyone who wants to see how it works to read the tests and whether they work correctly or incorrectly.

## Functioning

###  Explanation of "Implementation and design"

First of all, this was a project for the university, so the code is written in spanish.

With that clarified, we move on to the explanation.

For the implementación of the BST, i used a main structure of the binary search tree, and then I used nodes with 2 pointers and an element to carry out the logic of the tree with the implementation and management of the respective places due to the comparison with the comparator provided by the user.

The main structure of the binary search tree contains the root node, the comparator function and the size of the tree.

What things to save within the tree and how to compare them is up to the user based on the comparator function to use and what they decide to save.

### How it work?

It starts with a root node, and then, with the help of the comparator, each time nodes are added, they are placed in the respective place based on whether the element it contains is greater, less than or equal to another. tree element already inserted.

The convention agreed is that every element less than or equal to it will end up on the left. While any larger element will end up on the right.

To eliminate, the necessary precautions are taken in cases of complexity to take care of the integrity of the tree itself, so that it does not break the structure and we do not lose memory.

The tree can be traversed in 3 ways: "PREODER, INORDER, POSTORDER".

---

# Testing

For test the code, i implemented my own tests.

In the repository actions you can see the status of the executed tests.

Also add the analysis with valgrind to see if you have memory leaks, since the program uses dynamic memory.

## The comparator used for testing works like this:

The comparator used in the "tests" casts the "void*" to "int" and compares the elements, so for all the tests carried out by this means, a vital condition is that every element to be inserted is of the character "int*" (passed by parameter like "void*").

---

# Types of routes implemented in the ADT

Preorder: In the preorder traversal, we first visit the root node, then traverse the left subtree, and finally the right subtree. That is, a node is processed before its children.

Inorder: In inorder traversal, we first traverse the left subtree, then visit the root node, and finally traverse the right subtree. This path produces an ordered sequence of the elements of the ABB, from lowest to highest (or vice versa if reversed).

Postorder: In postorder traversal, we first traverse the left subtree, then the right subtree, and finally visit the root node. This means that we process a node after its children.

---

# Functions

First of all, it is important to clarify something, in order to reduce the amount of information in the explanations below, corresponding to each function, the best case of its execution is contemplated.

But, each function has all its respective validations to, in case of error, terminate its execution.

The pre and post conditions of each function are specified in the corresponding ".h" file.

One more thing, the functions like "abb_insertar" or "abb_eliminar", that have complexity O(log(n)) is always in the case that we have a balanced tree.

Although we do not implement the tree balancing algorithm, we have this consideration when analyzing the complexity.

It is important to make this clarification because if the tree is loaded in a certain way, what will happen is that it can "Degenerate into a List", making the tree behave very similar to a list and the complexity of this type of functions becomes O(n).

## Computational complexity

I am going to make a list of the computational complexities in "BIG O" notation of each implemented function.

- abb_crear = O(1).

- abb_insertar = O(log(n)).

- abb_quitar = O(log(n)).

- abb_buscar = O(log(n)).

- abb_vacio = O(1).

- abb_tamanio = O(1).

- abb_destuir = O(n).

- abb_destuir_todo = O(n).

- abb_con_cada_elemento = O(n).

- abb_recorrer = O(n).
