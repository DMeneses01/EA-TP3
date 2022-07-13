/* Duarte Emanuel Ramos Meneses - 2019216949
   Patricia Beatriz Silva Costa - 2019213995
   EA - Trabalho3 - 2021/2022 */

#include <iostream>
#include <string>
#include <sstream>  
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct operation
{
    int time;
    int num_dep;
    vector<int> dependencies;
    vector<int> go_to;
    int times_vis;
    int best_time;
    int bottle;
};

int num_op=0, option=-1;
int x=0, y=0, z=0;
int initial=0;
vector<operation> ops;
vector<int> cycles;
int node_fin = 0;

vector<int> visited;
vector<int> aux_visited;

priority_queue<int, vector<int>, greater<int>> pq;
vector<int> res;
int total_time = 0;

vector<int> check_vec;

int valid_final() {
    int fin = 0;
    for (int i=1; i<=num_op; i++) {
        if (ops[i].go_to.size() == 0) {
            fin++;
        }
        if (fin > 1)  return 0;
    }   
    return 1;
}

int valid_cycle(int pos) {
    visited[pos] = 1;
    aux_visited[pos] = 1;

    for (size_t i=0; i<ops[pos].go_to.size(); i++) {
        if (visited[ops[pos].go_to[i]] == 0) {
            if (valid_cycle(ops[pos].go_to[i])) return 1;
        }
        else if (aux_visited[ops[pos].go_to[i]] == 1)   return 1;        
    }

    aux_visited[pos] = 0;
    
    return 0;
}

void option1(){
    while(!pq.empty()){
        int p1 = pq.top();
        pq.pop();
        res.emplace_back(p1);
        total_time += ops[p1].time; 
        sort(ops[p1].go_to.begin(), ops[p1].go_to.end());
        for(size_t i = 0; i < ops[p1].go_to.size(); i++){
            ops[ops[p1].go_to[i]].times_vis ++;
            if (ops[ops[p1].go_to[i]].times_vis == ops[ops[p1].go_to[i]].num_dep)
                pq.push(ops[p1].go_to[i]);   
        }
    }
}


int option2(int pos) {
    if (ops[pos].go_to.size() == 0) {
        ops[pos].best_time = ops[pos].time;
        return 1;
    }

    for (size_t i=0; i<ops[pos].go_to.size(); i++) {
        if (ops[ops[pos].go_to[i]].best_time == 0)
            option2(ops[pos].go_to[i]);
    }

    int max_time = 0;
    for (size_t i=0; i<ops[pos].go_to.size(); i++) {
        if (max_time < ops[ops[pos].go_to[i]].best_time) {
            max_time = ops[ops[pos].go_to[i]].best_time;
        }
    }
    ops[pos].best_time = max_time + ops[pos].time;

    return 0;
}

void check_goto(int pos, int base) {

    visited[pos] = 1;
    for (size_t i=0; i<ops[pos].go_to.size(); i++) {
        if (visited[ops[pos].go_to[i]] == 0) {
            ops[base].bottle ++;
            check_goto(ops[pos].go_to[i], base);
        }   
    }
}

void check_dependencies(int pos, int base) {

    visited[pos] = 1;
    for (size_t i=0; i<ops[pos].dependencies.size(); i++) {
        if (visited[ops[pos].dependencies[i]] == 0) {
            ops[base].bottle ++;
            check_dependencies(ops[pos].dependencies[i], base);
        }   
    }
}


void option3() {

    while (!pq.empty()) {
        visited = vector<int> (num_op + 1);

        int p1 = pq.top();
        pq.pop();
        check_goto(p1, p1);
        check_dependencies(p1, p1);

        if (ops[p1].bottle == num_op - 1)  cout << p1 << "\n";
        
        for (size_t i=0; i<ops[p1].go_to.size(); i++) {
            ops[ops[p1].go_to[i]].times_vis ++;
            if (ops[ops[p1].go_to[i]].times_vis == ops[ops[p1].go_to[i]].num_dep) {
                pq.push(ops[p1].go_to[i]);
            }
        }
    }
}



int main (){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> num_op;

    ops = vector<operation> (num_op + 1);

    for (int i=0; i<num_op; i++) {
        cin >> x >> y;

        ops[i+1].time = x;
        ops[i+1].num_dep = y;
        ops[i+1].times_vis = 0;

        if (y == 0) {
            if (initial != 0) {
                cout << "INVALID\n";
                return 0;
            }
            else initial = i+1;
        }
    
        for (int j=0; j<y; j++) {
            cin >> z;
            ops[i+1].dependencies.emplace_back(z);
            ops[z].go_to.emplace_back(i+1);
        }

        
    }

    cin >> option;

    if (!valid_final()) {
        cout << "INVALID\n";
        return 0;
    }

    visited = vector<int> (num_op + 1);
    aux_visited = vector<int> (num_op + 1);
    
    if(initial == 0) initial = 1;
    if (valid_cycle(initial)) {
        cout << "INVALID\n";
        return 0;
    }    

    switch (option) {

        case 0:
            cout << "VALID\n";
            break;

        
        case 1:
            pq.push(initial);
            option1();
            cout << total_time << "\n";
            for (size_t i = 0; i < res.size(); i++) {
                cout << res[i] << "\n";
            }   
            break;
        
        case 2:
            option2(initial);
            cout << ops[initial].best_time << "\n";
            break;
           
        case 3:
            pq.push(initial);
            option3();
            break;
        
        default:
            break;
    }

    return 0;
}