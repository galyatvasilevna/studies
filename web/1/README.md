# Лабораторная работа №1  
## Создание виртуального окружения. Контроллеры и маршруты

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
Настроить виртуальное пространство. Разработать контроллер приложения и организовать маршруты.

## Задание
1. Создать виртуальное окружение.
2. Установить Django.
3. Создать проект `MySite`.
4. Создать приложение `news`.
5. Реализовать контроллеры `index` и `test`.
6. Настроить маршруты, включая вложенную маршрутизацию.

## Ссылка на репозиторий GitHub
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

## Ход выполнения работы

1. **Создание виртуального окружения**  
   Выполнена команда:
   ```bash
   python -m venv venv
   ```
   Проверка пакетов через `pip list` 

2. **Установка Django**  
   ```bash
   pip install Django
   ```

3. **Создание проекта**  
   ```bash
   django-admin startproject MySite
   ```
   (рис. 4, 5). Запуск сервера:
   ```bash
   cd MySite
   python manage.py runserver
   ```

4. **Создание приложения**  
   ```bash
   python manage.py startapp news
   ```

5. **Регистрация приложения в `settings.py`**  
   ```python
   INSTALLED_APPS = [
       ...
       'news.apps.NewsConfig',
   ]
   ```

6. **Создание контроллеров в `news/views.py`**  
   ```python
   from django.shortcuts import render
   from django.http import HttpResponse

   def index(request):
       # print(dir(request))
       return HttpResponse('Hello world')

   def test(request):
       return HttpResponse('<h1>Тестовая страница</h1>')
   ```

7. **Настройка маршрутов в `MySite/urls.py`**  
   ```python
   from django.contrib import admin
   from django.urls import path, include

   urlpatterns = [
       path('admin/', admin.site.urls),
       path('news/', include('news.urls')),
   ]
   ```

8. **Создание `news/urls.py`**  
   ```python
   from django.urls import path
   from .views import *

   urlpatterns = [
       path('', index),
       path('test/', test),
   ]
   ```
   (рис. 17, 18).

9. **Проверка результата**  
   Страница `http://127.0.0.1:8000/news/test/` отображает «Тестовая страница» .


## Вывод
Настроила виртуальное пространство. Разработала контроллер приложения и организовала маршруты.

---

**Ссылка на репозиторий:** [https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
