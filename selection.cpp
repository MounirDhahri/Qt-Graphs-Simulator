#include "selection.h"
#include "ui_selection.h"
#include "graphwidget.h"
#include "simulation.h"

#include <QApplication>
#include <QTime>
#include <QMainWindow>
#include <QStringList>
#include <QMessageBox>

FenTable::FenTable(int num,bool oriented, QWidget *parent) :
    QWidget(parent),oriented(oriented),nodes(num),
    ui(new Ui::Selection)
{
    tab=ui->Table;
    ui->setupUi(this);
    ui->Table->setColumnCount(nodes);
    ui->Table->setRowCount(nodes);
    QStringList headers;
    for (int i=0; i<nodes; i++)
    {
        QString q=QString::number(i);
        headers<<q;
    }

    ui->Table->setVerticalHeaderLabels(headers);
    ui->Table->setHorizontalHeaderLabels(headers);

    for(int i=0; i<nodes; i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText("0");
        item->setBackgroundColor(Qt::gray);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->Table->setItem(i,i,item);
    }
    if (!oriented)
    {
        for (int i=0; i<nodes; i++)
        {
            for (int j=i+1; j<nodes; j++)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText("-");
                ui->Table->setItem(i,j,item);
            }
            for (int k=0; k<i; k++)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setBackgroundColor(Qt::darkGray);
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                ui->Table->setItem(i,k,item);
            }
        }
    }
    else
    {
        for (int i=0; i<nodes; i++)
        {
            for (int j=0; j<nodes; j++)
            {
                if (i != j){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText("-");
                    ui->Table->setItem(i,j,item);
                }
            }
        }
    }
    for (int i=0; i<nodes; i++)
    {
        ui->startNodeBox->addItem(QString::number(i));
        ui->finishNodeBox->addItem(QString::number(i));
    }
}

FenTable::~FenTable()
{
    delete ui;
}

void FenTable::on_Go_clicked()
{
    bool neg = false;
    if (!oriented)
    {
        for (int i=0; i<nodes; i++)
        {
            for (int j=0; j<i; j++)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(ui->Table->item(j,i)->text());
                ui->Table->setItem(i,j,item);
            }
        }
    }
    if (ui->algorithmsBox->currentText() == "Plus Court Chemin")
    {
        for (int i=0; i<nodes; i++)
        {
            for (int j=0; j<nodes; j++)
            {
                if (ui->Table->item(i,j)->text().toInt() < 0)
                {
                    QMessageBox::critical(this, "Valeur négative", "La recherche du plus court chemin implémente l'algorithme de Dijktra qui nécessite des valeurs positives de tout les arcs");
                    neg = true;
                }
                if (neg) break;
            }
            if (neg) break;
        }
    }
    if (!neg)
    {
    GraphWidget *widget = new GraphWidget(ui->Table,nodes);
    Simulation *sim = new Simulation(widget,ui->algorithmsBox->currentText(), ui->startNodeBox->currentText().toInt(), ui->finishNodeBox->currentText().toInt(), ui->fullSearch->isChecked());
    sim->show();
    this->showMinimized();
    }
}
