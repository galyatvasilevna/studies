from django.contrib import admin
from .models import News, Category, Course, Lesson

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
admin.site.register(Category)

admin.site.register(Course)
admin.site.register(Lesson)
