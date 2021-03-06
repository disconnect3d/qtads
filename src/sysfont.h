// This is copyrighted software. More information is at the end of this file.
#pragma once
#include <QColor>
#include <QFontInfo>
#include <QFontMetrics>

#include "config.h"
#include "htmlsys.h"

/* Tads HTML layer class whose interface needs to be implemented by the
 * interpreter.
 *
 * See htmltads/htmlsys.h and htmltads/notes/porting.htm for information
 * about this class.
 */
class CHtmlSysFontQt: public QFont, public CHtmlSysFont
{
private:
    QColor fColor;
    QColor fBgColor;

public:
    // When color() is a valid color (QColor::isValid()) it should be used as
    // the foreground color when drawing text in this font.
    const QColor& color() const
    {
        return fColor;
    }

    HTML_color_t htmlColor() const
    {
        return HTML_make_color(fColor.red(), fColor.green(), fColor.blue());
    }

    void color(HTML_color_t color)
    {
        fColor = QColor(HTML_color_red(color), HTML_color_green(color), HTML_color_blue(color));
    }

    // When bgColor() is a valid color (QColor::isValid()) it should be used as
    // the background color when drawing text in this font.
    const QColor& bgColor() const
    {
        return fBgColor;
    }

    HTML_color_t htmlBgColor() const
    {
        return HTML_make_color(fBgColor.red(), fBgColor.green(), fBgColor.blue());
    }

    void bgColor(HTML_color_t color)
    {
        fBgColor = QColor(HTML_color_red(color), HTML_color_green(color), HTML_color_blue(color));
    }

    bool operator==(const CHtmlSysFontQt& f) const
    {
        return QFont::operator==(f) and fColor == f.fColor and fBgColor == f.fBgColor;
    }

    CHtmlSysFontQt& operator=(const QFont& f)
    {
        QFont::operator=(f);
        return *this;
    }

    // Set the font descriptor.
    void set_font_desc(const CHtmlFontDesc* src)
    {
        desc_.copy_from(src);
        // Clear the explicit-face-name flag, since this is important only
        // when looking up a font.
        desc_.face_set_explicitly = false;
    }

    //
    // CHtmlSysFont interface implementation.
    //
    void get_font_metrics(CHtmlFontMetrics* m) override
    {
        // qDebug() << Q_FUNC_INFO << "called";

        QFontMetrics tmp(*this);

        m->ascender_height = tmp.ascent();
        m->descender_height = tmp.descent();
        m->total_height = tmp.height();
    }

    int is_fixed_pitch() override
    {
        return QFontInfo(*this).fixedPitch();
    }

    int get_em_size() override
    {
        return QFontInfo(*this).pixelSize();
    }
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
