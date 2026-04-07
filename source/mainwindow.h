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
#include <QGraphicsPolygonItem>
#include <QtMath>
#include "ui_Geometrydraw.h"

// Родительский класс фигур
class Figure {
public:
    Figure();
    virtual ~Figure();

    void draw(QPainter* painter);
    void move(int dx, int dy);

    virtual void rotate(double angle) = 0;
    virtual void scale(double factor) = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual QRect getBounds() const = 0;
    virtual QPolygon getPolygon() const = 0;

    void setColor(const QColor& color) { m_color = color; }
    QColor getColor() const { return m_color; }
    void setPosition(const QPoint& center) { m_center = center; }
    QPoint getPosition() const { return m_center; }

protected:
    QPoint m_center;
    QColor m_color;
};

// Дочерний класс треугольник
class Triangle : public Figure {
public:
    Triangle(const QPoint& center, double size);

    void rotate(double angle) override;
    void scale(double factor) override;
    bool contains(const QPoint& point) const override;
    QRect getBounds() const override;
    QPolygon getPolygon() const override;

private:
    QVector<QPoint> getVertices() const;

    double m_size;
    double m_angle;
};

// Дочерний класс прямоугольник
class Rectangle : public Figure {
public:
    Rectangle(const QPoint& center, double width, double height);

    void rotate(double angle) override;
    void scale(double factor) override;
    bool contains(const QPoint& point) const override;
    QRect getBounds() const override;
    QPolygon getPolygon() const override;

private:
    QVector<QPoint> getVertices() const;

    double m_width;
    double m_height;
    double m_angle;
};

// Дочерний класс ромб
class Rhombus : public Figure {
public:
    Rhombus(const QPoint& center, double width, double height);

    void rotate(double angle) override;
    void scale(double factor) override;
    bool contains(const QPoint& point) const override;
    QRect getBounds() const override;
    QPolygon getPolygon() const override;

private:
    QVector<QPoint> getVertices() const;

    double m_width;
    double m_height;
    double m_angle;
};

// Дочерний класс многоугольник
class CustomPolygon : public Figure {
public:
    CustomPolygon(const QPoint& center, int sides, double radius);

    void rotate(double angle) override;
    void scale(double factor) override;
    bool contains(const QPoint& point) const override;
    QRect getBounds() const override;
    QPolygon getPolygon() const override;

    void setSides(int sides);
    int getSides() const { return m_sides; }

private:
    QVector<QPoint> getVertices() const;

    int m_sides;
    double m_radius;
    double m_angle;
};

// Главное окно программы
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void createFigure();
    void deleteFigure();
    void moveFigure();
    void rotateFigure();
    void scaleFigure();
    void changeColor();
    void onFigureTypeChanged(int index);
    void updateInfo();
    void updateScene();
    void selectFigure(const QPoint& pos);
    void startDragging(const QPoint& pos);
    void stopDragging();

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