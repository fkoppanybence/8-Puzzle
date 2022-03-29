#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

typedef vector<int> IntArray;

class State{

public:
    IntArray state;

    explicit State(const IntArray& arr){
        state.resize(9);
        state = arr;
    }

    /// find the index of the empty slot
    int findZeroIndex() const{
        for (int i = 0; i < state.size(); ++i)
            if (state[i] == 0) return i;

        return state.size();
    }

    int getManhattanCost(){
        int cost = 0;
        for (int i = 0; i < state.size(); ++i){
            int v = state[i];
            if (v != 0) {

                v = v - 1;
                int gx = v % 3;
                int gy = v / 3;
                int x = i % 3;
                int y = i / 3;

                int mancost = abs(x - gx) + abs(y - gy);
                cost += mancost;
            }
        }
        return cost;
    }

    void print() const{
        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; ++j){
                int index = i * 3 + j;
                cout << state[index] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
};

class Node{

public:
    Node(const State& state, Node *parent, int depth = 0) : state1(state) ,depth1(depth){
        parent1 = parent;
        mc1 = state1.getManhattanCost();
    }

public:
    State state1;
    Node *parent1;
    int mc1;
    int depth1;
};

bool isInArray(const State& state, const vector<Node*> &li){
    unsigned int i = 0;
    for (; i < li.size(); ++i){
        if (state.state == li[i]->state1.state)
            return true;
    }
    return false;
}

bool CompOp(Node * i, Node * current){
    return ((i->mc1 + i->depth1) > (current->mc1 + current->depth1));
}

typedef vector<Node *> NodeList;
NodeList openlist;
NodeList closedlist;
State goal(vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 0 });
State start(vector<int>{ 8, 2, 4, 7, 5, 3, 0, 6, 1 });

Node * GetNextNode(){
    if (openlist.empty()) return 0;
    Node * current;

    sort(openlist.begin(), openlist.end(), CompOp);
    current = openlist.back();

    openlist.pop_back();
    closedlist.push_back(current);
    return current;
}

void ExpandNode(Node * current, bool &_solved){
    if (current->state1.state == goal.state){
        _solved = true;
        return;
    }

    int zero = current->state1.findZeroIndex();

    IntArray neighbours;
    if(zero == 0){
        neighbours = vector<int>{1, 3};
    }else if(zero == 1){
        neighbours = vector<int>{0, 2, 4};
    }else if(zero == 2){
        neighbours = vector<int>{1, 5};
    }else if(zero == 3){
        neighbours = vector<int>{4, 0, 6};
    }else if(zero == 4){
        neighbours = vector<int>{3, 5, 1, 7};
    }else if(zero == 5){
        neighbours = vector<int>{4, 2, 8};
    }else if(zero == 6){
        neighbours = vector<int>{7, 3};
    }else if(zero == 7){
        neighbours = vector<int>{6, 8, 4};
    }else if(zero == 8){
        neighbours = vector<int>{7, 5};
    }

    for (int next : neighbours){
        State state_temp = current->state1;

        int tmp = state_temp.state[zero];
        state_temp.state[zero] = state_temp.state[next];
        state_temp.state[next] = tmp;

        if (!isInArray(state_temp, closedlist)){
            Node * n(new Node(state_temp, current, current->depth1 + 1));
            openlist.push_back(n);
        }
    }
}

int main()
{
    Node * node;
    int count = 0;
    bool _solved = false;

    Node * root(new Node(start, 0, 0));
    openlist.push_back(root);

    while (!_solved){
        node = GetNextNode();
        ExpandNode(node, _solved);
        count++;
    }

    vector<Node*> solution;
    Node *s = node;
    do{
        solution.push_back(s);
        s = s->parent1;
    } while (s != NULL);

    cout << "solved in " << solution.size() - 1 << " steps\n";
    for (int i = (int)solution.size() - 1; i >= 0; i--){
        solution[i]->state1.print();
    }

    return 0;
}