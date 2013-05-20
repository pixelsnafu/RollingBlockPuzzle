/**
Filname: rollingblock.cpp
Purpose: Calculates the shortest path from start to goal
state in a rolling block maze. In other words, calculates the
minimum number of moves in which a block of dimensions 1x1x2 
can reach from start to goal state just by rolling in 4(top,
bottom, left, right) directions. The maze is a grid of x vertical 
rows and y horizontal columns. 

Authors : Piyush Verma
Ravdeep Johar

Pseudocode for the search : 
1. push start node into queue
2. while queue is not empty: 
3.      pop node from queue
4.      if node is goal, return
5.      else expand node's top, left, right and bottom
6.      add parents to the children and push them into the queue whichever nodes are valid

Running Time:
If the maze grid has x rows and y columns, then the running time of the algorithm is O(xy).
1. Time to fill the maze matrix = x * y
2. Time to perform bfs = x*y (number of vertices) +  2*x*y (number of edges) = O(xy).
3. Everything else = constant time O(1).
958640
**/

#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

char** puzzle;
int rows, columns;

//class for a location node on the graph
class node{
    //x,y coordinates
    int x1, y1, x2, y2;
    //parent of the node in the tree
    node* parent;
    //state of the block (standing or resting)
    int state;
public:

    //constructor
    node(int x1, int y1, int x2 = -1, int y2 = -1, int state = 1){
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
        parent = NULL;
        this->state = state;
    }

    //getters and setters
    int getX1() const{
        return x1;
    }

    int getY1() const{
        return y1;
    }

    int getX2() const{
        return x2;
    }

    int getY2() const{
        return y2;
    }

    int getState(){
        return state;
    }

    void setParent(node* p){
        this->parent = p;
    }

    node* getParent(){
        return parent;
    }

    //overloaded == operator for comparison of nodes
    bool operator==(const node& n) const{
        if(n.getX1() == x1 && n.getY1() == y1 && n.getX2() == x2 && n.getY2() == y2 && n.state == state)
            return true;
        else return false;
    }

    //overloaded != operator for comparison of nodes
    bool operator!=(const node& n) const{
        return !(*this == n);
    }

    //function to check if a node is valid in the maze grid
    bool isValid(){
        if(state == 1){
            if(x1 >= 0 && x1 < rows && y1 >=0 && y1 < columns && puzzle[x1][y1] != '*')
                return true;
            else
                return false;
        }else if(state == 2){
            if(x1 == x2){
                if(y1<y2){
                    if(x1 >= 0 && x1 < rows && y1 >=0 && y2<columns && puzzle[x1][y1] != '*' && puzzle[x2][y2] != '*')
                        return true;
                    else 
                        return false;
                }else{
                    if(x1>=0 && x1 < rows && y2 >= 0 && y2 < columns && puzzle[x1][y1] != '*' && puzzle[x2][y2] != '*')
                        return true;
                    else
                        return false;
                }  
            }else if(y1 == y2){
                if(x1<x2){
                    if(x1 >= 0 && x2<rows && y1 >= 0 && y1 < columns && puzzle[x1][y1] != '*' && puzzle[x2][y2] != '*')
                        return true;
                    else
                        return false;
                }else{
                    if(x2 >= 0 && x1 < rows && y1 >= 0 && y1 < columns && puzzle[x1][y1] != '*' && puzzle[x2][y2] != '*')
                        return true;
                    else return false;
                }
            }
        }
        return false;
    }

    //function to get the top node of the current node
    node* getTop(){
        if(state == 1){
            return new node(x1-2, y1, x1-1, y1, 2);
        }else if(state == 2){
            if(y1 == y2){
                if(x1 < x2)
                    return new node(x1-1, y1);
                else
                    return new node(x2-1, y2);
            }else if(x1 == x2){
                return new node(x1-1, y1, x2-1, y2, 2);
            }
        }
    }

    //function to get the bottom node of the current node
    node* getBottom(){
        if(state == 1){
            return new node(x1+1, y1, x1+2, y1, 2);
        }else if(state == 2){
            if(y1 == y2){
                if(x1 < x2)
                    return new node(x2+1, y2);
                else
                    return new node(x1+1, y1);
            }else if(x1 == x2){
                return new node(x1+1, y1, x2+1, y2, 2);
            }
        }
    }

    //function to get the right node of the current node
    node* getRight(){
        if(state == 1){
            return new node(x1, y1+1, x1, y1+2, 2);
        }else if(state == 2){
            if(y1 == y2){
                return new node(x1, y1+1, x2, y2+1, 2);
            }else if(x1 == x2){
                if(y1 < y2)
                    return new node(x2, y2+1);
                else
                    return new node(x1, y1+1);
            }
        }
    }

    //function to get the left node of the current node
    node* getLeft(){
        if(state == 1){
            return new node(x1, y1-2, x1, y1-1, 2);
        }else if(state == 2){
            if(y1 == y2){
                return new node(x1, y1-1, x2, y2-1, 2);
            }else if(x1 == x2){
                if(y1 < y2)
                    return new node(x1, y1-1);
                else
                    return new node(x2, y2-1);
            }
        }
    }    
};

//encapsulated node for queue
struct queueNode{
    node* location;
    queueNode* next;
    queueNode(node* n){
        location = n;
        next = NULL;
    }
};

//implementation of queue for bfs
class queue{
    queueNode* front;
    queueNode* rear;

public:

    //constructor
    queue(){
        front = NULL;
        rear = NULL;
    }

    //function to push a node at the end of the queue
    void enqueue(queueNode* q){
        if(front == NULL){
            front = q;
        }else{
            rear->next = q;
        }

        rear = q;
    }

    //function to delete a node from the top of the queue
    queueNode* dequeue(){
        queueNode* temp = new queueNode(front->location);
        temp = front;
        front = front->next;
        return temp;
    }

    //function to check if the queue is empty
    bool isEmpty(){
        if(front == NULL)
            return true;
        else return false;
    }
};

//globals
node* start, *goal;
node* bfs();
void printPath(vector<node*>);
int main(int argc, char* argv[]){
    //taking the input from the user
    cout<<"Enter the rows and the columns of the grid : ";
    char input[100];
    cin.getline(input, 10);
    sscanf(input, "%d %d", &rows, &columns);
    //cout<<rows<<" "<<columns<<endl;

    cout<<"Enter the grid(.*SG) : \n";

    puzzle = new char*[rows];
    for(int i = 0; i<rows; i++){
        puzzle[i] = new char[columns];
        //memset(puzzle[i], '0', columns*sizeof(char));
        cin.getline(input, 100);
        char* i_ptr = input;
        for(int j = 0; j<columns; j++){
            sscanf(i_ptr+2*j, "%c", &puzzle[i][j]);
            if(puzzle[i][j] == 'S' || puzzle[i][j] == 's')
                start = new node(i, j);
            if(puzzle[i][j] == 'G' || puzzle[i][j] == 'g')
                goal = new node(i, j);
        }
    }

    //performing bfs on the maze and displaying the output
    vector<node*> nodeVector;
    node* result = bfs();
    if(result == NULL)
        cout<<"Sorry! There is no solution to this maze.\nThe Goal state cannot be reached from the "
                "Start state in any possible way.\n";
    else{
        int count = 0;
        nodeVector.push_back(result);
        while(result->getParent() != NULL){
            count++;
            result = result->getParent();
            nodeVector.push_back(result);
        }
        cout<<"The block can reach from the Start to the Goal state in a minimum of "<<count<<" moves."<<endl;
        cout<<"From the State state...\n";
        printPath(nodeVector);
        cout<<"The block has now reached the Goal state.\n";
    }
}

//function to perform bfs on the maze grid
node* bfs(){

    queue* q = new queue;
    //push start node into the queue
    q->enqueue(new queueNode(start));
    while(!q->isEmpty()){
        //get the top node from the queue
        queueNode* qn = q->dequeue();
        node* n = qn->location;
        //if node matches goal state, then return
        if(*n == *goal){
            delete q;
            return n;
        }
        //get top node of the node n popped from the queue
        node* top = n->getTop();
        //if valid then add its parent and push it in the queue
        if(top->isValid()){
            if(n->getParent() == NULL || *top != *(n->getParent())){
                top->setParent(n);
                q->enqueue(new queueNode(top));
            }
        }

        //get bottom node of the node n popped from the queue
        node* bottom = n->getBottom();
        //if valid, then set the parent and push it into the queue
        if(bottom->isValid()){
            if(n->getParent() == NULL || *bottom != *(n->getParent())){
                bottom->setParent(n);
                q->enqueue(new queueNode(bottom));
            }
        }

        //get the right node of the node n popped from the queue
        node* right = n->getRight();
        //if valid, then add its parent and push it into the queue
        if(right->isValid()){
            if(n->getParent() == NULL || *right != *(n->getParent())){
                right->setParent(n);
                q->enqueue(new queueNode(right));
            }
        }

        //get the left node of the node n popped from the queue
        node* left = n->getLeft();
        //if valid, then add its parent and push it into the queue
        if(left->isValid()){
            if(n->getParent() == NULL || *left != *(n->getParent())){
                left->setParent(n);
                q->enqueue(new queueNode(left));
            }
        }
    }
    //if queue is empty, i.e. goal state cannot be reached, return null
    return NULL;
}

void printPath(vector<node*> nodeVector){


    for(int i = nodeVector.size()-1; i>0; i--){
        if(*(nodeVector[i]->getTop()) == *(nodeVector[i-1])){
            if(nodeVector[i-1]->getState() == 1)
                cout<<"Roll up so that the block stands at the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<")"<<endl;
            else
                cout<<"Roll up so that the block lies at the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<"),("<<nodeVector[i-1]->getX2()<<","<<nodeVector[i-1]->getY2()<<")"<<endl;
            continue;
        }else if(*(nodeVector[i]->getBottom()) == *(nodeVector[i-1])){
            if(nodeVector[i-1]->getState() == 1)
                cout<<"Roll down so that the block stands at the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<")"<<endl;
            else
                cout<<"Roll down so that the block lies at the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<"),("<<nodeVector[i-1]->getX2()<<","<<nodeVector[i-1]->getY2()<<")"<<endl;
            continue;
        }else if(*(nodeVector[i]->getLeft()) == *(nodeVector[i-1])){
            if(nodeVector[i-1]->getState() == 1)
                cout<<"Roll towards left so that the block stands at the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<")"<<endl;
            else
                cout<<"Roll towards left so that the block lies at the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<"),("<<nodeVector[i-1]->getX2()<<","<<nodeVector[i-1]->getY2()<<")"<<endl;
            continue;
        }else if(*(nodeVector[i]->getRight()) == *(nodeVector[i-1])){
            if(nodeVector[i-1]->getState() == 1)
                cout<<"Roll towards right so that the block stands at the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<")"<<endl;
            else
                cout<<"Roll towards right so that the block lies the coordinates ("<<nodeVector[i-1]->getX1()<<","<<nodeVector[i-1]->getY1()<<"),("<<nodeVector[i-1]->getX2()<<","<<nodeVector[i-1]->getY2()<<")"<<endl;
            continue;
        }
    }
}