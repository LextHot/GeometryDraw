#include <QtMath>
#include "ui_Geometrydraw.h"

// Родительский класс фигур
class Figure {
public:
    Figure(); // Конструктор родительского класса фигур
    ~Figure(); // Деструктор родительского класса фигур

    void draw(QPainter* painter); // Отрисовать фигуру
    void move(int dx, int dy); // Переместить фигуры

    virtual void rotate(double angle) = 0; // Повернуть фигуру
    virtual void scale(double factor) = 0; // Масштабировать фигуру
    virtual bool contains(const QPoint& point) const = 0; // Определить нажатие по фигуре
    virtual QRect getBounds() const = 0; // Получить границы фигуры
    virtual QPolygon getPolygon() const = 0; // Отрисовать и получить готовый полигон

    void setColor(const QColor& color) { m_color = color; } // Установить цвет
    QColor getColor() const { return m_color; } // Получить текущий цвет
    void setPosition(const QPoint& center) { m_center = center; } // Установить координаты
    QPoint getPosition() const { return m_center; } // Получить координаты

protected:
    QPoint m_center; // Центр расположения фигуры
    QColor m_color; // Цвет фигуры
};

// Дочерний класс треугольник
class Triangle : public Figure {
public:
    Triangle(const QPoint& center, double size); // Конструктор класса треугольник
    ~Triangle(); // Деструктор дочернего класса треугольник

    void rotate(double angle) override; // Повернуть фигуру
    void scale(double factor) override; // Масштабировать фигуру
    bool contains(const QPoint& point) const override; // Определить нажатие по фигуре
    // Получить границы треугольника для отлавливания нажатия на нее при выделении
    QRect getBounds() const override;
    // Отрисовать в базовом классе и получить готовый полигон
    QPolygon getPolygon() const override;

private:
    QVector<QPoint> getVertices() const; // Получить координаты вершин

    double m_size; // Размер стороны
    double m_angle; // Начальный угол расположения
};

// Дочерний класс прямоугольник
class Rectangle : public Figure {
public:
    // Конструктор класса прямоугольник
    Rectangle(const QPoint& center, double width, double height);
    ~Rectangle(); // Деструктор дочернего класса прямоугольник

    void rotate(double angle) override; // Повернуть фигуру
    void scale(double factor) override; // Масштабировать фигуру
    bool contains(const QPoint& point) const override; // Определить нажатие по фигуре
    // Получить границы прямоугольника для отлавливания нажатия на нее при выделении
    QRect getBounds() const override;
    // Отрисовать в базовом классе и получить готовый полигон
    QPolygon getPolygon() const override;

private:
    QVector<QPoint> getVertices() const; // Получить координаты вершин

    double m_width; // Ширина прямоугольника
    double m_height; // Высота прямоугольника
    double m_angle; // Начальный угол расположения
};

// Дочерний класс ромб
class Rhombus : public Figure {
public:
    // Конструктор класса ромб
    Rhombus(const QPoint& center, double width, double height);
    ~Rhombus(); // Деструктор дочернего класса ромб

    void rotate(double angle) override; // Повернуть фигуру
    void scale(double factor) override; // Масштабировать фигуру
    bool contains(const QPoint& point) const override; // Определить нажатие по фигуре
    // Получить границы ромба для отлавливания нажатия на нее при выделении
    QRect getBounds() const override;
    // Отрисовать в базовом классе и получить готовый полигон
    QPolygon getPolygon() const override;

private:
    QVector<QPoint> getVertices() const; // Получить координаты вершин

    double m_width; // Ширина ромба
    double m_height; // Высота ромба
    double m_angle; // Начальный угол расположения
};

// Дочерний класс многоугольник
class CustomPolygon : public Figure {
public:
    // Конструктор класса многоугольник
    CustomPolygon(const QPoint& center, int sides, double radius);
    ~CustomPolygon(); // Деструктор дочернего класса многоугольник

    void rotate(double angle) override; // Повернуть фигуру
    void scale(double factor) override; // Масштабировать фигуру
    bool contains(const QPoint& point) const override; // Определить нажатие по фигуре
    // Получить границы многоугольника для отлавливания нажатия на нее при выделении
    QRect getBounds() const override;
    // Отрисовать в базовом классе и получить готовый полигон
    QPolygon getPolygon() const override;
    int getSides() const { return m_sides; } // Получить число углов полигона

private:
    QVector<QPoint> getVertices() const; // Получить координаты вершин

    int m_sides; // Число углов многоугольника
    double m_radius; // Радиус описанной окружности многоугольника
    double m_angle; // Начальный угол расположения
};