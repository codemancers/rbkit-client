#ifndef RIBBONTOOLBAR_H
#define RIBBONTOOLBAR_H

#include <QWidget>
#include <QHash>
#include <QBoxLayout>

class QIcon;
class QHBoxLayout;
class QVBoxLayout;
class QTabBar;
class QToolButton;

class TabContainerWidget {
public:
    explicit TabContainerWidget(QWidget *_widget, int _index,
                                bool _inLayout, const QString _tabIdentifier,
                                QBoxLayout *layout)
        : widget(_widget), index(_index),
          inLayout(_inLayout), tabIdentifier(_tabIdentifier),
          containerLayout(layout) {

    }

    QWidget *widget;
    int index;
    bool inLayout;
    const QString tabIdentifier;
    QBoxLayout *containerLayout;

    void inline hideWidget(QBoxLayout *layout) {
        if (inLayout) {
            layout->removeWidget(widget);
            this->inLayout = false;
        }
        widget->hide();
    }

    void inline showWidget(QBoxLayout *layout) {
        if (!inLayout) {
            layout->addWidget(widget);
            this->inLayout = true;
        }
        widget->show();
    }
};

class RibbonToolBar : public QWidget
{
    Q_OBJECT

    QHBoxLayout *layout0;
    QVBoxLayout *layout1;
    QHash<QString, QToolButton *> toolBarButtons;
    QHash<QString, TabContainerWidget *> containerWidgets;
    QWidget *ribbonUiWidget;
    void insertRibbonWidget();
    QTabBar *tabBar;
    int tabIndexes;
public:
    explicit RibbonToolBar(QWidget *parent = 0);
    ~RibbonToolBar();
    void loadStyleSheet(const QString &fileName);
    QToolButton* addRibbonAction(const QString &actionName, const QString &actionIdentifier, const QIcon &icon);
    void addRibbonTab(const QString &tabName, const QString &tabIdentifier);
    QToolButton* addRibbonAction(const QString &actionName, const QString &actionIdentifier,
                         const QIcon &icon, const QString &tabIdentifier);
signals:

public slots:
    void tabSelectionChanged(int index);
};


void makeMarginSpacingZero(QBoxLayout *layout);
#endif // RIBBONTOOLBAR_H
