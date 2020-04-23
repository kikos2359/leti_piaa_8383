#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

void printArray(std::vector<int>& arr){
    for (unsigned int i = 0; i < arr.size() - 1; i++) {
        std::cout << arr[i] << ',';
    }
    std::cout << arr[arr.size() - 1];
}

void prefixFunc(std::string p, std::vector<int>& prefixes){ //вычисление префикс-ф для образца
    std::cout << "Вычисление префикс-ф p для образца P = " << p << std::endl;
    std::cout << " p[0] = 0" << std::endl;
    for (unsigned int index = 1; index < p.size(); index++){    //проход по каждому символу строки кроме первого
        std::cout << " p[" << index << "]: " << std::endl;
        int previous = prefixes[index-1]; //изначально равен значению предыдущего символа
        std::cout << "  Предыдущее значение p[" << index-1 << "]=" << previous << std::endl;
        while (previous > 0 && p[previous] != p[index]){   //если предыдущее положительно и не равно исходному символу...
            std::cout << "  Оно больше нуля и P[" << previous << "] != P[" << index << "] = " << p[index] << ", берем p[" << previous-1 << "]" << std::endl;
            previous = prefixes[previous-1];    //...берем еще назад
        }
        if (p[previous] == p[index]){ //если символы образца равны, то прибавляем значение на единицу
            std::cout << "  P[" << previous << "] = P[" << index << "] = " << p[index] << ", прибавляем значение на единицу" << std::endl;
            previous++;
        }
        std::cout << " p[" << index << "] = " << previous << std::endl;
        prefixes.push_back(previous);   //пушаем в вектор
    }
}

std::vector<int> KMPFunc(){     //алгоритм
    std::string p;  //образец
    char c;     //символ текста
    std::vector<int> answer;    //индексы вхождений образца в тексте
    std::vector<int> prefixes;  //префикс-ф образца
    std::cin >> p;
    prefixes.push_back(0);  //пушаем значение для первого символа образца
    prefixFunc(p, prefixes);    //находим остальные значения
    std::cout << "Префикс-ф для образца равна [";
    printArray(prefixes);
    std::cout << "]" << std::endl;
    int indexT = 0;     //индекс символа в тексте
    int indexP = 0;     //индекс символа в образце и значение префикс-ф
    std::cin.get();     //пропускаем enter
    std::cout << "Начинаем поиск, посимвольно обрабатывая строку, indexP = 0" << std::endl;
    while (std::cin.get(c)){    //считывание символов текста
        if (c == '\n' || c == EOF) break;
        std::cout << " T[" << indexT << "] = " << c << ":" << std::endl;
        while (indexP > 0 && p[indexP] != c) { //аналогичный алгоритм нахождения префикс-ф
            std::cout << "  Возвращаемся навзад, пока indexP не станет равным 0 или P[indexP] станет равным T[" << indexT << "]" << std::endl;
            std::cout << "  indexP = p[indexP-1] = p[" << indexP-1 << "] = " << prefixes[indexP-1] << std::endl;
            indexP = prefixes[indexP-1];
        }
        if (p[indexP] == c){
            std::cout << "  P[" << indexP << "] = T[" << indexT << "] = " << c << ", увеличиваем indexP на 1" << std::endl;
            indexP++;
        }
        if (indexP == p.size()){    //если значение префикс-ф равно длине образца, то подстрока найдена
            std::cout << "  indexP = длине образца = " << p.size() << ", подстрока найдена" << std::endl;
            std::cout << "  Индекс первого вхождения = indexT - длина образца + 1 = " << indexT << "-" << p.size() << "+1" << std::endl;
            answer.push_back(indexT - p.size() + 1);    //индекс первого вхождения
            indexP = prefixes[indexP-1];    //последний символ, для которого выполняется идентичность подстроки и обзаца
            std::cout << "  Пушаем в массив индексов, indexP теперь равен предыдущму значению p[" << indexP+1 << "-1] = " << indexP << std::endl;
        }
        indexT++;
    }
    return answer;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::vector<int> answer = KMPFunc();    //массив индексов вхождений
    if (answer.empty()){    //массив пустой значит вхождений нет
        std::cout << "-1";
    } else{ //печать в требуемом виде
        printArray(answer);
    }
    return 0;
}