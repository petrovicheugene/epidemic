//============================================================
#ifndef ZABSTRACTEPIDEMICPROCESSDASHBOARD_H
#define ZABSTRACTEPIDEMICPROCESSDASHBOARD_H
//============================================================
#include <QWidget>
//============================================================
class ZAbstractDashBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ZAbstractDashBoard(QWidget* parent = nullptr);
    virtual bool zp_connect(QObject* component) = 0;
signals:

};
//============================================================
#endif // ZABSTRACTEPIDEMICPROCESSDASHBOARD_H
