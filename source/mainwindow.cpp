#include "mainwindow.h"

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