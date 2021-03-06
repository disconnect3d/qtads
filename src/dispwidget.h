// This is copyrighted software. More information is at the end of this file.
#pragma once
#include <QDebug>
#include <QTime>
#include <QWidget>

#include "config.h"

/* The widget where CHtmlSysWin* performs actual paint operations.  It also
 * handles mouse events.
 */
class DisplayWidget: public QWidget
{
    Q_OBJECT

private:
    // We track the current link the mouse is currently hovering over and the
    // link over which the mouse button has been pressed but not released yet.
    class CHtmlDispLink* fHoverLink;
    class CHtmlDispLink* fClickedLink;

    // Origin of current selection range.
    QPoint fSelectOrigin;

    // Do we currently have a selection?
    bool fHasSelection;

    // Drag start position.
    QPoint fDragStartPos;

    // Time of last double-click event.
    QTime fLastDoubleClick;

    // Stop tracking links.
    void fInvalidateLinkTracking();

    // Returns the text currently selected in this window.
    QString fMySelectedText();

    void fHandleDoubleOrTripleClick(QMouseEvent* e, bool tripleClick);

    void fSyncClipboard();

protected:
    // Are we in text selection mode?
    bool inSelectMode;

    // The window we're embeded in.
    class CHtmlSysWinQt* parentSysWin;

    // Our parent's formatter, for easy access.
    class CHtmlFormatter* formatter;

    // Holds the widget that currently has an active selection range, or null
    // if there's no active selection.
    static DisplayWidget* curSelWidget;

    void paintEvent(QPaintEvent* e) override;

    void mouseMoveEvent(QMouseEvent* e) override;

    void leaveEvent(QEvent* e) override;

    void mousePressEvent(QMouseEvent* e) override;

    void mouseReleaseEvent(QMouseEvent* e) override;

    void mouseDoubleClickEvent(QMouseEvent* e) override;

public:
    DisplayWidget(class CHtmlSysWinQt* parent, class CHtmlFormatter* formatter);
    ~DisplayWidget() override;

    // When our parent's notify_clear_contents() is called, we need to know
    // about it so we can perform link tracking invalidation.
    void notifyClearContents()
    {
        // When clearing contents, the display items are already gone. Set them
        // Null so we won't try to access them.
        fClickedLink = fHoverLink = 0;
        fInvalidateLinkTracking();
    }

    // Clear the selection range.
    virtual void clearSelection();

    // Get the text contained in the currently active selection. Returns a
    // null string is there's currently no display widget with an active
    // selection.
    static QString selectedText();

    // Update link tracking for specified mouse position.  If the specified
    // position isNull(), it will be autodetected.
    //
    // TODO: What happens with multi-pointer systems?
    void updateLinkTracking(const QPoint& pos);
};

/*
    Copyright 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2018, 2019 Nikos
    Chantziaras.

    This file is part of QTads.

    QTads is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any later
    version.

    QTads is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
    details.

    You should have received a copy of the GNU General Public License along
    with QTads. If not, see <https://www.gnu.org/licenses/>.
*/
