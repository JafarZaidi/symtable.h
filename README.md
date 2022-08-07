Symbol Table Project

In this project, I was tasked in emulating encapsulation through a program

The way I went about doing that was by using a deque of Scopes

The Deque keeps track of which Scope we're in whereas the scopes themselves keep track of variables and other information 

Then, once we want to exit the current Scope, we pop the front of deque (we add scopes to the front, so think a stack)

Various functions exist to manipulate the deque of Scopes: Adding a new scope, exiting the current scope, adding information to the current scope, getting the number scopes, and dumping the information of the entirety of the Deque
