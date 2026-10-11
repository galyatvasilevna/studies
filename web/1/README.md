```markdown
# Лабораторная работа №2  
## Модели. CRUD

**Министерство науки и высшего образования Российской Федерации**  
Федеральное государственное автономное образовательное учреждение высшего образования  
«СЕВЕРО-КАВКАЗСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ»

**Институт информационных технологий и кибербезопасности**  
**Кафедра программной инженерии**

**Дисциплина:** Разработка web-приложений  
**Выполнил:** Тарасенко Галина Васильевна, 2 курс, группа ПИЖ-6-о-25-1(2), направление подготовки 09.03.04 «Программная инженерия»  
**Проверил:** Ассистент Белик А.И.  

**Ставрополь 2026**

---

## Цель работы
Разработка моделей, обучение проведению миграции данных, работа с базой данных.

## Задание (Вариант 1. Блог)
- Модель: `Post` (поля: `title`, `content`, `author`, `created_at`, `is_published`).
- Добавить поле `views_count` (целое число, по умолчанию 0).
- Написать метод `__str__`, возвращающий заголовок поста.
- Вывести все посты, у которых `is_published=True`.

## Ссылка на репозиторий GitHub
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

## Ход выполнения работы

1. **Установка менеджера баз данных Letos**  
   Для управления моделями и просмотра структуры БД установлен Letos (рис. 1).  
   *Рисунок 1 — Letos*

2. **Создание моделей**  
   В файле `news/models.py` описаны модели `News` и `Post`.  
   ```python
   from django.db import models

   class News(models.Model):
       title = models.CharField(max_length=150)
       content = models.TextField(blank=True)
       created_at = models.DateTimeField(auto_now_add=True)
       updated_at = models.DateTimeField(auto_now=True)
       photo = models.ImageField(upload_to='photos/%Y/%m/%d')
       is_published = models.BooleanField(default=True)

       def __str__(self):
           return self.title

   class Post(models.Model):
       title = models.CharField(max_length=150)
       content = models.TextField(blank=True)
       author = models.CharField(max_length=100)
       created_at = models.DateTimeField(auto_now_add=True)
       is_published = models.BooleanField(default=True)
       views_count = models.IntegerField(default=0)

       def __str__(self):
           return self.title
   ```

3. **Установка библиотеки Pillow**  
   Для работы с `ImageField` установлена библиотека Pillow командой `pip install Pillow`.

4. **Создание и применение миграций**  
   - `python manage.py makemigrations` — создание миграций (рис. 4).  
   - `python manage.py sqlmigrate news 0001` — просмотр SQL (рис. 5).  
   - `python manage.py migrate` — применение миграций (рис. 6).  
   После миграций в БД появилась таблица `news_news` (рис. 7).

5. **Настройка `settings.py` и `urls.py`**  
   - В `settings.py` добавлены `MEDIA_ROOT` и `MEDIA_URL` (рис. 8).  
   - В корневом `urls.py` подключены маршруты для админки, приложения `news` и раздача медиафайлов.

6. **Работа с ORM в Django shell**  
   - Создание записи через `News(title=..., content=...)` и `save()` (рис. 10).  
   - Массовое создание через `News.objects.create(...)` (рис. 12).  
   - Выборка всех записей: `News.objects.all()` (рис. 13, 15).  
   - Фильтрация: `filter()`, `get()`, `exclude()` (рис. 17, 18, 21).  
   - Обновление записи (рис. 19).  
   - Удаление записи: `delete()` (рис. 20).  
   - Для модели `Post`: создание записей, фильтрация по `is_published=True` (рис. 23).

7. **Добавление метода `__str__`**  
   Метод `__str__` возвращает заголовок, что улучшает отображение объектов в консоли и админке.


## Выводы
В ходе лабораторной работы были разработаны модели, изучен механизм миграций, получены навыки работы с базой данных через Django ORM. Освоены команды `makemigrations`, `migrate`, `sqlmigrate`, а также интерактивная оболочка `shell`.

---

## Ответы на контрольные вопросы

1. **Модель в Django** — это слой данных (компонент MTV), который описывает структуру таблицы БД через Python-класс (наследник `models.Model`).
2. **Типы полей:** `CharField` (строки), `TextField` (большой текст), `DateTimeField` (дата/время), `ImageField` (изображения), `BooleanField` (True/False), `IntegerField` (целые числа).
3. **blank vs null:** `blank=True` — поле может быть пустым в формах (валидация). `null=True` — поле может хранить NULL в базе данных.
4. **Миграции** — механизм отслеживания и применения изменений моделей к БД. Команды: `makemigrations` (создать), `migrate` (применить), `sqlmigrate` (показать SQL).
5. **Суперпользователь:** команда `python manage.py createsuperuser` в терминале.
6. **ORM** — взаимодействие с БД через Python-код без SQL. CRUD: Create (`create()`, `save()`), Read (`all()`, `filter()`, `get()`), Update (изменить атрибут + `save()`), Delete (`.delete()`).
7. **filter() vs get():** `filter()` возвращает QuerySet (0, 1 или много записей). `get()` возвращает ровно одну запись (иначе — ошибка).
8. **Медиафайлы:** установить Pillow; в `settings.py` задать `MEDIA_ROOT` и `MEDIA_URL`; в `urls.py` добавить маршрут через `static()` при `DEBUG=True`.
9. **`__str__`** — возвращает строковое представление объекта (например, заголовок) для удобного отображения в консоли и админке.
10. **makemigrations без migrate:** файл миграции создастся, но таблица в БД не появится. При обращении к модели возникнет ошибка `OperationalError: no such table`.

---

**Ссылка на репозиторий:** [https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```
