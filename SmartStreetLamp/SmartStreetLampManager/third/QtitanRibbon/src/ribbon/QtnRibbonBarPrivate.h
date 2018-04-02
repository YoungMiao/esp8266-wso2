/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
** 
** Copyright (c) 2009-2017 Developer Machines (http://www.devmachines.com)
**           ALL RIGHTS RESERVED
** 
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and 
**  criminal penalties and will be prosecuted to the maximum extent 
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/
#ifndef QTN_RIBBONBARPRIVATE_H
#define QTN_RIBBONBARPRIVATE_H

#include <QStyle>
#include <QStack>
#include <QLabel>

#include "QtnRibbonBar.h"
#include "QtnRibbonTabBar.h"
#include "QtnRibbonCustomizePage.h"
#include "QtnRibbonQuickAccessBar.h"
#include "../styles/QtnCommonStylePrivate.h"

#include "QtitanDef.h"

QTITAN_BEGIN_NAMESPACE


class LogotypeLabel : public QLabel
{
public:
    LogotypeLabel(QWidget* parent);

public:
    Qt::AlignmentFlag alignmentLogotype() const  { return m_alignment; }
    void setAlignmentLogotype(Qt::AlignmentFlag alignment) { m_alignment = alignment; }

protected:
    virtual void paintEvent(QPaintEvent* p);

private:
    Qt::AlignmentFlag m_alignment;
};

class RibbonTabBar;
class RibbonTitleButton;
class OfficeFrameHelper;
class StyleOptionTitleBar;
class RibbonSystemButton;
class RibbonBackstageCloseButton;
class MenuMinimizedGroups;
class RibbonKeyTip;
class RibbonCustomizeManager;

class RibbonBarPrivate : public QObject
{
public:
    Q_OBJECT
    QTN_DECLARE_PUBLIC(RibbonBar)
public:
    explicit RibbonBarPrivate();
    virtual ~RibbonBarPrivate();
    static RibbonBarPrivate* _get(RibbonBar* ribbon);
    static const RibbonBarPrivate* _get(const RibbonBar* ribbon);
public:
    void init(); 
    void calcHeightItem();

    void layoutRibbon();
    void layoutTitleButton();
    bool isExistAssociativeTabWithPage(const RibbonTab* tab);
    void layoutContextHeaders();
    void layoutCorner();

    void removeAllContextHeaders();
    bool reduceContextHeaders();

    void updateMinimizedModePage(RibbonPage* page);
    void swapVisiblePages(int index);
    void setVisibleBackstage(bool visible);

    QAction* createSystemButton(const QIcon& icon, const QString& text);

    RibbonPage* insertPage(int indTab, int index);
    void insertPage(int indTab, int index, RibbonPage* page);
    void removePage(int index, bool deletePage = true);
    int indexPage(RibbonPage* page) const;

    bool validIndex(int index) const { return index >= 0 && index < m_listPages.count(); }
    int heightTabs() const;
    int topBorder() const;

    void titleBarOptions(StyleOptionTitleBar& opt) const;
    QStyle::SubControl getSubControl(const QPoint& pos) const;

    RibbonTitleButton* findTitleButton(QStyle::SubControl subControl) const;
    QAction* addTitleButton(QStyle::SubControl subControl, bool add, QRect& rcControl);

    ContextHeader* hitTestContextHeaders(const QPoint& point) const;
    QMenu* createContextMenu();

    void setKeyboardMode(bool b);
    void showKeyTips(QWidget* w);
    bool hideKeyTips();
    void createKeyTips();
    void createGroupKeyTips();
    void createWidgetKeyTips(RibbonGroup* group, QWidget* widget, const QString& prefix, const QRect& rcGroups, bool visible);
    bool createPopupKeyTips(QMenu* levalMenu);
    void destroyKeyTips();
    void calcPositionKeyTips();
    int rowItemHeight() const;
    int rowItemCount() const;
    int maxGroupHeight() const;
    void createCustomizeDialog();

private Q_SLOTS:
    void processClickedSubControl();
    void toggledCustomizeBar();
    void toggledQuickAccessBarPos();
    void toggledMinimized();
    void currentChanged(int index);

protected:
    bool pressTipCharEvent(const QKeyEvent* key);
    virtual bool eventFilter(QObject*, QEvent* event);

public:
    QRect m_rcPageArea;
    QRect m_rcTabBar;
    int m_currentIndexPage;
    int m_heightTabs;
    RibbonTabBar* m_tabBar;
    RibbonQuickAccessBar* m_quickAccessBar; 
    RibbonSystemButton* m_controlSystemButton;
    QVector<QWidget*> m_visibleWidgets;

    OfficeFrameHelper* m_frameHelper;
    QRect m_rcTitle;
    QRect m_rcHeader;
    QRect m_rcTitleText;
    QRect m_rcQuickAccess;

    bool m_showTitleAlways;
    bool m_ribbonBarVisible;
    bool m_minimizationEnabled;
    bool m_minimized;
    bool m_keyboardState;
    bool m_altPressed;
    bool m_keyTipsComplement;
    bool m_keyTipsEnabled;
    bool m_keyTipsShowing;
    bool m_titleGroupsVisible;
    QWidget* m_keyboardFocusWidget;
    RibbonBar::QuickAccessBarPosition m_quickAccessBarPosition;
    RibbonBar::TabBarPosition m_tabBarPosition;

    QAction* m_actCloseButton;
    QAction* m_actNormalButton;
    QAction* m_actMaxButton;
    QAction* m_actMinButton;

    QList<ContextHeader*> m_listContextHeaders;
    QList<RibbonKeyTip*> m_keyTips;
    QStack<QWidget*> m_levels;
    int m_countKey;

    QStyle::SubControl m_hoveredSubControl;
    QStyle::SubControl m_activeSubControl;
    QString m_windowTitle;

    LogotypeLabel* m_logotypeLabel;

    QPixmap m_pixTitleBackground;
    RibbonCustomizeManager* m_customizeManager;
    RibbonCustomizeDialog* m_customizeDialog;
protected:
    QList<RibbonPage*> m_listPages;
    int m_rowItemHeight;
    int m_rowItemCount;
private:
    Q_DISABLE_COPY(RibbonBarPrivate)
};

QTITAN_END_NAMESPACE

#endif // QTN_RIBBONPRIVATE_H


