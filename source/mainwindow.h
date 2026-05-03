#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsPolygonItem>
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

// Главное окно программы
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // Конструктор класса главного окна приложения
    MainWindow(QWidget* parent = nullptr);
    // Деструктор класса главного окна приложения
    ~MainWindow();

protected:
    // Отловить нажатие клавиши delete для удаления фигуры
    void keyPressEvent(QKeyEvent* event) override;
    // Отловить нажатие ЛКМ и получить координату нажатия
    void mousePressEvent(QMouseEvent* event) override;
    // Перетащить фигуру при удержании ЛКМ
    void mouseMoveEvent(QMouseEvent* event) override;
    // Отпустить фигуру при отпускании ЛКМ
    void mouseReleaseEvent(QMouseEvent* event) override;
    // Метод разделения событий выбора фигуры, ее перетаскивания и отпускания
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void createFigure(); // Выбрать создаваемую фигуру
    void deleteFigure(); // Удалить выбранную фигуру
    void moveFigure(); // Переместить выбранную фигуру
    void rotateFigure(); // Повернуть выбранную фигуру
    void scaleFigure(); // Масштабировать выбранную фигуру
    void changeColor(); // Изменить цвет выбранной фигуры
    // Сделать неактивными ненужные для выбранной фигуры входные параметры
    void onFigureTypeChanged(int index);
    void updateInfo(); // Обновить и вывести информацию о выбранной фигуре
    void updateScene(); // Обновить отрисованный экран
    void selectFigure(const QPoint& pos); // Выделить фигуру
    void startDragging(const QPoint& pos); // Начать перемещение фигуры
    void stopDragging(); // Прекратить перемещение фигуры

    bool m_isDragging;
    QPointF m_lastDragPos;
    Figure* m_draggedFigure;

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QVector<Figure*> m_figures;
    Figure* m_selectedFigure;

    // UI элементы
    Ui::GeometrydrawClass* ui;
};