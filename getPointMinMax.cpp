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
        if (pointPath.typePointPath != 'A')
            for (const auto& point : pointPath.points) {
                if (pMM.pointMin.X > point.X) pMM.pointMin.X = point.X;
                if (pMM.pointMin.Y > point.Y) pMM.pointMin.Y = point.Y;
                if (pMM.pointMax.X < point.X) pMM.pointMax.X = point.X;
                if (pMM.pointMax.Y < point.Y) pMM.pointMax.Y = point.Y;
            }
        else {
            if (pMM.pointMin.X > pointPath.x - pointPath.rx) pMM.pointMin.X = pointPath.x - pointPath.rx;
            if (pMM.pointMin.Y > pointPath.y - pointPath.ry) pMM.pointMin.Y = pointPath.y - pointPath.ry;
            if (pMM.pointMax.X < (pointPath.x - pointPath.rx) + 2 * pointPath.rx) pMM.pointMax.X = (pointPath.x - pointPath.rx) + 2 * pointPath.rx;
            if (pMM.pointMax.Y < (pointPath.y - pointPath.ry) + 2 * pointPath.ry) pMM.pointMax.Y = (pointPath.y - pointPath.ry) + 2 * pointPath.ry;
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