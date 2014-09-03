embmatrix
=========

Simple matrix library for embedded systems written in C++.

The main goal of the project was creation of library realizing basic matrix
math (Matlab like) without dynamic memory allocation. After the months of
development I realize that beautiful API can not be done without dynamic
memory. As compromise this library uses fast memory pools instead of classical
malloc/free.
