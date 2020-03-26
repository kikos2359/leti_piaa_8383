#include <iostream>
#include <vector>
#include <string>
#include <stack>

class Path{ //ребро
public:
    char start{};   //начало
    char end{};     //конец
    double length{};    //длина
    bool checked = false;   //обошел ли алгоритм ребро
};

class minPath{  //граф и алгоритм
public:
    char first{};   //вершина от которой найти путь
    char last{};    //до которой
    std::vector<Path> graph;    //множество ребер
    std::stack<char> checkedPath;   //путь
    void getMinimumPath();  //алгоритм
};


void minPath::getMinimumPath(){
    double minL = 100000.0;
    unsigned int minI = 100000;
    if (checkedPath.empty()){   //добавление начальной вершины
        checkedPath.push(first);
    } else{
        if (checkedPath.top() != first) //добавление вершины, если она еще не была добавлена
            checkedPath.push(first);
    }
    for (unsigned int i = 0; i < graph.size(); i++){
        if (graph[i].start == first){   //находим начальную вершину
            if (graph[i].end == last){  //если сосед конечная вершина то заканчиваем алгоритм
                std::cout << graph[i].start << "->" << last << " it's end!!!" << std::endl;
                checkedPath.push(last);
                return;
            }
            if (graph[i].length < minL && !(graph[i].checked)){ //находим оптимального соседа
                minL = graph[i].length;
                minI = i;
            } else{
                std::cout << first << "->" << graph[i].end << " it's no optimal" << std::endl;
            }
        }
    }
    if (minI == 100000){    //если оптимального соседа нет, то прыгаем назад, к родителю
        checkedPath.pop();
        std::cout << first << " havn't optimal neighbors, jump to " << checkedPath.top() << std::endl;
        first = checkedPath.top();
        getMinimumPath();
        return;
    }
    std::cout << first << "->" << graph[minI].end << " it's optimal" << std::endl;
    graph[minI].checked = true;
    first = graph[minI].end;    //теперь оптимальный сосед является начальной вершиной
    getMinimumPath();
}

int main() {
    auto graphM = new minPath();
    std::stack<char> newP;
    Path path{};
    std::cout << "Enter the path you want to find:" << std::endl;
    std::cin >> graphM->first >> graphM->last;
    std::cout << "Enter the paths (enter '!' if you want to finish):" << std::endl;
    while(true){
        std::cin >> path.start;
        if (path.start == '!')
            break;
        std::cin >> path.end >> path.length;
        graphM->graph.push_back(path);
    }
    graphM->getMinimumPath();
    do{
        newP.push(graphM->checkedPath.top());
        graphM->checkedPath.pop();
    } while (!graphM->checkedPath.empty());
    do{
        std::cout << newP.top();
        newP.pop();
    } while (!newP.empty());
    return 0;
}
