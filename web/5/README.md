# Лабораторная работа №5  
## Внешний вид шаблона. Директивы. Теги. Фильтры

**Министерство науки и высшего образования Российской Федерации**  
Федеральное государственное автономное образовательное учреждение высшего образования  
«СЕВЕРО-КАВКАЗСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ»

**Институт информационных технологий и кибербезопасности**  
**Кафедра программной инженерии**

**Дисциплина:** Разработка web-приложений  
**Выполнил:** Тарасенко Галина Васильевна, 2 курс, группа ПИЖ-6-о-25-1(2)
**Проверил:** Ассистент Белик А.И.  

**Ставрополь 2026**

---

## Цель работы
Изучить внешний вид шаблонов Django, освоить подключение Bootstrap, использование директив, тегов и фильтров для вывода данных.

## Задание
1. Подключить Bootstrap через CDN.
2. Добавить навигационную панель (Navbar).
3. Вывести список новостей с помощью карточек (Card) с использованием цикла и фильтров.

## Ссылка на репозиторий GitHub
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

## Теоретическая часть
Шаблоны Django позволяют отделять логику представления от HTML-разметки. Для динамического вывода данных используются:
- **Директивы** (`{% ... %}`) — управляют логикой (циклы, условия).
- **Теги** — встроенные или пользовательские расширения.
- **Фильтры** (`{{ ... | ... }}`) — преобразуют вывод переменных (форматирование даты, обрезка строк и т.д.).

Bootstrap — популярный CSS-фреймворк, подключаемый через CDN, ускоряет создание адаптивных интерфейсов.

---

## Ход выполнения работы

### 1. Подключение Bootstrap через CDN
В шаблоне `index.html` подключены CSS и JS Bootstrap через CDN.

```html
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css">
    <title>{{ title }}</title>
</head>
<body>
    <h1>Новости</h1>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>
```

### 2. Добавление навигационной панели
Навигационная панель скопирована из документации Bootstrap и вставлена вместо заголовка `<h1>`.

```html
<nav class="navbar navbar-expand-lg bg-body-tertiary">
  <div class="container-fluid">
    <a class="navbar-brand" href="/">Navbar</a>
    <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent">
      <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarSupportedContent">
      <ul class="navbar-nav me-auto">
        <li class="nav-item"><a class="nav-link" href="/">Главная</a></li>
        <li class="nav-item"><a class="nav-link" href="/">Добавить новость</a></li>
      </ul>
    </div>
  </div>
</nav>
```

### 3. Вывод списка новостей с помощью карточек
Внутри контейнера создан цикл по элементам `news`, для каждого выводится карточка с заголовком, содержимым и датой.

```html
<div class="container mt-3">
  <h1>{{ title }}</h1>
  <div class="row">
    {% for item in news %}
      <div class="card mt-3">
        <div class="card-header">
          Категория
        </div>
        <div class="card-body">
          <h5 class="card-title">{{ item.title }}</h5>
          <p class="card-text">{{ item.content }}</p>
          <a href="#" class="btn btn-primary">Read more...</a>
        </div>
        <div class="card-footer text-body-secondary">
          {{ item.created_at|date:"Y-m-d H:i:s" }}
        </div>
      </div>
    {% endfor %}
  </div>
</div>
```

### 4. Использованные директивы и фильтры
- Директива `{% for item in news %}` ... `{% endfor %}` — цикл по списку новостей.
- Фильтр `{{ item.created_at|date:"Y-m-d H:i:s" }}` — форматирование даты.
- Переменные `{{ title }}`, `{{ item.title }}`, `{{ item.content }}` — вывод данных из контекста.

---

## Результаты
- Подключён Bootstrap через CDN.
- Добавлена навигационная панель.
- Список новостей выведен в виде карточек с использованием цикла и фильтра даты.
- Освоены директивы, теги и фильтры шаблонов Django.

## Выводы
В ходе лабораторной работы изучены основы работы с шаблонами Django, подключение Bootstrap для улучшения внешнего вида страниц, использование директив и фильтров для вывода данных. Получены навыки создания адаптивных интерфейсов.

---

**Ссылка на репозиторий:** [https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
