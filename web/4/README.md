# Лабораторная работа №4  
## Связи моделей. Внешний вид шаблона

**Министерство науки и высшего образования Российской Федерации**  
Федеральное государственное автономное образовательное учреждение высшего образования  
«СЕВЕРО-КАВКАЗСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ»

**Институт информационных технологий и кибербезопасности**  
**Кафедра программной инженерии**

**Дисциплина:** Разработка web-приложений  
**Выполнил:** Тарасенко Галина Васильевна, 2 курс, группа ПИЖ-б-о-25-1(2), направление подготовки 09.03.04 «Программная инженерия»  
**Проверил:** Ассистент ДЦРСИЭ Белик А.И.  

**Ставрополь 2026**

---

## Цель работы
Освоить создание и настройку связей между моделями Django (`ForeignKey`, `ManyToManyField`, `OneToOneField`), изучить параметры связей (`on_delete`, `related_name`, `null`, `blank`, `default`), научиться применять миграции при добавлении связей к существующим моделям, а также настраивать отображение связанных данных в административной панели Django.

## Задание
**Вариант 11. Образование: курсы и уроки**

| Компонент | Описание |
|---|---|
| Первичная модель | `Course` (название, описание, преподаватель, цена) |
| Вторичная модель | `Lesson` (название, содержание, длительность, номер) |
| Тип связи | `ForeignKey` (один курс — много уроков) |
| `on_delete` | `CASCADE` |
| Дополнительно | `ordering` по номеру урока, `list_editable` для длительности |

## Ссылка на репозиторий GitHub
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

## Теоретическое обоснование

Django предоставляет три класса для организации связей между таблицами:

- `ForeignKey` — отношения «многие-к-одному». Требует два позиционных аргумента: класс модели, с которой связываем, и опцию `on_delete`.
- `ManyToManyField` — отношения «многие-ко-многим».
- `OneToOneField` — отношения «один-к-одному».

Параметр `on_delete` определяет поведение при удалении объекта, на который ссылаются:
- `CASCADE` — каскадное удаление (удаляет и связанные объекты).
- `PROTECT` — предотвращает удаление, вызывая `ProtectedError`.
- `RESTRICT` — предотвращает удаление, вызывая `RestrictedError` (более мягкий, чем `PROTECT`).

---

## Ход выполнения работы

### 1. Создание модели `Category` и связи с `News`

В файле `news/models.py` создана новая модель `Category`. Поле `title` индексируется (`db_index=True`) для быстрого поиска. Настроены названия и сортировка по умолчанию. Модель `Category` размещена перед `News`.

```python
from django.db import models

class Category(models.Model):
    title = models.CharField(max_length=150, db_index=True, verbose_name='Наименование категории')

    class Meta:
        verbose_name = 'Категория'
        verbose_name_plural = 'Категории'
        ordering = ['title']  # сортировка по наименованию

    def __str__(self):
        return self.title


class News(models.Model):
    title = models.CharField(max_length=150, verbose_name='Наименование')
    content = models.TextField(blank=True, verbose_name='Контент')
    created_at = models.DateTimeField(auto_now_add=True, verbose_name='Дата публикации')
    updated_at = models.DateTimeField(auto_now=True, verbose_name='Обновлено')
    photo = models.ImageField(upload_to='photos/%Y/%m/%d', verbose_name='Фото')
    is_published = models.BooleanField(default=True, verbose_name='Опубликовано', blank=True)
    category = models.ForeignKey(Category, on_delete=models.PROTECT, null=True, verbose_name='Категория')

    def __str__(self):
        return self.title
```

> **Важно:** так как в таблице `news` уже есть записи, добавление обязательного внешнего ключа вызывает ошибку. Поэтому указано `null=True`. При попытке задать `default=1` возникла ошибка, так как в таблице `news_category` ещё нет записи с `id=1`. После удаления `default=1` и последних миграций миграции выполнены заново.

### 2. Настройка административной панели

В `news/admin.py` зарегистрированы модели и настроено отображение связанных данных.

```python
from django.contrib import admin
from .models import News, Category

class NewsAdmin(admin.ModelAdmin):
    list_display = ('id', 'title', 'category', 'created_at', 'updated_at', 'is_published')
    list_display_links = ('id', 'title')
    search_fields = ('title', 'content')
    list_editable = ('is_published',)
    list_filter = ('is_published', 'category')

class CategoryAdmin(admin.ModelAdmin):
    list_display = ('id', 'title')
    list_display_links = ('id', 'title')
    search_fields = ('title',)

admin.site.register(News, NewsAdmin)
admin.site.register(Category, CategoryAdmin)
```

### 3. Индивидуальное задание: модели `Course` и `Lesson`

Созданы модели для варианта 11 «Образование: курсы и уроки».

```python
class Course(models.Model):
    title = models.CharField(max_length=200, verbose_name='Название курса')
    description = models.TextField(blank=True, verbose_name='Описание')
    teacher = models.CharField(max_length=150, verbose_name='Преподаватель')
    price = models.DecimalField(max_digits=8, decimal_places=2, verbose_name='Цена')

    def __str__(self):
        return self.title

    class Meta:
        verbose_name = 'Курс'
        verbose_name_plural = 'Курсы'
        ordering = ['title']


class Lesson(models.Model):
    course = models.ForeignKey(
        Course,
        on_delete=models.CASCADE,
        related_name='lessons',
        verbose_name='Курс'
    )
    title = models.CharField(max_length=200, verbose_name='Название урока')
    content = models.TextField(blank=True, verbose_name='Содержание')
    duration = models.PositiveIntegerField(verbose_name='Длительность (мин)')
    order = models.PositiveIntegerField(verbose_name='Номер урока')

    def __str__(self):
        return f'{self.order}. {self.title}'

    class Meta:
        verbose_name = 'Урок'
        verbose_name_plural = 'Уроки'
        ordering = ['order']  # сортировка по номеру урока
```

### 4. Настройка админки для `Course` и `Lesson`

```python
class CourseAdmin(admin.ModelAdmin):
    list_display = ('id', 'title', 'teacher', 'price')
    list_display_links = ('id', 'title')
    search_fields = ('title', 'teacher')
    list_filter = ('teacher',)

class LessonAdmin(admin.ModelAdmin):
    list_display = ('id', 'title', 'course', 'order', 'duration')
    list_display_links = ('id', 'title')
    list_editable = ('duration',)
    list_filter = ('course',)
    search_fields = ('title', 'content')
    ordering = ('course', 'order')

admin.site.register(News, NewsAdmin)
admin.site.register(Category, CategoryAdmin)
admin.site.register(Course, CourseAdmin)
admin.site.register(Lesson, LessonAdmin)
```

### 5. Применение миграций

После создания и изменения моделей выполнены команды:

```bash
python manage.py makemigrations
python manage.py migrate
```

В результате в базе данных появились таблицы `news_category`, `news_course`, `news_lesson`, а в таблице `news_news` добавлено поле `category_id`.

### 6. Результат

В административной панели Django отображаются разделы «Категории», «Курсы», «Новости», «Уроки». Для уроков доступно редактирование длительности прямо из списка (`list_editable`), работает фильтрация по курсу и сортировка по номеру урока.

---

## Выводы
В ходе лабораторной работы были изучены связи моделей Django, их параметры и особенности миграций. Получены навыки настройки административной панели для работы со связанными данными, что позволяет удобно управлять курсами и уроками.

---

**Ссылка на репозиторий:** [https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
