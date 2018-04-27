#pragma once

//Uses K-D Tree C++ implementation found here: https://www.geeksforgeeks.org/k-dimensional-tree/ but alters int[] to instead use Point.h struct

#ifndef KDTREE
#define KDTREE

#include "stdafx.h"
#include "Point.h"
#include "FloatColor.h"

using namespace std;

// A structure to represent node of kd tree
struct Node
{
	Point point; // To store point
	int numThingsWithin;
	FloatColor currentColor;
	int treeSize;
	Node *left, *right;
};

// A method to create a node of K D tree
struct Node* newNode(Point p)
{
	struct Node* temp = new Node;

	temp->point.x = p.x;
	temp->point.y = p.y;

	temp->numThingsWithin = 0;
	temp->currentColor = FloatColor(0,0,0);
	temp->treeSize = 1;

	temp->left = temp->right = NULL;
	return temp;
}

// Inserts a new node and returns root of modified tree
// The parameter depth is used to decide axis of comparison
Node *insertRec(Node *root, Point point, unsigned depth)
{
	// Tree is empty?
	if (root == NULL)
		return newNode(point);

	root->treeSize++;
	// Calculate current dimension (cd) of comparison
	unsigned cd = depth % 2;

	// Compare the new point with root on current dimension 'cd'
	// and decide the left or right subtree
	if (point[cd] < (root->point[cd]))
		root->left = insertRec(root->left, point, depth + 1);
	else
		root->right = insertRec(root->right, point, depth + 1);

	return root;
}

// Function to insert a new point with given point in
// KD Tree and return new root. It mainly uses above recursive
// function "insertRec()"
Node* insert(Node *root, Point point)
{
	return insertRec(root, point, 0);
}

// A utility method to determine if two Points are same
// in K Dimensional space
bool arePointsSame(Point p, Point p2)
{
	// Compare individual pointinate values
	return p == p2;
}

// Searches a Point represented by "point[]" in the K D tree.
// The parameter depth is used to determine current axis.
Node* searchRec(Node* root, Point point, unsigned depth)
{
	// Base cases
	if (root == NULL)
		return NULL;
	if (arePointsSame(root->point, point))
		return root;

	// Current dimension is computed using current depth and total
	// dimensions (k)
	unsigned cd = depth % 2;

	// Compare point with root with respect to cd (Current dimension)
	if (point[cd] < root->point[cd])
		return searchRec(root->left, point, depth + 1);

	return searchRec(root->right, point, depth + 1);
}

// Searches a Point in the K D tree. It mainly uses
// searchRec()
Node* search(Node* root, Point point)
{
	// Pass current depth as 0
	return searchRec(root, point, 0);
}

#endif // !KDTREE