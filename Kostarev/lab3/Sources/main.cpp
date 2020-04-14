#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

class Edge{     //класс ребра
public:
    char a{};   //начальная вершина
    char b{};   //конечная
    int p;  //фактическая величина потока
    int w;  //пропускная способность (вес)
    Edge(){
        p = 0;
        w = 0;
    }
    Edge(char a, char b){
        this->a = a;
        this->b = b;
        p = 0;
        w = 0;
    }
};

class FordFalkerson{    //класс алгоритма
public:
    std::vector<Edge*> graph;   //граф для обработки
    std::vector<Edge*> path;    //пройденный путь в виде стека ребер
    std::string pathInChar;     //пройденный путь в виде стека вершин
    std::vector<std::string> visitedPaths;  //пройденные пути
    int wMin = 1000;    //минимальная пропускная сп.
    int pMax{}; //максимальный поток
    char s{};   //исток
    char t{};   //сток
    char u{};   //проходдимая вершина
    FordFalkerson() = default;
    bool isVisitedV(char v){    //является ли вершина уже пройденной
        return pathInChar.find(v) != std::string::npos;
    }
    bool isVisited(std::string p){  //является ли путь уже пройденным
        for (auto & i:visitedPaths){
            if (i == p)
                return true;
        }
        return false;
    }
    bool haveInvert(Edge* e){   //есть ли в графе противоположное от е ребро
        for (auto &i:graph){
            if (e->a == i->b && e->b == i->a)
                return true;
        }
        return false;
    }
    void execute(){ //выполнение алгоритма
        int sze = graph.size();
        for (int i = 0; i < sze; i++){  //добавление к графу противоположных ребер
            if (!haveInvert(graph[i])){
                auto edgeInvert = new Edge(graph[i]->b, graph[i]->a);
                graph.push_back(edgeInvert);
            }
        }
        u = s;  //начало с истока
        pathInChar.push_back(u);    //пушим в стек
        std::cout << "Начинаем с истока " << s << std::endl;
        while (true){
            bool noChild = true;
            for(auto& i:graph){
                if (i->a == u && !isVisitedV(i->b)){
                    std::cout << "   " << i->a << "->" << i->b << " (" << i->p << "/" << i->w << "):\t";
                    pathInChar.push_back(i->b);
                    if (i->p < i->w && i->b != s && !isVisited(pathInChar)){    //если ребро не пройдено и не с нулевой пропускной
                        if (i->w - i->p < wMin){
                            wMin = i->w - i->p;
                            std::cout << "минимальная пропускная способность теперь " << wMin << ", ";
                        } //если пропускная сп. минимальная, обновляем значение
                        u = i->b;   //конечная вершина ребра становится текущей
                        path.push_back(i);  //пушим ребро в стек
                        visitedPaths.push_back(pathInChar); //пушим вершину в стек
                        std::cout << "пройденный путь " << pathInChar << std::endl;
                        noChild = false;
                        break;
                    }
                    std::cout << "пропускная способность = 0 или уже пройденный путь, пропускаем" << std::endl;
                    pathInChar.pop_back();
                }
            }
            if (noChild){   //если нет непройденных ребер, возвращаемся к предыдущему
                pathInChar.pop_back();
                if (pathInChar.empty()){ //если непройденных не осталось, заканчиваем цикл
                    std::cout << "Весь граф пройден!" << std::endl;
                    break;
                } else{
                    std::cout << "Все подвершины пройдены, поднимаемся, пройденный путь " << pathInChar << std::endl;
                }
                u = path.back()->a;
                path.pop_back();
                wMin = 1000;
                for(auto& i:path){  //обновляем минимальную пропускную сп. без учета удаленного ребра
                    if (i->w - i->p < wMin)
                        wMin = i->w - i->p;
                }
            }
            if (u == t){    //если вершина сток, возвращаемся к предыдущей в стеке
                std::cout << "Дошли до стока, прибавляем  мнимальную пропускную способность " << wMin << " у ребер пути " << pathInChar << std::endl;
                pathInChar.pop_back();
                std::cout << "Поднимаемся выше, пройденный путь " << pathInChar << std::endl;
                for(auto& i:path){  //добавляем минимальную пропускную к ребрам пути и отнимаем у противоположных
                    i->p += wMin;
                    for (auto & j:graph) {
                        if ((i->a == j->b) && (i->b == j->a))
                            j->p -= wMin;
                    }
                }
                u = path.back()->a;
                path.pop_back();
                wMin = 1000;
                for(auto& i:path){ //обновляем минимальную пропускную сп. без учета удаленного ребра
                    if (i->w - i->p < wMin)
                        wMin = i->w - i->p;
                }
            }
        }
        for (auto&i:graph){ //считаем максимальный поток
            if (i->a == s){
                pMax += i->p;
            }
        }
    }
    void printAnswer(){     //печать ответа в требуемом виде
        std::cout << pMax << std::endl;
        for (auto&i:graph){
            for (auto&j:graph){
                if ((j->a > i->a) || (j->a == i->a && j->b > i->b)){
                    auto k = new Edge();
                    k = j;
                    j = i;
                    i = k;
                }
            }
        }
        for (auto&i:graph){
            if (i->w > 0) {
                if (i->p < 0)
                    i->p = 0;
                std::cout << i->a << " " << i->b << " " << i->p <<std::endl;
            }
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    auto algorithm = new FordFalkerson();   //инициализация алгоритма
    int n;
    std::cin >> n >> algorithm->s >> algorithm->t;
    for (int i = 0; i < n; i++){    //заполняем граф ребрами
        auto edge = new Edge();
        std::cin >> edge->a >> edge->b >> edge->w;
        algorithm->graph.push_back(edge);
    }
    algorithm->execute();   //алгоритм
    algorithm->printAnswer();   //печать результатов
    return 0;
}