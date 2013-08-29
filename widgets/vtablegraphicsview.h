#ifndef VTABLEGRAPHICSVIEW_H
#define VTABLEGRAPHICSVIEW_H

#include <QGraphicsView>

class VTableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    enum typeMove_e { Left, Right, Up, Down };
    explicit VTableGraphicsView(QGraphicsScene* pScene, QWidget *parent = 0);
    
signals:
    /**
     * @brief itemChect Сигнал, що посилається коли змінюється стан вибору деталі.
     * @param flag Зберігає стан вибору деталі: false - знайдено, true - не знайдено.
     */
    void itemChect( bool flag );
public slots:
    /**
     * @brief selectionChanged Слот виконується при зміні стану вибору деталей.
     */
    void selectionChanged();
    /**
     * @brief rotateItems слот, який виконується при натисненні кнопки повороту деталі.
     */
    void rotateItems();
    /**
     * @brief MirrorItem дзеркалить об'єкт відносно вертикальної вісі семетрії об'єкта.
     */
    void MirrorItem();
    /**
     * @brief ZoomIn збільшує масштаб листа.
     */
    void ZoomIn();
    /**
     * @brief ZoomOut зменшує масштаб листа.
     */
    void ZoomOut();
protected:
    /**
     * @brief wheelEvent обробник повороту колеса мишки.
     * @param event передається подія.
     */
    void wheelEvent ( QWheelEvent * event );
    /**
     * @brief mousePressEvent обробник натиснення кнопки миші.
     * @param mousePress передається подія.
     */
    void mousePressEvent(QMouseEvent *mousePress);
    /**
     * @brief mouseReleaseEvent обробник відпускання кнопки миші.
     * @param event передається подія
     */
    void mouseReleaseEvent ( QMouseEvent * event );
    /**
     * @brief keyPressEvent обробник натиснення клавіші.
     * @param event передається подія.
     */
    void keyPressEvent ( QKeyEvent * event );
    /**
     * @brief rotateIt виконує поворот вибраних деталей на 180 градусів.
     */
    void rotateIt();
    /**
     * @brief MoveItem переміщує виділені об'єкти сцени.
     * @param move напрямок переміщення.
     */
    void MoveItem( VTableGraphicsView::typeMove_e move );
    
};

#endif // VTABLEGRAPHICSVIEW_H
