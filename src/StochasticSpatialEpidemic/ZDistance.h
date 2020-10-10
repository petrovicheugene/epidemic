//============================================================
#ifndef ZDISTANCE_H
#define ZDISTANCE_H
//============================================================
#include <QtGlobal>
//============================================================
class ZDistance
{
public:
    ZDistance();
    ZDistance(quint64, qreal);
    friend bool operator==(const ZDistance& left, const ZDistance& right)
    {
        return left.zv_distance == right.zv_distance;
    }
    friend bool operator>(const ZDistance& left, const ZDistance& right)
    {
        return left.zv_distance > right.zv_distance;
    }
    friend bool operator<(const ZDistance& left, const ZDistance& right)
    {
        return left.zv_distance < right.zv_distance;
    }

    // VARS
    quint64 zv_id;
    qreal zv_distance;
};
//============================================================
//bool operator==(const ZDistance& left, const ZDistance& right);
//bool operator>(const ZDistance& left, const ZDistance& right);
//bool operator<(const ZDistance& left, const ZDistance& right);

#endif // ZDISTANCE_H
