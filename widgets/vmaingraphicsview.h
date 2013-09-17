#ifndef VMAINGRAPHICSVIEW_H
#define VMAINGRAPHICSVIEW_H

#include <QGraphicsView>

class VMainGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit VMainGraphicsView(QWidget *parent = 0);
    
signals:
protected:
    /**
     * @brief wheelEvent обробник повороту колеса мишки.
     * @param event передається подія.
     */
    void wheelEvent ( QWheelEvent * event );
    
public slots:
    /**
     * @brief ZoomIn збільшує масштаб листа.
     */
    void ZoomIn();
    /**
     * @brief ZoomOut зменшує масштаб листа.
     */
    void ZoomOut();
    
};

#endif // VMAINGRAPHICSVIEW_H
