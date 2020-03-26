
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class Path{ //ребро
public:
    char start{};   //откуда
    char end{};     //куда
    double length{};    //длина(вес)
};

class Dot{  //вершина
public:
    char name{};
    bool checked{};     //проходили ли мы ее (для астар)
    double g{};
    double f{};
    double h{};
    Dot(){      //конструктор
        checked = false;    //по умолчанию вершина не пройдена
    }
    explicit Dot(char a){   //перегрузка
        name = a;
    }
};

class minPath{  //граф и алгоритм
public:
    Dot first;  //откуда нужно найти путь
    char last{};    //куда
    std::vector<Path> graph;    //множество ребер
    std::vector<Dot> dots;     //вершины которые надо пройти для астар
    std::map<char, double> heuristicOfDot;      //эвристические функции вершин введенные пользователем
    std::map<char, char> minPath;   //карта минимального пути
    std::string getMinimumPath();   //алгоритм
    std::string reconstruct(char s, char f){    //вывод минимального пути из алгоритма
        std::string newStr;
        char curr = f;
        newStr += f;
        while (curr != s){
            curr = minPath[curr];
            newStr += curr;
        }
        std::reverse(newStr.begin(), newStr.end());
        return newStr;
    }
    int heuristicFunc(char a){  //обращение к эвристической функции
        return heuristicOfDot[a];
    }
    bool allchecked(){      //все ли вершины из dots пройдены
        for (auto & dot : dots){
            if (!dot.checked)
                return false;
        }
        return true;
    }
    bool isClosed(char a){  //пройдена ли вершина а из dots
        for (auto & i: dots){
            if (i.name == a && i.checked)
                return true;
        }
        return false;
    }
    unsigned int inDots(Dot* a){    //индекс+1 вершины а из dots (если нет то 0)
        for (unsigned int i = 0; i < dots.size(); i++){
            if (dots[i].name == a->name)
                return i+1;
        }
        return 0;
    }
    void printDots(){   //печать dots
        for (auto & i: dots){
            std::cout << i.name << "(" << i.f << ",";
            if (i.checked){
                std::cout << "checked) ";
            } else{
                std::cout << "unchecked) ";
            }
        }
        std::cout << std::endl;
    }
};


std::string minPath::getMinimumPath(){
    first.g = 0;    //сначала вычисляем эвристику для начальной вершины
    first.h = heuristicFunc(first.name);
    first.f = first.g + first.h;
    dots.push_back(first);  //закидываем начальную в dots
    int iterations = 0;
    while (!allchecked()){  //пока все вершины не пройдены, если пройдены то пути нет
        iterations++;
        std::cout << iterations << " iteration." << std::endl;
        double fmin = 10000.0;
        unsigned int imin = 0;
        std::cout << "\tDots: ";
        printDots();
        for (unsigned int i = 0; i < dots.size(); i++) {    //нахождение в dots вершины с минимальной f
            if (dots[i].f < fmin && !dots[i].checked){
                fmin = dots[i].f;
                imin = i;
            }
        }
        if (dots[imin].name == last){   //если это конечная вершина завершаем алгоритм
            return reconstruct(first.name, last);;
        }
        dots[imin].checked = true;  //взятая вершина метится пройденной
        for (auto & i: graph){  //находим соседей вершины
            if (i.start == dots[imin].name){
                bool gFuncIsBetter;
                if (isClosed(i.end)){   //соседи которые уже пройдены пропускаем
                    continue;
                }
                auto neighbor = new Dot(i.end);
                for (auto & dot : dots){
                    if (neighbor->name == dot.name)
                        neighbor = &dot;
                }
                double length = i.length + dots[imin].g;    //считаем значение функции
                if (!inDots(neighbor)){ //если соседа нет в dots то пушаем
                    dots.push_back(*neighbor);
                    std::cout << "\tDots: ";
                    printDots();
                    gFuncIsBetter = true;
                } else{
                    gFuncIsBetter = neighbor->g > length;
                }
                if (gFuncIsBetter){ //если нашли лучшее значение функции для вершины
                    minPath[neighbor->name] = dots[imin].name;  //создаем цепь сосед-вершина
                    dots[inDots(neighbor)-1].g = length;    //перезаписываем данные
                    dots[inDots(neighbor)-1].h = heuristicFunc(dots[inDots(neighbor)-1].name);
                    dots[inDots(neighbor)-1].f = dots[inDots(neighbor)-1].g + dots[inDots(neighbor)-1].h;
                    std::cout << "\tMap: " << reconstruct(first.name, neighbor->name) << std::endl;
                }
            }
        }
    }
    return "0"; //если путь не найден
}

int main() {
    auto graphM = new minPath();    //граф с которым будет работа
    Path path{};
    std::cout << "Enter the heuristic node functions (enter '!' if you want to finish):" << std::endl;
    while (true) {  //ввод эвристических функций для вершин
        char dot;
        std::cin >> dot;
        if (dot == '!')
            break;
        std::cin >> graphM->heuristicOfDot[dot];
    }
    std::cout << "Enter the path you want to find:" << std::endl;
    std::cin >> graphM->first.name >> graphM->last;
    std::cout << "Enter the paths (enter '!' if you want to finish):" << std::endl;
    while(true){    //ввод ребер
        std::cin >> path.start;
        if (path.start == '!')
            break;
        std::cin >> path.end >> path.length;
        graphM->graph.push_back(path);
    }
    std::cout << graphM->getMinimumPath();  //вывод результат алгоритма
    return 0;
}