#include <QtWidgets>
#include "assistant.h"
#include "selection.h"

Assistant::Assistant(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new InfoPage);
    setWindowTitle(tr("Assistant de configuration"));
}

void Assistant::accept()
{
    int num = field("numSommets").toInt();
    bool oriented = field("orienté").toBool();
    FenTable *fenTable = new FenTable(num,oriented);
    fenTable->show();

    QDialog::accept();
}

IntroPage::IntroPage(QWidget* parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    label = new QLabel(tr("Cet assistant vous aidera à configurer votre graphe. "
                          "Veuillez remplir les champs par les informations requises."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

InfoPage::InfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Informations"));
    nodesLabel = new QLabel(tr("&Nombre de sommets:"));
    orientedLabel = new QLabel(tr("Graphe &Orienté?"));
    numNodes = new QSpinBox;
    oriented = new QCheckBox;
    nodesLabel->setBuddy(numNodes);
    orientedLabel->setBuddy(oriented);

    registerField("numSommets",numNodes);
    registerField("orienté",oriented);


    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nodesLabel, 0, 0);
    layout->addWidget(numNodes, 0, 1);
    layout->addWidget(orientedLabel, 1, 0);
    layout->addWidget(oriented, 1, 1);
    setLayout(layout);
}

