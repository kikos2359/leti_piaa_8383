#include <iostream>
#include <string>
#include <vector>
//#include <windows.h>

void printArray(std::vector<int>& arr){
    for (unsigned int i = 0; i < arr.size() - 1; i++) {
        std::cout << arr[i] << ',';
    }
    std::cout << arr[arr.size() - 1];
}

void prefixFunc(std::string b, std::vector<int>& prefixes){ //вычисление префикс-ф для строки B
    std::cout << "Вычисление префикс-ф p для B = " << b << std::endl;
    std::cout << " p[0] = 0" << std::endl;
    for (unsigned int index = 1; index < b.size(); index++){    //проход по каждому символу строки кроме первого
        std::cout << " p[" << index << "]: " << std::endl;
        int previous = prefixes[index-1]; //изначально равен значению предыдущего символа
        std::cout << "  Предыдущее значение p[" << index-1 << "]=" << previous << std::endl;
        while (previous > 0 && b[previous] != b[index]){ //если предыдущее положительно и не равно исходному символу...
            std::cout << "  Оно больше нуля и B[" << previous << "] != B[" << index << "] = " << b[index] << ", берем p[" << previous-1 << "]" << std::endl;
            previous = prefixes[previous-1];    //...берем еще назад
        }
        if (b[previous] == b[index]){ //если символы образца равны, то прибавляем значение на единицу
            std::cout << "  B[" << previous << "] = B[" << index << "] = " << b[index] << ", прибавляем значение на единицу" << std::endl;
            previous++;
        }
        std::cout << " p[" << index << "] = " << previous << std::endl;
        prefixes.push_back(previous);   //пушаем в вектор
    }
}

int KMPFunc(std::string &a, std::string &b){
    std::vector<int> prefixes;  //префикс-ф строки B
    if (a.size() != b.size()){   //если строки с разными длинами то они не цикл сдвиг
        std::cout << "Строки не равны по длине" << std::endl;
        return -1;
    }
    a += a;     //удваиваем строку А
    std::cout << "Удваиваем строку А, А = " << a << std::endl;
    prefixes.push_back(0);  //значение для первого символа строки B
    prefixFunc(b, prefixes);    //вычисление остальных знач
    std::cout << "Префикс-ф для B равна [";
    printArray(prefixes);
    std::cout << "]" << std::endl;
    int indexB = 0;
    std::cout << "Начинаем поиск, посимвольно обрабатывая строку, indexB = 0" << std::endl;
    for (int indexA = 0; indexA < a.size(); indexA++){  //проход по удвоенной строке A
        std::cout << " A[" << indexA << "] = " << a[indexA] << ":" << std::endl;
        while (indexB > 0 && b[indexB] != a[indexA]){  //аналогично как для нахождения значений префикс-ф
            std::cout << "  Возвращаемся навзад, пока indexB не станет равным 0 или A[indexA] станет равным B[" << indexB << "]" << std::endl;
            std::cout << "  indexA = p[indexA-1] = p[" << indexA-1 << "] = " << prefixes[indexA-1] << std::endl;
            indexB = prefixes[indexB-1];
        }
        if (b[indexB] == a[indexA]){
            std::cout << "  B[" << indexB << "] = A[" << indexA << "] = " << a[indexA] << ", увеличиваем indexB на 1" << std::endl;
            indexB++;
        }
        if (indexB == b.size()){    //подстрока найдена
            std::cout << "  indexB = длине образца = " << b.size() << ", подстрока найдена" << std::endl;
            std::cout << "  Индекс первого вхождения = indexA - длина образца + 1 = " << indexA << "-" << b.size() << "+1" << std::endl;
            return indexA - b.size() + 1;   //первое вхождение
        }
    }
    return -1;  //если вхождения не найдены то печатаем -1
}

int main() {
    setlocale(LC_ALL, "RUS");
    //SetConsoleOutputCP(CP_UTF8);
    std::string a;  //A
    std::string b;  //B
    std::cin >> a;
    std::cin >> b;
    std::cout << KMPFunc(a, b);
    return 0;
}