#ifndef TOWERPOSTION_H
#define TOWERPOSTION_H
#include <QPoint>
class TowerPostion
{
    QPoint towerPos;
    bool towerExsist;

public:
    TowerPostion();
    TowerPostion(QPoint pos);
    QPoint getPos();
    void setPos(QPoint pos);
    QPoint getcenterpos();
    bool getTowerExsistence();
    bool containPos(QPoint pos);
    void setExsistence(bool exsist=true);

};

#endif // TOWERPOSTION_H
