```markdown
# Лабораторная работа №3  
## Шаблоны. Админка Django

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
Изучить механизм шаблонов Django, научиться выводить данные из моделей в HTML-страницы, освоить настройку и кастомизацию административной панели Django.

## Задание
1. Вывести данные из модели `News` в браузер с использованием шаблонов.
2. Создать и настроить шаблон `index.html` для отображения списка новостей.
3. Передать данные из представления в шаблон через контекст.
4. Настроить сортировку записей по дате создания.
5. Создать суперпользователя и зарегистрировать модель в админ-панели.
6. Настроить отображение списка объектов в админке (`list_display`, `list_display_links`, `search_fields`).
7. Добавить человеко-понятные имена полей и моделей через `verbose_name` и `Meta`.

## Ссылка на репозиторий GitHub
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

## Ход выполнения работы

### 1. Вывод данных без использования шаблонов
На начальном этапе данные выводились напрямую через `HttpResponse` с ручным формированием HTML-строки в `views.py`.

```python
# news/views.py
from django.shortcuts import render
from django.http import HttpResponse
from .models import News

def index(request):
    news = News.objects.all()
    res = '<h1>Список новостей</h1>'
    for item in news:
        res += f'<div>\n<p>{item.title}</p>\n<p>{item.content}</p>\n</div>\n<hr>'
    return HttpResponse(res)

def test(request):
    return HttpResponse('<h1>Тестовая страница</h1>')
```

### 2. Создание шаблона и передача контекста
В приложении `news` создана папка `templates/news`, в которой размещён файл `index.html`. В `views.py` подключён `render` для передачи данных в шаблон.

**Файл `news/templates/news/index.html`:**
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>{{ title }}</title>
</head>
<body>
<h1>{{ title }}</h1>

{% for item in news %}
    <div>
        <p>{{ item.title }}</p>
        <p>{{ item.content }}</p>
        <p>{{ item.created_at }}</p>
    </div>
    <hr>
{% endfor %}
</body>
</html>
```

**Обновлённый `news/views.py`:**
```python
from django.shortcuts import render
from django.http import HttpResponse
from .models import News

def index(request):
    news = News.objects.all()
    return render(request, 'news/index.html', {'news': news, 'title': 'Список новостей'})

def test(request):
    return HttpResponse('<h1>Тестовая страница</h1>')
```

### 3. Форматирование даты и сортировка
Для удобного отображения даты в шаблоне использован фильтр `date`. В `views.py` добавлена сортировка записей по убыванию даты создания.

**Фрагмент шаблона:**
```html
{% for item in news %}
    <div>
        <p>{{ item.title }}</p>
        <p>{{ item.content }}</p>
        <p>{{ item.created_at|date:"Y-m-d H:i:s" }}</p>
    </div>
    <hr>
{% endfor %}
```

**Обновлённый контроллер:**
```python
def index(request):
    news = News.objects.order_by('-created_at')
    context = {
        'news': news,
        'title': 'Список новостей'
    }
    return render(request, template_name='news/index.html', context=context)
```

### 4. Настройка админ-панели Django
Создан суперпользователь командой `python manage.py createsuperuser`. Модель зарегистрирована в `admin.py`.

**Базовый `news/admin.py`:**
```python
from django.contrib import admin
from .models import News

admin.site.register(News)
```

**Кастомизация отображения:**
```python
class NewsAdmin(admin.ModelAdmin):
    list_display = ('id', 'title', 'created_at', 'updated_at', 'is_published')
    list_display_links = ('id', 'title')
    search_fields = ('id', 'title')

admin.site.register(News, NewsAdmin)
```

### 5. Настройка имён полей и моделей
В `models.py` добавлены `verbose_name` для полей и подкласс `Meta` для настройки отображения модели.

**`news/models.py`:**
```python
from django.db import models

class News(models.Model):
    title = models.CharField(max_length=150, verbose_name='Наименование')
    content = models.TextField(blank=True, verbose_name='Контент')
    created_at = models.DateTimeField(auto_now_add=True, verbose_name='Дата публикации')
    updated_at = models.DateTimeField(auto_now=True, verbose_name='Обновлено')
    photo = models.ImageField(upload_to='photos/%Y/%m/%d', verbose_name='Фото')
    is_published = models.BooleanField(default=True, verbose_name='Опубликовано')

    def __str__(self):
        return self.title

    class Meta:
        verbose_name = 'Новость'
        verbose_name_plural = 'Новости'
        ordering = ['-created_at', ]
```

**`news/apps.py`:**
```python
from django.apps import AppConfig

class NewsConfig(AppConfig):
    name = 'news'
    verbose_name = 'Новости'
```


## Выводы
В ходе лабораторной работы были изучены основы работы с шаблонами Django, передача данных из представлений, настройка административной панели. Получены навыки кастомизации отображения моделей в админке, что упрощает управление контентом.

---

**Ссылка на репозиторий:** [https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```
