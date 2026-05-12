#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QColorDialog>
#include <QKeyEvent>
#include <QGraphicsPolygonItem>
#include "geometry.h"

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

    bool m_isDragging; // Флаг перемещения фигуры
    QPointF m_lastDragPos; // Предыдущая позиция фигуры
    Figure* m_draggedFigure = nullptr; // Перемещаемая фигура

    QGraphicsScene* m_scene; // Графическая сцена
    QVector<Figure*> m_figures; // Перечень фигур на сцене
    Figure* m_selectedFigure = nullptr; // Текущая выбранная фигура

    // UI элементы
    Ui::GeometrydrawClass* ui;
};