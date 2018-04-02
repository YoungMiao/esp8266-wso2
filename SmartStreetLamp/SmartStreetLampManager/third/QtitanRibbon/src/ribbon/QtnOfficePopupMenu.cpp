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
#include <QApplication>
#include <QStyleOption>
#include <QWidgetAction>
#include <QPainter>
#include <qevent.h>
#include <QLayout>

#include "QtnRibbonStyle.h"
#include "QtnOfficePopupMenu.h"
#include "QtnRibbonGallery.h"
#include "QtnRibbonBarPrivate.h"
#ifdef DEBUG_MEMORY_ENABLED
#include "QtitanMSVSDebug.h"
#endif

QTITAN_USE_NAMESPACE


/*!
\property OfficePopupMenu::gripVisible
*/

QTITAN_BEGIN_NAMESPACE

class OfficePopupMenuPrivate : public QObject
{
public:
    QTN_DECLARE_PUBLIC(OfficePopupMenu)
public:
    explicit OfficePopupMenuPrivate();

public:
    void init();
    int calcMinimumHeight(bool without = false);
    QWidget* findWidget(const char* nameWidget) const;
    void udateSizeGallery(const QSize& sz);

public:
    QWidget* m_widgetPopup; 
    bool m_resizable      : 1;
    bool m_pressSizeGrip  : 1;
    bool m_showGrip       : 1;
    int m_lastWidth;
};

QTITAN_END_NAMESPACE

OfficePopupMenuPrivate::OfficePopupMenuPrivate()
{
    m_widgetPopup = Q_NULL;
    m_resizable = false;
    m_pressSizeGrip = false;
    m_showGrip = true;
    m_lastWidth = -1;
}

void OfficePopupMenuPrivate::init()
{
    QTN_P(OfficePopupMenu);
    p.setProperty(_qtn_PopupBar, true);
    p.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p.setMouseTracking(true);
}

int OfficePopupMenuPrivate::calcMinimumHeight(bool without)
{
    QTN_P(OfficePopupMenu);

    int height = 11;

    QList<QAction*> listActions = p.actions();
    for (int i = 0; i < listActions.count(); i++) 
    {
        if (QWidgetAction* widgetAction = qobject_cast<QWidgetAction*>(listActions.at(i)))
        {
            if (QWidget* w = widgetAction->defaultWidget())
            {
                if (!without)
                {
                    int minHeight = w->minimumSizeHint().height();
                    if (minHeight == -1)
                        minHeight = w->sizeHint().height();
                    height += minHeight;
                }
            }
        }
        else
            height += p.actionGeometry(listActions.at(i)).height();
    }
    return height;
}

QWidget* OfficePopupMenuPrivate::findWidget(const char* nameWidget) const
{
    QTN_P(const OfficePopupMenu);
    const QObjectList& listChildren = p.children();
    for (int i = 0; i < listChildren.size(); ++i) 
    {
        QWidget* w = qobject_cast<QWidget*>(listChildren.at(i));

        if (w && w->property(nameWidget).toBool() )
            return w;
    }
    return Q_NULL;
}

void OfficePopupMenuPrivate::udateSizeGallery(const QSize& sz)
{
    QTN_P(const OfficePopupMenu);
    if (QWidget* w = findWidget(_qtn_WidgetGallery))
    {
        if (!w->isWindow() && !w->isHidden())
        {
            QSize szOld = p.sizeHint();
            QSize sizeWidget = w->geometry().size();
            int offset = szOld.height() - sz.height();
            sizeWidget.setHeight(sizeWidget.height() - offset);
            RibbonGallery* gallery = (RibbonGallery*)w;
            gallery->updatelayout();
            QRect rect(QPoint(0, 0), sizeWidget);
            w->setGeometry(rect.adjusted(1, 1, -1, 1));
        }
    }
}

/*!
class OfficePopupMenu
\brief Constructs the OfficePopupMenu object with the given \a parent.
*/ 
OfficePopupMenu::OfficePopupMenu(QWidget* parent)
    : QMenu(parent)
{
    QTN_INIT_PRIVATE(OfficePopupMenu);
    QTN_D(OfficePopupMenu);
    d.init();

    connect(this, SIGNAL(aboutToShow()), this, SLOT(aboutToShowBar()));
}


/*!
\brief Destructor of the OfficePopupMenu object.
*/ 
OfficePopupMenu::~OfficePopupMenu()
{
    QTN_FINI_PRIVATE();
}

/*!
\brief Creates an instance of the OfficePopupMenu object with given \a parent.
*/ 
OfficePopupMenu* OfficePopupMenu::createPopupMenu(QWidget* parent)
{
    OfficePopupMenu* popupBar = new OfficePopupMenu(parent);
    return popupBar;
}

/*!
\brief Adds \a widget to the Office Popup Window and return an action associated with them.
*/ 
QAction* OfficePopupMenu::addWidget(QWidget* widget)
{
    QTN_D(OfficePopupMenu);

    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    addAction(action);

    if (widget && widget->property("isResizable").toBool())
        d.m_resizable = true;

    if (RibbonGallery* gallery = qobject_cast<RibbonGallery*>(d.findWidget(_qtn_WidgetGallery)))
    {
        gallery->adjustSize();
        gallery->updatelayout();
    }
    return action;
}

/*!
\brief Sets visibility of the Popup Window Grip to \a visible.
*/ 
void OfficePopupMenu::setGripVisible(bool visible)
{
    QTN_D(OfficePopupMenu);
    d.m_showGrip = visible;
}

/*!
\brief Returns the visibility of the Popup Window Grip.
*/ 
bool OfficePopupMenu::isGripVisible() const
{
    QTN_D(const OfficePopupMenu);
    return d.m_showGrip ;
}

/*! \internal */
void OfficePopupMenu::setWidgetBar(QWidget* widget)
{
    QTN_D(OfficePopupMenu);
    Q_ASSERT(widget != Q_NULL && d.m_widgetPopup == Q_NULL);
    d.m_widgetPopup = widget;
}

/*! \internal */
void OfficePopupMenu::aboutToShowBar()
{
    QTN_D(OfficePopupMenu);

    if (RibbonGallery* gallery = qobject_cast<RibbonGallery*>(d.findWidget(_qtn_WidgetGallery)))
    {
        if (!gallery->isWindow() && !gallery->isHidden())
            gallery->updatelayout();
    }
}

/*! \reimp */
QSize OfficePopupMenu::sizeHint() const
{
    QSize size = QMenu::sizeHint();
    
    QTN_D(const OfficePopupMenu);

    int height = 0;
    if (d.m_resizable)
        height = 11;

    QSize sz;
    if (d.m_widgetPopup)
        sz = d.m_widgetPopup->geometry().size();
    else if (QWidget* w = d.findWidget(_qtn_WidgetGallery))
    {
        sz = w->geometry().size();
        if (sz.isNull())
        {
            sz = size;
            sz = QSize(qMin(size.width(), sz.width()), size.height());
        }
    }
    size.setWidth(d.m_lastWidth != -1 ? d.m_lastWidth : sz.width() + 3);
    size.setHeight(size.height() + height);
    return size;
}

/*! \reimp */
void OfficePopupMenu::paintEvent(QPaintEvent* event)
{
    QMenu::paintEvent(event);
    QTN_D(OfficePopupMenu);

    if (d.m_resizable)
    {
        QPainter p(this);
        QStyleOptionSizeGrip opt;
        opt.init(this);
        opt.rect.adjust(1, 0, -1, -1);
        opt.rect.setTop(opt.rect.bottom() - 11);
        style()->drawControl((QStyle::ControlElement)RibbonStyle::CE_PopupSizeGrip, &opt, &p, this);
    }
}

/*! \reimp */
void OfficePopupMenu::mousePressEvent(QMouseEvent* event)
{
    QTN_D(OfficePopupMenu);
    if (d.m_resizable)
    {
        if (event->buttons() == Qt::LeftButton) 
        {
            QRect rcResizeGripper(rect());
            rcResizeGripper.setTop(rcResizeGripper.bottom() - 11);
            QRect rcResizeGripperAll = rcResizeGripper;
            rcResizeGripper.setLeft(rcResizeGripper.right() - rcResizeGripper.height());
            if (rcResizeGripper.contains(event->pos()))
            {
                if (QWidget* w = d.findWidget(_qtn_WidgetGallery))
                {
                    if (w && !w->isWindow() && !w->isHidden())
                        w->setAttribute(Qt::WA_TransparentForMouseEvents, true);
                }
                d.m_pressSizeGrip = true;
                return;
            }
            else if (rcResizeGripperAll.contains(event->pos()))
                return;
        }
    }
    QMenu::mousePressEvent(event);
}

/*! \reimp */
void OfficePopupMenu::mouseMoveEvent(QMouseEvent* event)
{
    QTN_D(OfficePopupMenu);

    if (QWidget* w = d.findWidget(_qtn_WidgetGallery))
    {
        QMouseEvent evPress(event->type(), event->pos(), event->globalPos(), event->button(), event->buttons(), event->modifiers());
        QApplication::sendEvent(w, &evPress);
    }

    if (d.m_resizable)
    {
        QRect rcResizeGripper(rect());
        rcResizeGripper.setTop(rcResizeGripper.bottom() - 11);
        rcResizeGripper.setLeft(rcResizeGripper.right() - rcResizeGripper.height());

        if (d.m_pressSizeGrip || rcResizeGripper.contains(event->pos()))
            setCursor(Qt::SizeFDiagCursor);
        else
            unsetCursor();

        if (d.m_pressSizeGrip)
        {
            QPoint np(event->globalPos());

            QRect rect = geometry();
            rect.setRight(np.x());
            rect.setBottom(np.y());

            d.m_lastWidth = rect.width();
            setGeometry(rect);
            return;
        }
    }

    if (!d.m_pressSizeGrip)
        QMenu::mouseMoveEvent(event);
}

/*! \reimp */
void OfficePopupMenu::mouseReleaseEvent(QMouseEvent* event)
{
    QTN_D(OfficePopupMenu);
    d.m_pressSizeGrip = false;

    if (QWidget* w = d.findWidget(_qtn_WidgetGallery))
    {
        if (w && !w->isWindow() && !w->isHidden())
            w->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
    QMenu::mouseReleaseEvent(event);
}

/*! \reimp */
void OfficePopupMenu::moveEvent(QMoveEvent* event)
{
    Q_UNUSED(event);
    QTN_D(OfficePopupMenu);
    if (d.m_pressSizeGrip)
        setCursor(Qt::SizeFDiagCursor);
}

/*! \reimp */
void OfficePopupMenu::showEvent(QShowEvent* event)
{
    QTN_D(OfficePopupMenu);
    QMenu::showEvent(event);

    if (QWidget* w = d.findWidget(_qtn_WidgetGallery))
    {
        RibbonGallery* gallery = (RibbonGallery*)w;
        gallery->updatelayout();
    }

    if (d.m_widgetPopup && d.m_resizable)
    {
        setMinimumWidth(d.m_widgetPopup->geometry().width() + 4);
        setMinimumHeight(d.calcMinimumHeight());
    }
    else if (QWidget* w = d.findWidget(_qtn_WidgetGallery))
    {
        if (!d.m_resizable && !w->isWindow() && !w->isHidden())
            w->setGeometry(QRect(QPoint(1, 0), w->sizeHint()));
    }
}

/*! \reimp */
void OfficePopupMenu::resizeEvent(QResizeEvent* event)
{
    QTN_D(OfficePopupMenu);
    if (d.m_resizable)
        d.udateSizeGallery(event->size());
    QMenu::resizeEvent(event);
}

