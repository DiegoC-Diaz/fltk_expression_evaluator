#pragma
#include <iostream>
#include <FL/Fl_Input.H>

using namespace std;
/*Bloody-BadBuy 
* https://gist.github.com/Bloody-Badboy/08d71fa18755bb22cfab7950630e0339
* Base para crear la Link List para guardar las variables...
* //
*/
template<class T>


class Node {
public:
    Fl_Input * token;
    Fl_Input* value;
    Node<T>* next;

    Node();
    explicit Node(T val);
  
   
};

template<class T>
Node<T>::Node() {
    next = nullptr;
}



template<class T>
Node<T>::Node(T val) {
    token = val;
    value = nullptr;
    next = nullptr;
}



template<class E>
class LinkedList {

private:
    Node<E>* head;
    int s;

public:

    const static int SORT_ASC = 0;
    const static int SORT_DESC = 1;

    LinkedList();

    void add(E e);
    Node<E>* getHead();

    void add(int index, E e);

    void addFirst(E e);
    void addLast(E e);
    void remove(E e);
    void removeFirst();
    void setValue(E e,int index);
    void removeLast();
    int size();

    bool contains(E e);
    void reverse();

    void sort(int order);

    void print();
    void clear();
};

template<class E>
LinkedList<E>::LinkedList() {
    head = nullptr;
    s = 0;
}

template<class E>
void LinkedList<E>::addFirst(E e) {
    auto* newNode = new Node<E>(e);
    if (head == nullptr) {
        head = newNode;
    }
    else {
        newNode->next = head;
        head = newNode;
    }

    s++;
}

template<class E>
void LinkedList<E>::add(E e) {

    auto* newNode = new Node<E>(e);
    if (head == nullptr) {
        head = newNode;
        s++;
        return;
    }

    Node<E>* current = head;
    Node<E>* tmp;

    do {
        tmp = current;
        current = current->next;
    } while (current != nullptr);


    tmp->next = newNode;

    s++;
}
template<class E>
void LinkedList<E>::setValue(E e,int index) {


    if (head == nullptr) {

        return;
    }

    Node<E>* current = head;
    int i = 0;
    do {
        if (i++ == index) {
            break;
        }
        current = current->next;
    } while (current != nullptr);

    current->value = e;
}
template<class E>
inline Node<E>* LinkedList<E>::getHead()
{
    return head;
}

template<class E>
void LinkedList<E>::remove(E e) {

    Node<E>* current = head;
    Node<E>* prev = nullptr;
    bool found = false;

    if (current == nullptr) {
        cerr << "Err: can't perform remove() on an empty list" << endl;
        return;
    }

    do {
        if (current->token == e) {
            found = true;
            break;
        }

        prev = current;
        current = current->next;
    } while (current != nullptr);

    if (!found) {
        cerr << "Err: no element found with token " << e << endl;
        return;
    }

    // if the first element
    if (current == head) {
        prev = head;
        head = current->next;
        delete prev;
        return;
    }

    // if the last element
    if (current->next == nullptr) {
        prev->next = nullptr;
        delete current;
        return;
    }

    prev->next = current->next;
    delete current;

    s--;
}

template<class E>
void LinkedList<E>::removeFirst() {
    Node<E>* tmp = head;

    if (tmp == nullptr) {
        cerr << "Err: can't perform removeFirst() on an empty list" << endl;
        return;
    }

    head = tmp->next;
    delete tmp;

    s--;
}

template<class E>
void LinkedList<E>::removeLast() {
    Node<E>* current = head;
    Node<E>* prev = nullptr;

    if (current == nullptr) {
        cerr << "Err: can't perform removeLast() on an empty list" << endl;
        return;
    }

    do {
        prev = current;
        current = current->next;
    } while (current->next != nullptr);


    if (current->next == nullptr) {
        prev->next = nullptr;
        delete current;
        s--;
        return;
    }

}

template<class E>
void LinkedList<E>::reverse() {

    Node<E>* current = head;
    Node<E>* newNext = nullptr;
    Node<E>* tmp;

    if (current == nullptr) {
        cerr << "Err: can't perform reverse() on an empty list" << endl;
        return;
    }

    do {
        tmp = current->next;
        current->next = newNext;
        newNext = current;
        current = tmp;
    } while (current != nullptr);

    head = newNext;
}

template<class E>
void LinkedList<E>::sort(int order) {

    Node<E>* tmpPtr = head;
    Node<E>* tmpNxt = nullptr;

    if (tmpPtr == nullptr) {
        cerr << "Err: can't perform sort() on an empty list" << endl;
        return;
    }

    tmpNxt = head->next;

    E tmp;

    while (tmpNxt != nullptr) {
        while (tmpNxt != tmpPtr) {
            if (order == SORT_ASC) {
                if (tmpNxt->token < tmpPtr->token) {
                    tmp = tmpPtr->token;
                    tmpPtr->token = tmpNxt->token;
                    tmpNxt->token = tmp;
                }
            }
            else if (order == SORT_DESC) {
                if (tmpNxt->token > tmpPtr->token) {
                    tmp = tmpPtr->token;
                    tmpPtr->token = tmpNxt->token;
                    tmpNxt->token = tmp;
                }
            }
            else {
                cerr << "Err: invalid sort order '" << order << "'" << endl;
                return;
            }
            tmpPtr = tmpPtr->next;
        }
        tmpPtr = head;
        tmpNxt = tmpNxt->next;
    }
}

template<class E>
void LinkedList<E>::print() {

    Node<E>* current = head;

    cout << "Size:" << s << " -> [ ";
    while (current != nullptr) {
        cout << current->token << " ";
        current = current->next;
    }
    cout << "] " << endl;
}

template<class E>
void LinkedList<E>::clear() {

    Node<E>* current = head;

    do {
        Node<E>* tmp = current;
        current = current->next;
        delete tmp;
    } while (current != nullptr);

    head = nullptr;

    s = 0;
}

template<class E>
int LinkedList<E>::size() {
    return s;
}

template<class E>
void LinkedList<E>::addLast(E e) {
    add(e);
}

template<class E>
void LinkedList<E>::add(int index, E e) {

    if (index < 0 || index > s) {
        char buff[32];
        snprintf(buff, sizeof(buff), "Index: %d, Size: %d", index, s);
        throw std::out_of_range(buff);
    }

    if (index == 0) {
        addFirst(e);
        return;
    }

    if (index == s) {
        addLast(e);
        return;
    }

    Node<E>* current = head;
    auto* newNode = new Node<E>(e);

    int i = 0;
    do {
        if (i++ == index) {
            break;
        }
        current = current->next;
    } while (current != nullptr);

    newNode->next = current->next;
    current->next = newNode;

    s++;

}

template<class E>
bool LinkedList<E>::contains(E e) {
    Node<E>* current = head;

    while (current != nullptr) {
        if (current->token == e) {
            return true;
        }
        current = current->next;
    }
    return false;
}