#ifndef SRC_HPP
#define SRC_HPP

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

struct Data {
    int key;
    std::string value;
    Data(int k, std::string v) : key(k), value(v) {}
};

struct Node {
    int key;
    std::string value;
    Node *next; // for bucket list
    Node *prev_time, *next_time; // for insertion order list

    Node(int k, std::string v) : key(k), value(v), next(nullptr), prev_time(nullptr), next_time(nullptr) {}
};

class LinkedHashMap {
public:
    Node **array;
    int array_len;
    bool forEachByTime;
    Node *beg; // head of double linked list
    Node *cur; // tail of double linked list

    LinkedHashMap() : array(nullptr), array_len(0), forEachByTime(true), beg(nullptr), cur(nullptr) {}

    void init(int len_, bool forEachByTime_) {
        array_len = len_;
        forEachByTime = forEachByTime_;
        array = new Node*[array_len];
        for (int i = 0; i < array_len; ++i) {
            array[i] = nullptr;
        }
        beg = nullptr;
        cur = nullptr;
    }

    void clearMemory() {
        Node *p = beg;
        while (p) {
            Node *tmp = p;
            p = p->next_time;
            delete tmp;
        }
        delete[] array;
        array = nullptr;
        beg = cur = nullptr;
    }

    void insert(int key, std::string value) {
        Node *node = new Node(key, value);

        // Insert to bucket head
        node->next = array[key];
        array[key] = node;

        // Insert to double linked list tail
        if (!beg) {
            beg = cur = node;
        } else {
            cur->next_time = node;
            node->prev_time = cur;
            cur = node;
        }
    }

    void remove(int key, std::string value) {
        Node *p = array[key];
        Node *prev = nullptr;
        while (p) {
            if (p->value == value) {
                Node *toDelete = p;

                // Remove from bucket list
                if (prev) {
                    prev->next = p->next;
                    p = p->next;
                } else {
                    array[key] = p->next;
                    p = p->next;
                }

                // Remove from double linked list
                if (toDelete->prev_time) {
                    toDelete->prev_time->next_time = toDelete->next_time;
                } else {
                    beg = toDelete->next_time;
                }

                if (toDelete->next_time) {
                    toDelete->next_time->prev_time = toDelete->prev_time;
                } else {
                    cur = toDelete->prev_time;
                }

                delete toDelete;
            } else {
                prev = p;
                p = p->next;
            }
        }
    }

    std::vector<std::string> ask(int key) const {
        std::vector<std::string> res;
        Node *p = array[key];
        while (p) {
            res.push_back(p->value);
            p = p->next;
        }
        return res;
    }

    std::vector<Data> forEach() const {
        std::vector<Data> res;
        if (forEachByTime) {
            Node *p = beg;
            while (p) {
                res.push_back(Data(p->key, p->value));
                p = p->next_time;
            }
        } else {
            for (int i = 0; i < array_len; ++i) {
                Node *p = array[i];
                while (p) {
                    res.push_back(Data(p->key, p->value));
                    p = p->next;
                }
            }
        }
        return res;
    }
};

#endif
