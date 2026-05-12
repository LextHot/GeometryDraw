#include "geometry.h"

// Методы родительского класса фигур
// Конструктор родительского класса фигур
Figure::Figure() : m_center(0, 0), m_color(Qt::green) {}

// Деструктор родительского класса фигур
Figure::~Figure() {}

// Отрисовать фигуру
void Figure::draw(QPainter* painter) {
    QPolygon polygon = getPolygon();
    // Установить начальные параметры
    painter->save();
    painter->setBrush(QBrush(m_color));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPolygon(polygon);
    painter->restore();
}

// Переместить фигуры
void Figure::move(int dx, int dy) {
    m_center.setX(m_center.x() + dx);
    m_center.setY(m_center.y() + dy);
}

// Методы дочернего класса треугольника
// Конструктор класса треугольник
Triangle::Triangle(const QPoint& center, double size) : m_size(size), m_angle(M_PI / 6) {
    m_center = center;
}

// Деструктор дочернего класса треугольника
Triangle::~Triangle() {};

// Получить координаты вершин
QVector<QPoint> Triangle::getVertices() const {
    QVector<QPoint> vertices;
    // Радиус описанной окружности или расстояние от центра до вершины треугольника
    double radius = m_size / sqrt(3);

    // Определить координаты вершин
    for (int i = 0; i < 3; i++) {
        double angle = m_angle + i * 2 * M_PI / 3;
        int x = m_center.x() + static_cast<int>(radius * cos(angle));
        int y = m_center.y() + static_cast<int>(radius * sin(angle));
        vertices.append(QPoint(x, y));
    }

    return vertices;
}

// Отрисовать в базовом классе и получить готовый полигон
QPolygon Triangle::getPolygon() const {
    return QPolygon(getVertices());
}

// Повернуть фигуру
void Triangle::rotate(double angle) {
    m_angle += angle;
    // Нормализовать угол
    while (m_angle > 2 * M_PI) m_angle -= 2 * M_PI;
    while (m_angle < 0) m_angle += 2 * M_PI;
}

// Масштабировать фигуру
void Triangle::scale(double factor) {
    m_size *= factor;
    // Ограничить размеры
    if (m_size < 10) m_size = 10;
    if (m_size > 200) m_size = 200;
}

// Определить нажатие по фигуре
bool Triangle::contains(const QPoint& point) const {
    QVector<QPoint> vertices = getVertices();
    QPolygon polygon(vertices);
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

// Получить границы треугольника для отлавливания нажатия на нее при выделении
QRect Triangle::getBounds() const {
    QVector<QPoint> vertices = getVertices();
    QRect bounds;
    for (const auto& v : vertices)
        bounds = bounds.united(QRect(v, QSize(1, 1)));
    return bounds;
}

// Методы дочернего класса прямоугольника
// Конструктор класса прямоугольник
Rectangle::Rectangle(const QPoint& center, double width, double height) : m_width(width), m_height(height), m_angle(0) {
    m_center = center;
}

// Деструктор дочернего класса прямоугольника
Rectangle::~Rectangle() {};

// Получить координаты вершин
QVector<QPoint> Rectangle::getVertices() const {
    QVector<QPoint> vertices;

    double halfW = m_width / 2;
    double halfH = m_height / 2;

    // Определить точки вершин
    QVector<QPointF> localPoints;
    localPoints << QPointF(-halfW, -halfH)
        << QPointF(halfW, -halfH)
        << QPointF(halfW, halfH)
        << QPointF(-halfW, halfH);

    double cosA = cos(m_angle);
    double sinA = sin(m_angle);

    // Нормализовать угол
    for (const auto& p : localPoints) {
        int x = m_center.x() + static_cast<int>(p.x() * cosA - p.y() * sinA);
        int y = m_center.y() + static_cast<int>(p.x() * sinA + p.y() * cosA);
        vertices.append(QPoint(x, y));
    }

    return vertices;
}

// Отрисовать в базовом классе и получить готовый полигон
QPolygon Rectangle::getPolygon() const {
    return QPolygon(getVertices());
}

// Повернуть фигуру
void Rectangle::rotate(double angle) {
    m_angle += angle;
    while (m_angle > 2 * M_PI) m_angle -= 2 * M_PI;
    while (m_angle < 0) m_angle += 2 * M_PI;
}

// Масштабировать фигуру
void Rectangle::scale(double factor) {
    m_width *= factor;
    m_height *= factor;
    // Ограничить размер
    if (m_width < 10) m_width = 10;
    if (m_height < 10) m_height = 10;
    if (m_width > 200) m_width = 200;
    if (m_height > 200) m_height = 200;
}

// Определить нажатие по фигуре
bool Rectangle::contains(const QPoint& point) const {
    QVector<QPoint> vertices = getVertices();
    QPolygon polygon(vertices);
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

// Получить границы прямоугольника для отлавливания нажатия на нее при выделении
QRect Rectangle::getBounds() const {
    QVector<QPoint> vertices = getVertices();
    QRect bounds;
    for (const auto& v : vertices)
        bounds = bounds.united(QRect(v, QSize(1, 1)));
    return bounds;
}

// Методы дочернего класса ромба
// Конструктор класса ромб
Rhombus::Rhombus(const QPoint& center, double width, double height) : m_width(width), m_height(height), m_angle(0) {
    m_center = center;
}

// Деструктор дочернего класса ромб
Rhombus::~Rhombus() {};

// Получить координаты вершин
QVector<QPoint> Rhombus::getVertices() const {
    QVector<QPoint> vertices;

    // Определить точки вершин
    QVector<QPointF> localPoints;
    localPoints << QPointF(0, -m_height / 2)
        << QPointF(m_width / 2, 0)
        << QPointF(0, m_height / 2)
        << QPointF(-m_width / 2, 0);

    double cosA = cos(m_angle);
    double sinA = sin(m_angle);

    // Нормализовать угол
    for (const auto& p : localPoints) {
        int x = m_center.x() + static_cast<int>(p.x() * cosA - p.y() * sinA);
        int y = m_center.y() + static_cast<int>(p.x() * sinA + p.y() * cosA);
        vertices.append(QPoint(x, y));
    }

    return vertices;
}

// Отрисовать в базовом классе и получить готовый полигон
QPolygon Rhombus::getPolygon() const {
    return QPolygon(getVertices());
}

// Повернуть фигуру
void Rhombus::rotate(double angle) {
    m_angle += angle;
    while (m_angle > 2 * M_PI) m_angle -= 2 * M_PI;
    while (m_angle < 0) m_angle += 2 * M_PI;
}

// Масштабировать фигуру
void Rhombus::scale(double factor) {
    m_width *= factor;
    m_height *= factor;
    // Ограничить размеры
    if (m_width < 10) m_width = 10;
    if (m_height < 10) m_height = 10;
    if (m_width > 200) m_width = 200;
    if (m_height > 200) m_height = 200;
}

// Определить нажатие по фигуре
bool Rhombus::contains(const QPoint& point) const {
    QVector<QPoint> vertices = getVertices();
    QPolygon polygon(vertices);
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

// Получить границы ромба для отлавливания нажатия на нее при выделении
QRect Rhombus::getBounds() const {
    QVector<QPoint> vertices = getVertices();
    QRect bounds;
    for (const auto& v : vertices)
        bounds = bounds.united(QRect(v, QSize(1, 1)));
    return bounds;
}

// Методы дочернего класса многоугольника
// Конструктор класса многоугольник
CustomPolygon::CustomPolygon(const QPoint& center, int sides, double radius) : m_sides(sides), m_radius(radius), m_angle(3 * M_PI / 2) {
    m_center = center;
}

// Деструктор дочернего класса многоугольника
CustomPolygon::~CustomPolygon() {};

// Получить координаты вершин
QVector<QPoint> CustomPolygon::getVertices() const {
    QVector<QPoint> vertices;

    // Нормализовать угол
    for (int i = 0; i < m_sides; i++) {
        double angle = m_angle + i * 2 * M_PI / m_sides;
        int x = m_center.x() + static_cast<int>(m_radius * cos(angle));
        int y = m_center.y() + static_cast<int>(m_radius * sin(angle));
        vertices.append(QPoint(x, y));
    }

    return vertices;
}

// Отрисовать в базовом классе и получить готовый полигон
QPolygon CustomPolygon::getPolygon() const {
    return QPolygon(getVertices());
}

// Повернуть фигуру
void CustomPolygon::rotate(double angle) {
    m_angle += angle;
    while (m_angle > 2 * M_PI) m_angle -= 2 * M_PI;
    while (m_angle < 0) m_angle += 2 * M_PI;
}

// Масштабировать фигуру
void CustomPolygon::scale(double factor) {
    m_radius *= factor;
    // Ограничить размеры
    if (m_radius < 10) m_radius = 10;
    if (m_radius > 150) m_radius = 150;
}

// Определить нажатие по фигуре
bool CustomPolygon::contains(const QPoint& point) const {
    QVector<QPoint> vertices = getVertices();
    QPolygon polygon(vertices);
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

// Получить границы многоугольника для отлавливания нажатия на нее при выделении
QRect CustomPolygon::getBounds() const {
    QVector<QPoint> vertices = getVertices();
    QRect bounds;
    for (const auto& v : vertices)
        bounds = bounds.united(QRect(v, QSize(1, 1)));
    return bounds;
}