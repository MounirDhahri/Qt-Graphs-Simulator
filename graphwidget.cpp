#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "delay.h"

#include <math.h>
#include <climits>

#include <QKeyEvent>
#include <QQueue>
#include <QStack>

GraphWidget::GraphWidget(QTableWidget *table,int som, QWidget *parent)
    : QGraphicsView(parent), graph(table), numNodes(som)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Simulation"));
    int factor=100;
    if (numNodes<19) factor=200;
    for(int i=0;i<numNodes;i++)
    {
        nodes << new Node(this,i);
        scene->addItem(nodes[i]);
        if (i==0)
        {
            nodes[i]->setPos(50,360);
        }
        else if ((i-1)%4 == 0)
        {
            if (i==numNodes-1)
            {
                nodes[i]->setPos(((i-1)/4)*factor+factor+50,360);
            }
            else
                nodes[i]->setPos(((i-1)/4)*factor+factor+50,100);
        }
        else if ((((i-2)%4 == 0)&&(i!=numNodes-1)))
        {
            nodes[i]->setPos(((i-1)/4)*factor+factor+50,273);
        }
        else if ((((i-3)%4 == 0)&&(i!=numNodes-1)))
        {
            nodes[i]->setPos(((i-1)/4)*factor+factor+50,447);
        }
        else if ((((i-4)%4 == 0)&&(i!=numNodes-1)))
        {
            nodes[i]->setPos(((i-1)/4)*factor+factor+50,620);
        }
        else if (i==numNodes-1)
        {
            nodes[i]->setPos(((i-1)/4)*factor+2*factor+50,360);
        }
    }

    for(int i=0;i<numNodes;i++)
    {
        for(int j=0;j<numNodes;j++)
        {
            int value = table->item(i,j)->text().toInt();
            if((table->item(i,j)->text() != "-") && (value!=0))
            {
                scene->addItem(new Edge(nodes[i], nodes[j],value));
            }
        }
    }
}

int GraphWidget::minDistance(int dist[], bool sptSet[])
{
    int min = INT_MAX, min_index=0;

    for (int i = 0; i < numNodes; i++)
        if (!sptSet[i] && dist[i] < min)
            min = dist[i], min_index = i;

    return min_index;
}

void GraphWidget::startSim(QString alg, int start, int end,bool full)
{
    if(alg == "Parcours en Largeur")
    {
        QQueue<Node *> Q;
        Node *o = nodes[start];
        Q.enqueue(nodes[start]);
        nodes[start]->setExplored(true);
        if (!full)
        {
        while ((!Q.isEmpty()) && (o->getTag()!=end))
        {
            Node *n = Q.dequeue();
            n->setState(1);
            if(n->getTag()!=start)
            {
                o->setState(0);
                nodes[o->getTag()]->update();
            }
            n->update();
            foreach(Edge* e,n->edges())
            {
                if (!e->destNode()->isExplored())
                {
                    Q.enqueue(e->destNode());
                    e->destNode()->setExplored(true);
                }
            }
            o=n;
            Delay(2000);
        }
        }
        else
        {
            while (!Q.isEmpty())
            {
                Node *n = Q.dequeue();
                n->setState(1);
                if(n->getTag()!=start)
                {
                    o->setState(0);
                    nodes[o->getTag()]->update();
                }
                n->update();
                foreach(Edge* e,n->edges())
                {
                    if (!e->destNode()->isExplored())
                    {
                        Q.enqueue(e->destNode());
                        e->destNode()->setExplored(true);
                    }
                }
                o=n;
                Delay(2000);
            }
        }
        o->setState(0);
        nodes[o->getTag()]->update();
    }
    if(alg == "Parcours en Profondeur")
    {
        QStack<Node *> s;
        Node *o = nodes[start];
        s.push(nodes[start]);
        nodes[start]->setExplored(true);
        if (!full)
        {
        while ((!s.isEmpty()) && (o->getTag()!=end))
        {
            Node *n = s.pop();
            n->setState(1);
            if(n->getTag()!= start)
            {
                o->setState(0);
                nodes[o->getTag()]->update();
            }
            n->update();
            for(int i=n->edges().size()-1; i>=0;i--)
            {
                if (!n->edges()[i]->destNode()->isExplored())
                {
                    s.push(n->edges()[i]->destNode());
                    n->edges()[i]->destNode()->setExplored(true);
                }
            }
            o=n;
            Delay(2000);
        }
        }
        else
        {
            while (!s.isEmpty())
            {
                Node *n = s.pop();
                n->setState(1);
                if(n->getTag()!= start)
                {
                    o->setState(0);
                    nodes[o->getTag()]->update();
                }
                n->update();
                for(int i=n->edges().size()-1; i>=0;i--)
                {
                    if (!n->edges()[i]->destNode()->isExplored())
                    {
                        s.push(n->edges()[i]->destNode());
                        n->edges()[i]->destNode()->setExplored(true);
                    }
                }
                o=n;
                Delay(2000);
            }
        }
        o->setState(0);
        nodes[o->getTag()]->update();
    }

    if(alg == "Plus Court Chemin")
    {
        int parent[numNodes];
        int dist[numNodes];
        bool sptSet[numNodes];

        for (int i = 0; i < numNodes; i++)
        {
            dist[i] = INT_MAX;
            sptSet[i] = false;
        }
        dist[start] = 0;
        parent[start] = -1;
        for (int i = 0; i < numNodes-1; i++)
        {
            int u = minDistance(dist, sptSet);

            sptSet[u] = true;

            if (u==end) break;
            for (int i = 0; i < numNodes; i++)
            {
                int v = graph->item(u,i)->text().toInt();
                if (!sptSet[i] && v && dist[u] != INT_MAX && dist[u]+v < dist[i])
                {
                    dist[i] = dist[u] + v;
                    parent[i] = u;
                }
            }
        }

        int i = end;
        QList<Node *> sPath;
        sPath << nodes[i];
        while (i!=start)
        {
            i = parent[i];
            sPath<<nodes[i];
        }
        for(int i=sPath.size()-1; i>0;i--)
        {
            Edge *next=NULL;
            sPath[i]->setState(1);
            sPath[i]->update();
            Delay(1000);
            foreach(Edge *e, sPath[i]->edges())
            {
                if(e->destNode()==sPath[i-1]) next =e;
                e->setState(-1);
                e->update();
                Delay(1000);
            }
            foreach(Edge *e, sPath[i]->edges())
            {
                if(e->destNode()==sPath[i-1]) next =e;
                e->setState(0);
                e->update();
            }
            Delay(1000);
            next->setState(1);
            next->update();
            Delay(1000);
        }
        sPath[0]->setState(1);
        sPath[0]->update();
        Delay(1000);
    }
}
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
