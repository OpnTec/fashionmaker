#ifndef VMAINGRAPHICSVIEW_H
#define VMAINGRAPHICSVIEW_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QGraphicsView>
#pragma GCC diagnostic pop

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
