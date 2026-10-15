from django.db import models

class Category(models.Model):
    title=models.CharField(max_length=150, db_index=True, verbose_name='Наименование категории')

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
    photo = models.ImageField(upload_to='photos/%Y/%m/%d', verbose_name='Фото', blank=True)
    is_published = models.BooleanField(default=True, verbose_name='Опубликовано', blank=True)
    category = models.ForeignKey(Category, on_delete=models.PROTECT, null=True)

    def __str__(self):
        return self.title

    def my_func(self):
        return 'Последние новости'

    class Meta:
        verbose_name = 'Новость'
        verbose_name_plural = 'Новости'
        ordering = ['-created_at', ]

class Post(models.Model):
    title = models.CharField(max_length=150)
    content = models.TextField(blank=True)
    author = models.CharField(max_length=100)
    created_at = models.DateTimeField(auto_now_add=True)
    is_published = models.BooleanField(default=True)
    views_count = models.IntegerField(default=0)

    def __str__(self):
        return self.title

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
    course = models.ForeignKey(Course, on_delete=models.CASCADE, related_name='lessons', verbose_name='Курс')
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