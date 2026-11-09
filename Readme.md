Penshoppe Inventory & E-Commerce System

A simple console program in C++17 that manages a clothing store's products and lets customers shop. It was made for a DSA Final Laboratory Project, so each feature uses a different data structure or algorithm.

How to Run
bash
g++ -std=c++17 -o penshoppe.cpp
./penshoppe

(Replace main.cpp with your file name. On Windows, run penshoppe.exe.)

What the Program Does
Menu	Feature	Data Structure / Algorithm Used
1	View all products	Vector (list of products)
2	Filter by brand	Linear search
3	Search by keyword or by product ID	Linear search / Binary Search Tree
4	Filter by price range	Binary search
5	Add items to the cart	Doubly Linked List
6	Checkout and process orders	Queue (first in, first out)
7	Admin: add, update, delete, undo	Stack
8	Show partner brands	Circular Linked List
9	Sort the inventory	Insertion Sort / Quick Sort
10	Exit	-

Simple Explanation of Each Data Structure
Binary Search Tree: stores product IDs so an ID can be found quickly.
Doubly Linked List: the shopping cart. Each item points to the one before and after it.
Queue: placed orders line up and are processed in the order they arrived.
Stack: before every admin change, the inventory is saved. Undo brings back the last saved version.
Circular Linked List: the brand list. After the last brand, it loops back to the first.
Binary Search: the price filter sorts a copy of the products, then jumps straight to the minimum price.
Insertion Sort: sorts by price, stock, or brand.
Quick Sort: sorts by price.

How to Use
Browse: choose 1 to see all products, or 2, 3, 4 to filter and search.
Shop: choose 5, enter a product ID and quantity. The cart is shown.
Checkout: choose 6, enter your name. Stock is reduced and the order is processed.
Admin: choose 7 to add, update, or delete products. Choose "Undo" to reverse the last change.
Sort: choose 9 and pick a sorting method.

Sample Products
ID	Product	Brand	Price	Stock
1001	Classic Logo T-Shirt	Penshoppe	P499.00	25
1002	Basic Cotton Tee	Penshoppe	P399.00	30
1003	Denim Straight Pants	Penshoppe	P999.00	12
1004	Printed Hoodie	Penshoppe	P1299.00	10
2001	Oxygen Graphic Tee	Oxygen	P599.00	18
2002	Oxygen Cargo Pants	Oxygen	P1199.00	8
3001	Memo Casual Blouse	Memo	P799.00	14
3002	Memo Wide-Leg Pants	Memo	P1099.00	9
4001	Regatta Polo Shirt	Regatta	P899.00	16
4002	Regatta Casual Shorts	Regatta	P699.00	20
Notes
Data is not saved. Everything resets when the program closes.
Option 6 places the order and processes the queue right away.
Sorting changes the order of the real inventory.
Known bug: in menu option 3, the text uses \\n instead of \n, so it prints a literal \n. Change \\n to \n to fix it.