```markdown
# Лабораторная работа №1  
## Композиция и агрегация классов. Статические члены

**Вариант 11**  
**Дисциплина:** Алгоритмы и структуры данных  
**Выполнила:** Тарасенко Г. В., группа ПИЖ-б-о-25-1(2)

---

## Цель работы
Освоить проектирование отношений между классами: композиция, агрегация, ассоциация.  
Научиться использовать статические поля и методы, а также паттерн «Одиночка» (Singleton).

## Задание
Реализовать три уровня заданий:

1. **Базовый уровень** — композиция: `Building` (здание) и `Floor` (этаж).  
2. **Средний уровень** — агрегация + статический счётчик: `Roster` (состав) и `Athlete` (спортсмен).  
3. **Продвинутый уровень** — агрегация + Singleton: `Depot` (автопарк), `Bus` (автобус), `Route` (маршрут).

---

## Полный код решения

### Продвинутый уровень: агрегация + Singleton `Depot` + `Bus` + `Route`

```cpp
#include <iostream>
#include <vector>
using namespace std;

// ---------- Route: маршрут, живёт независимо ----------
class Route {
    int number;
    static int count;                 // счётчик маршрутов
public:
    Route(int n) : number(n) {
        count++;
        cout << "Route " << number << " created\n";
    }
    ~Route() {
        count--;
        cout << "Route " << number << " deleted\n";
    }
    int getNumber() const { return number; }
    static int getCount() { return count; }
};
int Route::count = 0;                 // определение вне класса

// ---------- Bus: агрегирует маршрут ----------
class Bus {
    int id;
    Route* route;                     // агрегация: указатель, не удаляем
public:
    Bus(int i) : id(i), route(nullptr) {
        cout << "Bus " << id << " created\n";
    }
    ~Bus() {
        cout << "Bus " << id << " deleted\n";   // Route НЕ удаляем!
    }
    void setRoute(Route* r) {
        if (r == nullptr) {           // проверка на nullptr
            cout << "Error: route is nullptr\n";
            return;
        }
        route = r;
        cout << "Bus " << id << " -> route " << route->getNumber() << "\n";
    }
    void print() const {
        cout << "Bus " << id << ": ";
        if (route) cout << "route " << route->getNumber();
        else       cout << "no route";
        cout << "\n";
    }
};

// ---------- Depot: Singleton + агрегация автобусов ----------
class Depot {
    vector<Bus*> buses;               // агрегация
    Depot() { cout << "Depot opened\n"; }               // приватный
    Depot(const Depot&) = delete;                       // запрет копии
    Depot& operator=(const Depot&) = delete;
    static Depot* instance;
public:
    static Depot* getInstance() {
        if (!instance) instance = new Depot();
        return instance;
    }
    ~Depot() { cout << "Depot closed\n"; }              // Bus НЕ удаляем

    void addBus(Bus* b) {
        if (b == nullptr) {           // проверка на nullptr
            cout << "Error: bus is nullptr\n";
            return;
        }
        buses.push_back(b);
    }
    void print() const {
        cout << "Depot has " << buses.size() << " bus(es):\n";
        for (auto* b : buses) b->print();
    }
};
Depot* Depot::instance = nullptr;

// ---------- main ----------
int main() {
    Route r1(10), r2(25);                        // маршруты живут отдельно
    Bus   b1(1), b2(2);                         // автобусы живут отдельно

    b1.setRoute(&r1);
    b2.setRoute(&r2);
    b2.setRoute(nullptr);                        // демонстрация защиты

    Depot* d1 = Depot::getInstance();
    Depot* d2 = Depot::getInstance();
    cout << "Singleton: d1==d2 -> " << (d1 == d2) << "\n";

    d1->addBus(&b1);
    d1->addBus(&b2);
    d1->addBus(nullptr);                         // демонстрация защиты
    d1->print();

    cout << "Routes alive: " << Route::getCount() << "\n";

    delete d1;                                   // закрываем автопарк
    cout << "-- Buses and routes still alive --\n";
    b1.print();
    b2.print();
    return 0;                                    // тут удаляются b1,b2,r1,r2
}
```

---

## Пример вывода (фрагмент)
```
Building created
Floor 1 created (h=3)
Floor 2 created (h=3)
...
Athletes alive: 3
Roster "Сборная" created
...
Depot opened
Singleton: d1==d2 -> 1
Routes alive: 2
Depot closed
-- Buses and routes still alive --
```

## Вывод
Изучены композиция и агрегация, разница во времени жизни объектов.  
Освоены статические поля и методы, паттерн Singleton и управление памятью в C++.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```
