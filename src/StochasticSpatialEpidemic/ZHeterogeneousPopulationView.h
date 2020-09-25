//============================================================
#ifndef ZPOPULATIONVIEW_H
#define ZPOPULATIONVIEW_H
//============================================================
#include <QGraphicsView>
//============================================================
class ZHeterogeneousPopulationView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulationView(QWidget* parent = nullptr);
    void zp_setScene(QGraphicsScene* scene);

protected:
    void wheelEvent(QWheelEvent* event) override;

signals:

};
//============================================================
#endif // ZPOPULATIONVIEW_H
