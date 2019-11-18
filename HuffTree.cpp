#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Node {
    char c;
    int freq;
    Node *left;
    Node *right;
    
public:
    Node(char c, int freq) {
        this->c = c;
        this->freq = freq;
        this->left = nullptr;
        this->right = nullptr;
    }
};


struct compare{
    
    bool operator()(const Node* a, const Node* b)
    {
        return a->c < b->c;
    }
};

class MinHeap {
    vector<Node *> data;
    
public:
    Node *extractMin();
    void insert(Node *val);
    int size();
    bool empty() { return size() == 0;}
    void print() {
        for (auto n : data) {
            cout << n->c <<":"<< n->freq<< endl;
        }
    }
    Node* pop() { return extractMin();}
    void push(Node* val) { insert(val);}
    vector<Node *> getData() { return data;}
private:
    void heapifyUp(int idx);
    void heapifyDown(int i);
    void swap(int i, int j);
};

Node *MinHeap::extractMin() {
    swap(0, data.size() - 1);
    
    Node *temp = data.back();
    data.pop_back();
    
    heapifyDown(0);
    
    return temp;
}

void MinHeap::insert(Node *val) {
    data.push_back(val);
    heapifyUp(data.size()-1);
}

int MinHeap::size() { return data.size(); }


void MinHeap::heapifyUp(int idx) {
    if (idx > 0) {
        int parent = (idx - 1) / 2;
        if (data[parent]->freq > data[idx]->freq) {
            swap(parent, idx);
            heapifyUp(parent);
        }
    }
}

void MinHeap::heapifyDown(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int mini = i;
    if (left < data.size() && data[left]->freq < data[mini]->freq) {
        mini = left;
    }
    if (right < data.size() && data[right]->freq < data[mini]->freq) {
        mini = right;
    }
    
    if (mini != i) {
        swap(mini, i);
        heapifyDown(mini);
    }
}

void MinHeap::swap(int i, int j) {
    Node *temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}




class HuffTree{
    Node* root;
    MinHeap pq;
    unordered_map<char, string> encoded_letters;
public:
    HuffTree()
    {
        root = nullptr;
    }
    void insert(string sentence)
    {
        unordered_map<char, int> letters;
        for(int x=0;x<sentence.size();x++)
        {
            letters[sentence[x]]++;
        }
        vector<Node*> newLetters;
        
        
        for(auto it:letters)
        {
            newLetters.push_back(new Node(it.first, it.second));
        }
        
        std::sort(newLetters.begin(), newLetters.end(), compare());
        
        
        for(Node* n : newLetters)
        {
            pq.insert(n);
        }
        
        build();
    }
    
    void build()
    {
        
        Node* temp;
        
        while(pq.size()>0)
        {
            if(pq.size()==1)
            {
                root = pq.extractMin();
                break;
            }
            
            temp = new Node('$',0);
            temp->left = pq.extractMin();
            temp->right = pq.extractMin();
            temp->freq = temp->right->freq + temp->left->freq;
            pq.push(temp);
        }
        
        buildMap();
        
    }
    
    void buildMap()
    {
        string word="";
        buildMap(root, word);
    }
    
    void buildMap(Node* root, string cypher)
    {
        if(root->left)
        {
            cypher.append("0");
            buildMap(root->left,cypher);
            cypher.pop_back();
        }
        if(root->right)
        {
            cypher.append("1");
            buildMap(root->right, cypher);
            cypher.pop_back();
        }
        if(root->c!='$')
        {
            encoded_letters.insert(make_pair(root->c, cypher));
        }
    }
    
    void encode_word(string sentence)
    {
        string output="";
        for(int x = 0;x<sentence.length();x++)
        {
            for(auto letters:encoded_letters)
            {
                if(letters.first==sentence[x])
                {
                    output.append(letters.second);
                }
            }
        }
        cout<<output<<endl;
        
    }
    
    void decode_word(string word_to_decode)
    {
        string output = "";
        Node* temp = root;
        
        for(int x=0;x<word_to_decode.length();x++)
        {
            if(word_to_decode[x]=='0')
            {
                temp=temp->left;
            }
            else if(word_to_decode[x]=='1')
            {
                temp=temp->right;
            }
            if((!temp->right) && (!temp->left))
            {
                output.push_back(temp->c);
                temp = root;
            }
        }
        
        cout<<output<<endl;
        
    }
    
};

int main() {
    HuffTree tree;
    
    string sentence = "i am writing a long meaningless sentence about absolutely nothing. hallelujah!";
    
    tree.insert(sentence);
    tree.encode_word(sentence);
    tree.decode_word("10101001101111110000001100011001111011111111011011");
}

/*
 i am writing a long meaningless sentence about absolutely nothing. hallelujah!
 */

/*
 10101001101111110000001100011001111011111111011011
 */




