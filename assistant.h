#ifndef CLASSWIZARD_H
#define CLASSWIZARD_H

#include <QWizard>
#include <QSpinBox>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE

class Assistant : public QWizard
{
    Q_OBJECT

public:
    Assistant(QWidget *parent = 0);

    void accept() Q_DECL_OVERRIDE;
};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};

class InfoPage : public QWizardPage
{
    Q_OBJECT

public:
    InfoPage(QWidget *parent = 0);

private:
    QLabel *nodesLabel;
    QLabel *orientedLabel;
    QSpinBox *numNodes;
    QCheckBox *oriented;
};

#endif
