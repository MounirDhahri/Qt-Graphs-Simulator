#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QFont>


Node::Node(GraphWidget *graphWidget, int i)
    : graph(graphWidget), tag(i), active(-1), explored(false)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

void Node::addIncoming(Edge *edge)
{
    incomingEdgesList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

QList<Node *> Node::connected()
{
    foreach(Edge *e, edgeList) connectedNodes<<e->destNode();
    foreach(Edge *e, incomingEdgesList)
    {
        if (! connectedNodes.contains(e->sourceNode())) connectedNodes<<e->sourceNode();
    }
    std::sort(connectedNodes.begin(),connectedNodes.end());
    return connectedNodes;
}

bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -15 - adjust, -15 - adjust, 35 + adjust, 35 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-15, -15, 30, 30);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    /*painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-10, -10, 30, 30);
    */
    QRadialGradient gradient(-3, -3, 15);
    gradient.setCenter(3, 3);
    gradient.setFocalPoint(3, 3);
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    }
    else if (active==1)
    {
        gradient.setColorAt(1, QColor(Qt::green));
        gradient.setColorAt(0, QColor(Qt::darkGreen));
    }
    else if (active==0)
    {
        gradient.setColorAt(0, Qt::gray);
        gradient.setColorAt(1, Qt::darkGray);
    }
    else
    {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-15, -15, 30, 30);
    QFont font=painter->font() ;
    font.setPointSize ( 12 );
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);
    if(tag>9) painter->drawText(-9,6,QString::number(tag));
    else painter->drawText(-5,6,QString::number(tag));
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        foreach (Edge *edge, incomingEdgesList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
