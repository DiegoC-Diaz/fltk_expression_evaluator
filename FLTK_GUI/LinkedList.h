#pragma once
#include <stdc++>
using namespace std;

struct Node { // linked list Node
    int data;
    Node* next;
};

Node* newNode(int k)
{ //defining new node
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = k;
    temp->next = NULL;
    return temp;
}

//Used to add new node at the end of the list
Node* addNode(Node* head, int k)
{
    if (head == NULL) {
        head = newNode(k);
    }
    else {
        Node* temp = head;
        Node* node = newNode(k);
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = node;
    }

    return head;
}

// Used to create new linked list and return head
Node* createNewLL()
{
    int cont = 1;
    int data;
    Node* head = NULL;
    while (cont) {
        cout << "Enter the data of the Node" << endl;
        cin >> data;
        head = addNode(head, data);
        cout << "Do you want to continue?(0/1)" << endl;
        cin >> cont;
    }
    return head;
}