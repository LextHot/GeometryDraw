#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

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
Triangle::Triangle(const QPoint& center, double size): m_size(size), m_angle(M_PI / 6) {
    m_center = center;
}

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
Rectangle::Rectangle(const QPoint& center, double width, double height): m_width(width), m_height(height), m_angle(0) {
    m_center = center;
}

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
Rhombus::Rhombus(const QPoint& center, double width, double height): m_width(width), m_height(height), m_angle(0) {
    m_center = center;
}

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
CustomPolygon::CustomPolygon(const QPoint& center, int sides, double radius): m_sides(sides), m_radius(radius), m_angle(3 * M_PI / 2) {
    m_center = center;
}

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

// Методы класса главного окна приложения
// Конструктор класса главного окна приложения
MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::GeometrydrawClass), m_isDragging(false), m_draggedFigure(nullptr) {
    ui->setupUi(this); // Инициализировать UI

    // Включить отслеживание мыши
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->installEventFilter(this);

    // Создать сцену
    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);

    // Подключить кнопки
    connect(ui->createButton, &QPushButton::clicked,
        this, &MainWindow::createFigure);
    connect(ui->deleteButton, &QPushButton::clicked,
        this, &MainWindow::deleteFigure);
    connect(ui->moveButton, &QPushButton::clicked,
        this, &MainWindow::moveFigure);
    connect(ui->rotateButton, &QPushButton::clicked,
        this, &MainWindow::rotateFigure);
    connect(ui->scaleButton, &QPushButton::clicked,
        this, &MainWindow::scaleFigure);
    connect(ui->colorButton, &QPushButton::clicked,
        this, &MainWindow::changeColor);
    connect(ui->figureType, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onFigureTypeChanged);
    onFigureTypeChanged(0);

    // Отключить кнопки, пока не выбрана фигура
    ui->deleteButton->setEnabled(false);
    ui->moveButton->setEnabled(false);
    ui->rotateButton->setEnabled(false);
    ui->scaleButton->setEnabled(false);
    ui->colorButton->setEnabled(false);
}

// Деструктор класса главного окна приложения
MainWindow::~MainWindow() {
    for (auto figure : m_figures)
        delete figure; // Очистить память
}

// Метод разделения событий выбора фигуры, ее перетаскивания и отпускания
bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->graphicsView->viewport()) {
        // Отловить нажатие по фигуре
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());
                // Обработать нажатие на фигуру
                startDragging(scenePos.toPoint());
                return true;
            }
        }
        // Отловить перемещение фигуры удержанием ЛКМ
        else if (event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->buttons() & Qt::LeftButton && m_isDragging) {
                QPointF scenePos = ui->graphicsView->mapToScene(mouseEvent->pos());
                // Обработать перемещение
                QPointF delta = scenePos - m_lastDragPos;
                if (delta.x() != 0 || delta.y() != 0) {
                    m_draggedFigure->move(delta.x(), delta.y());
                    m_lastDragPos = scenePos;
                    updateScene();
                }
                return true;
            }
        }
        // Отловить отпускание фигуры
        else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                m_isDragging = false;
                m_draggedFigure = nullptr;
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

// Отловить нажатие клавиши delete для удаления фигуры
void MainWindow::keyPressEvent(QKeyEvent* event) {
    // Отследить нажатие delete и backspace
    if (event->key() == Qt::Key_Delete ||
        event->key() == Qt::Key_Backspace) {
        deleteFigure(); // Удалить фигуру со сцены
    }
    else
        QMainWindow::keyPressEvent(event);
}

// Отловить нажатие ЛКМ и получить координату нажатия
void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // Проверить, что клик ЛКМ на graphicsView
        QPoint viewPos = ui->graphicsView->mapFromGlobal(event->globalPos());
        if (ui->graphicsView->rect().contains(viewPos)) {
            QPointF scenePos = ui->graphicsView->mapToScene(viewPos);
            startDragging(scenePos.toPoint());

            // Случай обычного выделения
            if (!m_isDragging)
                selectFigure(scenePos.toPoint());
        }
    }
    QMainWindow::mousePressEvent(event);
}

// Перетащить фигуру при удержании ЛКМ
void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    // Если фигуру перемещают
    if (m_isDragging && m_draggedFigure) {
        QPoint viewPos = ui->graphicsView->mapFromGlobal(event->globalPos());
        if (ui->graphicsView->rect().contains(viewPos)) {
            QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

            int dx = scenePos.x() - m_lastDragPos.x();
            int dy = scenePos.y() - m_lastDragPos.y();

            if (dx != 0 || dy != 0) {
                // Переместить фигуру
                m_draggedFigure->move(dx, dy);
                m_lastDragPos = scenePos.toPoint();
                updateScene();
                updateInfo();
            }
        }
    }
    QMainWindow::mouseMoveEvent(event);
}

// Отпустить фигуру при отпускании ЛКМ
void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        stopDragging();
    QMainWindow::mouseReleaseEvent(event);
}

// Начать перемещение фигуры
void MainWindow::startDragging(const QPoint& pos) {
    // Найти перемещаемую фигуру
    for (int i = m_figures.size() - 1; i >= 0; i--) {
        if (m_figures[i]->contains(pos)) {
            m_draggedFigure = m_figures[i];
            m_selectedFigure = m_draggedFigure;
            m_lastDragPos = pos;
            m_isDragging = true;

            // Обновить UI для выбранной фигуры
            ui->deleteButton->setEnabled(true);
            ui->moveButton->setEnabled(true);
            ui->rotateButton->setEnabled(true);
            ui->scaleButton->setEnabled(true);
            ui->colorButton->setEnabled(true);

            updateInfo();
            updateScene();
            break;
        }
    }
}

// Прекратить перемещение фигуры
void MainWindow::stopDragging() {
    m_isDragging = false;
    m_draggedFigure = nullptr;
}

// Выделить фигуру
void MainWindow::selectFigure(const QPoint& pos) {
    // Не переопределяем выбор, если идет перетаскивание
    if (m_isDragging) return;

    m_selectedFigure = nullptr;
    // Найти выделенную фигуру
    for (int i = m_figures.size() - 1; i >= 0; i--) {
        if (m_figures[i]->contains(pos)) {
            m_selectedFigure = m_figures[i];
            break;
        }
    }
    // Сделать кнопки активными при выборе фигуры
    if (m_selectedFigure) {
        ui->deleteButton->setEnabled(true);
        ui->moveButton->setEnabled(true);
        ui->rotateButton->setEnabled(true);
        ui->scaleButton->setEnabled(true);
        ui->colorButton->setEnabled(true);
    }
    // Сделать кнопки неактивными без выбора фигуры
    else {
        ui->deleteButton->setEnabled(false);
        ui->moveButton->setEnabled(false);
        ui->rotateButton->setEnabled(false);
        ui->scaleButton->setEnabled(false);
        ui->colorButton->setEnabled(false);
    }

    updateInfo();
    updateScene();
}

// Сделать неактивными ненужные для выбранной фигуры входные параметры
void MainWindow::onFigureTypeChanged(int index) {
    ui->param2->setEnabled(index == 1 || index == 2);
    ui->sidesSpinBox->setEnabled(index == 3);
}

// Выбрать создаваемую фигуру
void MainWindow::createFigure() {
    QPoint pos(ui->xSpinBox->value(), ui->ySpinBox->value());
    Figure* figure = nullptr;

    QString type = ui->figureType->currentText();
    // Определить тип фигуры
    if (type == "Треугольник")
        figure = new Triangle(pos, ui->param1->value());
    else if (type == "Прямоугольник")
        figure = new Rectangle(pos, ui->param1->value(), ui->param2->value());
    else if (type == "Ромб")
        figure = new Rhombus(pos, ui->param1->value(), ui->param2->value());
    else if (type == "Многоугольник")
        figure = new CustomPolygon(pos, ui->sidesSpinBox->value(), ui->param1->value());

    if (figure) {
        figure->setColor(Qt::darkMagenta); // Цвет по умолчанию
        m_figures.append(figure); // Добавить фигуру
        updateScene();
        updateInfo();
    }
}

// Удалить выбранную фигуру
void MainWindow::deleteFigure() {
    if (m_selectedFigure) {
        // Очистить список фигур от удаленной
        m_figures.removeOne(m_selectedFigure);
        delete m_selectedFigure;
        m_selectedFigure = nullptr;
        // Сделать кнопки неактивными
        ui->deleteButton->setEnabled(false);
        ui->moveButton->setEnabled(false);
        ui->rotateButton->setEnabled(false);
        ui->scaleButton->setEnabled(false);
        ui->colorButton->setEnabled(false);
        updateScene();
        updateInfo();
    }
}

// Переместить выбранную фигуру
void MainWindow::moveFigure() {
    if (m_selectedFigure) {
        m_selectedFigure->move(ui->moveDx->value(), ui->moveDy->value());
        updateScene();
        updateInfo();
    }
}

// Повернуть выбранную фигуру
void MainWindow::rotateFigure() {
    if (m_selectedFigure) {
        m_selectedFigure->rotate(ui->rotateAngle->value() * M_PI / 180.0);
        updateScene();
        updateInfo();
    }
}

// Масштабировать выбранную фигуру
void MainWindow::scaleFigure() {
    if (m_selectedFigure) {
        m_selectedFigure->scale(ui->scaleFactor->value());
        updateScene();
        updateInfo();
    }
}

// Изменить цвет выбранной фигуры
void MainWindow::changeColor() {
    if (m_selectedFigure) {
        QColor color = QColorDialog::getColor(m_selectedFigure->getColor(), this);
        if (color.isValid()) {
            m_selectedFigure->setColor(color);
            updateScene();
        }
    }
}

// Обновить и вывести информацию о выбранной фигуре
void MainWindow::updateInfo() {
    // Если фигура существует
    if (m_selectedFigure) {
        QString info = "<b>Выбрана фигура</b><br>";

        // Определить тип фигуры
        if (dynamic_cast<Triangle*>(m_selectedFigure))
            info += "Тип: Треугольник<br>";
        else if (dynamic_cast<Rectangle*>(m_selectedFigure))
            info += "Тип: Прямоугольник<br>";
        else if (dynamic_cast<Rhombus*>(m_selectedFigure))
            info += "Тип: Ромб<br>";
        else if (CustomPolygon* poly = dynamic_cast<CustomPolygon*>(m_selectedFigure)) {
            info += QString("Тип: Многоугольник<br>");
            info += QString("Кол-во углов: %1<br>").arg(poly->getSides());
        }
        // Вставить необходимые параметры текущей фигуры
        info += QString("Позиция: (%1, %2)<br>")
            .arg(m_selectedFigure->getPosition().x())
            .arg(m_selectedFigure->getPosition().y());

        QRect bounds = m_selectedFigure->getBounds();
        info += QString("Размеры: %1 x %2<br>")
            .arg(bounds.width())
            .arg(bounds.height());

        info += QString("Цвет: RGB(%1, %2, %3)")
            .arg(m_selectedFigure->getColor().red())
            .arg(m_selectedFigure->getColor().green())
            .arg(m_selectedFigure->getColor().blue());

        ui->infoLabel->setText(info);
    }
    // Если фигура была удалена
    else
        ui->infoLabel->setText("Нет выбранной фигуры");
}

// Обновить отрисованный экран
void MainWindow::updateScene() {
    m_scene->clear();

    // Нарисовать фигуру с заданными параметрами
    for (auto figure : m_figures) {
        QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem(figure->getPolygon());
        polygonItem->setBrush(QBrush(figure->getColor()));
        polygonItem->setPen(QPen(Qt::black, 2));
        m_scene->addItem(polygonItem);
    }

    // Выделить выбранную фигуры
    if (m_selectedFigure) {
        QRect bounds = m_selectedFigure->getBounds();
        bounds.adjust(-5, -5, 5, 5);
        QGraphicsRectItem* rect = m_scene->addRect(bounds, QPen(Qt::red, 3));
        rect->setZValue(1);
    }
}