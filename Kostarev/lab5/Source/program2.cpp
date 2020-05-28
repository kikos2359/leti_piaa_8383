#include <cstring>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

struct Node;
using Template = std::vector<std::string>;             //вектор образцов
using Position = std::vector<std::pair<int,int>>;        //вывод позиция - образец
using Trie = std::vector<Node>;                        //бор
using Alphabet = std::map<char, int>;                  //алфавит

const int LENGTH = 5;
int wordLength = 0;
Alphabet alphabet = { {'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}, {'N', 4} };

int getIndex(char symb){
    return alphabet[symb];
}

bool compare(std::pair<int, int> a, std::pair<int, int> b){   //компаратор для вывода результата
    if (a.first == b.first)
        return a.second < b.second;
    else
        return a.first < b.first;
}

struct Node{
    int neighbours[LENGTH];      // соседние вершины
    int movePath[LENGTH];        // массив переходов
    int parrent;                          // вершина-предок
    std::vector<int> patternNumber;  // номер строки-образца
    int suffLink;                         // суффиксная ссылка
    int upSuffLink;                       // "сжатая" суффиксная ссылка
    char charToParrent;                   // символ ведущий к предку
    bool terminal;                        // является ли терминальной
};                                      // (совпадает со строкой)

Node makeNode(int parrent, char transfer){                    // создание новой вершины
    Node newNode;
    memset(newNode.neighbours, 255, sizeof(newNode.neighbours));
    memset(newNode.movePath, 255, sizeof(newNode.neighbours));
    newNode.suffLink = newNode.upSuffLink = -1;               // изначально нет ссылки
    newNode.parrent = parrent;
    newNode.charToParrent = transfer;
    newNode.terminal = false;
    return newNode;
}

void initTrie(Trie& trie){
    // создаем бор, изначально только корень
    trie.push_back(makeNode(0, '#'));
}

void addString(std::string& str, Trie& trie, int count){
    int index = 0;
    for (int i = 0; i < str.length(); i++){
        int curr = getIndex(str[i]);
        if (trie[index].neighbours[curr] == -1){        // если нет ребра по символу
            trie.push_back(makeNode(index, str[i]));
            trie[index].neighbours[curr] = trie.size() - 1;
            std::cout << "\tcreate node for transition: " << index << "--" << str[i] << "->" << trie.size()-1 << std::endl;
        }
        index = trie[index].neighbours[curr];
    }
    trie[index].terminal = true;
    trie[index].patternNumber.push_back(count);
}

bool findString(std::string& str, Trie& trie){       //функция поиска строки в боре
    int index = 0;
    for (int i = 0; i < str.length(); i++){
        int curr = getIndex(str[i]);
        if (trie[index].neighbours[curr] == -1)
            return false;
        index = trie[index].neighbours[curr];
    }
    return true;
}

int getLink(int vert, int index, Trie& trie);

// Функция для вычисления суффиксной ссылки
int getSuffLink(int vert, Trie& trie){
    if (trie[vert].suffLink == -1)                     // еще не искали
        if (vert == 0 || trie[vert].parrent == 0)       // корень или родитель корень
            trie[vert].suffLink = 0;                    // для корня ссылка в корень
        else
            trie[vert].suffLink = getLink(getSuffLink(trie[vert].parrent, trie), getIndex(trie[vert].charToParrent), trie);
    return trie[vert].suffLink;
}

// Функция для вычисления перехода
int getLink(int vert, int index, Trie& trie){
    if (trie[vert].movePath[index] == -1)                                // если переход по данному
        if (trie[vert].neighbours[index] != -1)                           // символу ещё не вычислен
            trie[vert].movePath[index] = trie[vert].neighbours[index];
        else if (vert == 0)
            trie[vert].movePath[index] = 0;
        else
            trie[vert].movePath[index] = getLink(getSuffLink(vert, trie), index, trie);
    return trie[vert].movePath[index];
}


// Функция для вычисления сжатой суффиксной ссылки
int getUpSuffLink(int vert, Trie& trie){
    if (trie[vert].upSuffLink == -1){             // если сжатая суффиксная ссылка ещё не вычислена
        int tmp = getSuffLink(vert, trie);
        if (trie[tmp].terminal)                  // если ссылка на терминальную, то ок
            trie[vert].upSuffLink = tmp;
        else if (tmp == 0)                       // на корень = 0
            trie[vert].upSuffLink = 0;
        else
            trie[vert].upSuffLink = getUpSuffLink(tmp, trie);   //поиск ближайшей
    }
    return trie[vert].upSuffLink;
}

//проверка сжатых суффиксных ссылок
void checkUpLink(int c[], int vert, int index, Trie& trie, std::string& text, Template & templ, std::vector<int>& positions){
    int n = 0;
    int prev;
    for (int i = vert; i != 0; i = getUpSuffLink(i, trie)){
        if (trie[i].terminal){
            if (n)
                std::cout << "\t" << prev << "--UpLink-->" << i << std::endl;
            n++;
            prev = i;
            for (auto& in: trie[i].patternNumber){
                int tmp = index + 1 - templ[in].size() - positions[in];
                if (tmp >= 0 && tmp <= text.length() - wordLength){
                    c[tmp]++;
                    std::cout << "\tNode " << i << " is terminal, at pos " << index-templ[in].length()+1 << " find: " << templ[in] << std::endl;
                }
            }
        }
    }
}

//Функция для процессинга текста
void processText(int c[], std::string& text, Trie& trie, Template& templ, Position& outPositions, std::vector<int>& positions){
    int current = 0;
    for (int i = 0; i < text.length(); i++){
        current = getLink(current, getIndex(text[i]), trie);
        checkUpLink(c, current, i, trie, text, templ, positions);
    }
    std::cout << "\nResult: " << std::endl;
    int num = 0;
    for (int i = 0; i < text.size(); i++){
        if (c[i] == templ.size()){
            std::cout << i+1 << std::endl;
            num++;
        }
    }
}

// Функция разбиение образца на максимальные подстроки без джокера
void splitString(std::string& str, Template & templ, std::vector<int>& positions, char joker){
    int index = 0;
    int position = 0;
    int count = 0;
    for (int i = 0; i < str.length(); i = index){
        std::string buff = "";
        while (index < str.length() && str[index] == joker)   // пропускаем джокера
            index++;
        if (index == str.length()) return;
        position = index;
        while(index < str.length() && str[index] != joker)    // новоя подстрока
            buff += str[index++];
        if(!buff.empty()){
            count++;
            positions.push_back(position);                    //запоминаем позиции подстрок
            templ.push_back(buff);
        }
    }

}

int main(){
    int count;
    std::vector<int> positions;
    char joker;
    Position outPositions;
    Trie trie;
    std::string text;
    std::string word;
    std::cin >> text;
    std::cin >> word;
    std::cin >> joker;
    auto it = alphabet.find(joker);
    while (it != alphabet.end()){
        std::cout << "\nError, joker can`t be ALphabet symbol: ";
        std::cin >> joker;
        it = alphabet.find(joker);
    }
    wordLength = word.length();
    Template templ;
    initTrie(trie);
    splitString(word, templ, positions, joker);
    int i = 0;
    std::cout << "\n START Fill trie:" << std::endl;
    for (auto& buff: templ){
        addString(buff, trie, i++);
    }
    std::cout << "END Fill trie." << std::endl;
    int c[text.size()] = {0};
    std::cout << "Count of nodes: " << trie.size() << std::endl;
    std::cout << "Parts of pattern without joker " << joker << std::endl;
    for (auto& buff: templ){
        std::cout << "\t" << buff << std::endl;
    }
    std::cout << "Array of starts positions for parts" << std::endl;
    for (auto& buff: positions){
        std::cout << " " << buff;
    }
    std::cout << "\nSTART Proccesing text" << std::endl;
    processText(c,text, trie, templ, outPositions, positions);
    std::cout << "END Proccesing text" << std::endl;
    return 0;
}