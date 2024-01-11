#include "stdafx.h"
#include "classSVG.h"

void RectSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.X > this->p.X) pMM.pointMin.X = this->p.X;
    if (pMM.pointMin.Y > this->p.Y) pMM.pointMin.Y = this->p.Y;
    if (pMM.pointMax.X < this->p.X + this->width) pMM.pointMax.X = this->p.X + this->width;
    if (pMM.pointMax.Y < this->p.Y + this->height) pMM.pointMax.Y = this->p.Y + this->height;
}
void TextSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.X > this->p.X) pMM.pointMin.X = this->p.X;
    if (pMM.pointMin.Y > this->p.Y - this->fontSize) pMM.pointMin.Y = this->p.Y - this->fontSize;
    if (pMM.pointMax.X < this->p.X + this->textContent.size() * this->fontSize) pMM.pointMax.X = this->p.X + this->textContent.size() * this->fontSize;
    if (pMM.pointMax.Y < this->p.Y) pMM.pointMax.Y = this->p.Y;
}
void CircleSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.X > this->c.X - this->r) pMM.pointMin.X = this->c.X - this->r;
    if (pMM.pointMin.Y > this->c.Y - this->r) pMM.pointMin.Y = this->c.Y - this->r;
    if (pMM.pointMax.X < this->c.X + this->r) pMM.pointMax.X = this->c.X + this->r;
    if (pMM.pointMax.Y < this->c.Y + this->r) pMM.pointMax.Y = this->c.Y + this->r;
}
void EllipseSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.X > this->c.X - this->rx) pMM.pointMin.X = this->c.X - this->rx;
    if (pMM.pointMin.Y > this->c.Y - this->ry) pMM.pointMin.Y = this->c.Y - this->ry;
    if (pMM.pointMax.X < (this->c.X - this->rx) + 2 * rx) pMM.pointMax.X = (this->c.X - this->rx) + 2 * rx;
    if (pMM.pointMax.Y < (this->c.Y - this->ry) + 2 * ry) pMM.pointMax.Y = (this->c.Y - this->ry) + 2 * ry;
}
void LineSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.X > this->p1.X) pMM.pointMin.X = this->p1.X;
    if (pMM.pointMin.Y > this->p1.Y) pMM.pointMin.Y = this->p1.Y;
    if (pMM.pointMax.X < this->p1.X) pMM.pointMax.X = this->p1.X;
    if (pMM.pointMax.Y < this->p1.Y) pMM.pointMax.Y = this->p1.Y;
    if (pMM.pointMin.X > this->p2.X) pMM.pointMin.X = this->p2.X;
    if (pMM.pointMin.Y > this->p2.Y) pMM.pointMin.Y = this->p2.Y;
    if (pMM.pointMax.X < this->p2.X) pMM.pointMax.X = this->p2.X;
    if (pMM.pointMax.Y < this->p2.Y) pMM.pointMax.Y = this->p2.Y;
}
void PolygonSVG::getPointMINMAX(pointMinMax& pMM) {
    for (int i = 0; i < this->points.size(); i++)
    {
        if (pMM.pointMin.X > this->points[i].X) pMM.pointMin.X = this->points[i].X;
        if (pMM.pointMin.Y > this->points[i].Y) pMM.pointMin.Y = this->points[i].Y;
        if (pMM.pointMax.X < this->points[i].X) pMM.pointMax.X = this->points[i].X;
        if (pMM.pointMax.Y < this->points[i].Y) pMM.pointMax.Y = this->points[i].Y;
    }
}
void PolylineSVG::getPointMINMAX(pointMinMax& pMM) {
    for (int i = 0; i < this->points.size(); i++)
    {
        if (pMM.pointMin.X > this->points[i].X) pMM.pointMin.X = this->points[i].X;
        if (pMM.pointMin.Y > this->points[i].Y) pMM.pointMin.Y = this->points[i].Y;
        if (pMM.pointMax.X < this->points[i].X) pMM.pointMax.X = this->points[i].X;
        if (pMM.pointMax.Y < this->points[i].Y) pMM.pointMax.Y = this->points[i].Y;
    }
}
void PathSVG::getPointMINMAX(pointMinMax& pMM) {
    for (const auto& pointPath : PathData)
    {
        Point p(0, 0);
        if (pointPath.typePointPath == 'A')
        {
            for (const auto& Ash : pointPath.As) {
                if (pMM.pointMin.X > Ash.x - Ash.rx) pMM.pointMin.X = Ash.x - Ash.rx;
                if (pMM.pointMin.Y > Ash.y - Ash.ry) pMM.pointMin.Y = Ash.y - Ash.ry;
                if (pMM.pointMax.X < (Ash.x - Ash.rx) + 2 * Ash.rx) pMM.pointMax.X = (Ash.x - Ash.rx) + 2 * Ash.rx;
                if (pMM.pointMax.Y < (Ash.y - Ash.ry) + 2 * Ash.ry) pMM.pointMax.Y = (Ash.y - Ash.ry) + 2 * Ash.ry;
                p.X = Ash.x;
                p.Y = Ash.y;
            }
        }
        else if (pointPath.typePointPath == 'a')
        {
            for (const auto& Ash : pointPath.As)
            {
                if (pMM.pointMin.X > Ash.x - Ash.rx) pMM.pointMin.X = Ash.x - Ash.rx;
                if (pMM.pointMin.Y > Ash.y - Ash.ry) pMM.pointMin.Y = Ash.y - Ash.ry;
                if (pMM.pointMax.X < (Ash.x - Ash.rx) + 2 * Ash.rx) pMM.pointMax.X = (Ash.x - Ash.rx) + 2 * Ash.rx;
                if (pMM.pointMax.Y < (Ash.y - Ash.ry) + 2 * Ash.ry) pMM.pointMax.Y = (Ash.y - Ash.ry) + 2 * Ash.ry;
                p.X = p.X + Ash.x;
                p.Y = p.Y + Ash.y;
            }
        }
        else if (pointPath.typePointPath > 'A' && pointPath.typePointPath < 'Z') {
            for (const auto& point : pointPath.points) {
                if (pMM.pointMin.X > point.X) pMM.pointMin.X = point.X;
                if (pMM.pointMin.Y > point.Y) pMM.pointMin.Y = point.Y;
                if (pMM.pointMax.X < point.X) pMM.pointMax.X = point.X;
                if (pMM.pointMax.Y < point.Y) pMM.pointMax.Y = point.Y;
                p.X = point.X;
                p.Y = point.Y;
            }
        }
        else if (pointPath.typePointPath > 'a' && pointPath.typePointPath < 'z') {
            for (const auto& point : pointPath.points) {
                if (pMM.pointMin.X > p.X + point.X) pMM.pointMin.X = p.X + point.X;
                if (pMM.pointMin.Y > p.Y + point.Y) pMM.pointMin.Y = p.Y + point.Y;
                if (pMM.pointMax.X < p.X + point.X) pMM.pointMax.X = p.X + point.X;
                if (pMM.pointMax.Y < p.Y + point.Y) pMM.pointMax.Y = p.Y + point.Y;
                p.X = p.X + point.X;
                p.Y = p.Y + point.Y;
            }
        }

    }
}
void GroupSVG::getPointMINMAX(pointMinMax& ptMM) {
    for (const auto& element : elements) {
        if (element.type == GroupOrShape::SHAPE) {
            element.shape->getPointMINMAX(ptMM);
        }
        else if (element.type == GroupOrShape::GROUP) {
            element.group->getPointMINMAX(ptMM);
        }
    }
}