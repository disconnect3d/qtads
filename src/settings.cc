// This is copyrighted software. More information is at the end of this file.
#include <QDebug>
#include <QFileInfo>
#include <QSettings>

#include "globals.h"
#include "settings.h"
#include "syswingroup.h"

static QFont fontForStyleHint(const QFont::StyleHint hint)
{
    QFont f;
    f.setStyleHint(hint);
    f.setFamily(f.defaultFamily());
    return f;
}

void Settings::loadFromDisk()
{
    QSettings sett;

    sett.beginGroup(QString::fromLatin1("media"));
    enableGraphics = sett.value(QString::fromLatin1("graphics"), true).toBool();
#ifndef NO_AUDIO
    enableSoundEffects = sett.value(QString::fromLatin1("sounds"), true).toBool();
    enableMusic = sett.value(QString::fromLatin1("music"), true).toBool();
#else
    enableSoundEffects = false;
    enableMusic = false;
#endif
    enableLinks = sett.value(QString::fromLatin1("links"), true).toBool();
    useSmoothScaling = sett.value(QString::fromLatin1("smoothImageScaling"), true).toBool();
    sett.endGroup();

    sett.beginGroup(QString::fromLatin1("colors"));
    mainBgColor = sett.value(QString::fromLatin1("mainbg"), QColor(Qt::white)).value<QColor>();
    mainTextColor = sett.value(QString::fromLatin1("maintext"), QColor(Qt::black)).value<QColor>();
    bannerBgColor =
        sett.value(QString::fromLatin1("bannerbg"), QColor(Qt::lightGray)).value<QColor>();
    bannerTextColor =
        sett.value(QString::fromLatin1("bannertext"), QColor(Qt::black)).value<QColor>();
    inputColor = sett.value(QString::fromLatin1("input"), QColor(70, 70, 70)).value<QColor>();
    underlineLinks = sett.value(QString::fromLatin1("underlinelinks"), false).toBool();
    highlightLinks = sett.value(QString::fromLatin1("highlightlinks"), true).toBool();
    unvisitedLinkColor =
        sett.value(QString::fromLatin1("unvisitedlinks"), QColor(Qt::blue)).value<QColor>();
    hoveringLinkColor =
        sett.value(QString::fromLatin1("hoveringlinks"), QColor(Qt::red)).value<QColor>();
    clickedLinkColor =
        sett.value(QString::fromLatin1("clickedlinks"), QColor(Qt::cyan)).value<QColor>();
    sett.endGroup();

    const auto def_serif = fontForStyleHint(QFont::Serif);
    const auto def_sans = fontForStyleHint(QFont::SansSerif);
    const auto def_fixed = fontForStyleHint(QFont::Monospace);
    const auto def_writer = fontForStyleHint(QFont::TypeWriter);
    const auto def_script = fontForStyleHint(QFont::Cursive);

    sett.beginGroup(QString::fromLatin1("fonts"));
    mainFont.fromString(sett.value(QString::fromLatin1("main"), def_serif).toString());
    fixedFont.fromString(sett.value(QString::fromLatin1("fixed"), def_fixed).toString());
    serifFont.fromString(sett.value(QString::fromLatin1("serif"), def_serif).toString());
    sansFont.fromString(sett.value(QString::fromLatin1("sans"), def_sans).toString());
    scriptFont.fromString(sett.value(QString::fromLatin1("script"), def_script).toString());
    writerFont.fromString(sett.value(QString::fromLatin1("typewriter"), def_writer).toString());
    inputFont.fromString(sett.value(QString::fromLatin1("input"), def_serif).toString());
    useMainFontForInput = sett.value(QString::fromLatin1("useMainFontForInput"), true).toBool();
    sett.endGroup();

    sett.beginGroup(QString::fromLatin1("misc"));
    ioSafetyLevelRead = sett.value(QString::fromLatin1("ioSafetyLevelRead"), 2).toInt();
    ioSafetyLevelWrite = sett.value(QString::fromLatin1("ioSafetyLevelWrite"), 2).toInt();
    tads2Encoding =
        sett.value(QString::fromLatin1("tads2encoding"), QByteArray("windows-1252")).toByteArray();
    pasteOnDblClk = sett.value(QString::fromLatin1("pasteondoubleclick"), true).toBool();
    softScrolling = sett.value(QString::fromLatin1("softscrolling"), true).toBool();
    askForGameFile = sett.value(QString::fromLatin1("askforfileatstart"), false).toBool();
    confirmRestartGame = sett.value(QString::fromLatin1("confirmrestartgame"), true).toBool();
    confirmQuitGame = sett.value(QString::fromLatin1("confirmquitgame"), true).toBool();
    textWidth = sett.value(QString::fromLatin1("linewidth"), textWidth).toInt();
    lastFileOpenDir =
        sett.value(QString::fromLatin1("lastFileOpenDir"), QString::fromLatin1("")).toString();
    sett.endGroup();

    sett.beginGroup(QString::fromLatin1("recent"));
    recentGamesList = sett.value(QString::fromLatin1("games"), QStringList()).toStringList();
    Q_ASSERT(recentGamesList.size() <= recentGamesCapacity);
    // Remove any files that don't exist or aren't readable.
    for (int i = 0; i < recentGamesList.size(); ++i) {
        QFileInfo file(recentGamesList.at(i));
        if (not file.exists() or not(file.isFile() or file.isSymLink()) or not file.isReadable()) {
            recentGamesList.removeAt(i);
            --i;
        }
    }
    sett.endGroup();

    appGeometry =
        sett.value(QString::fromLatin1("geometry/appgeometry"), QByteArray()).toByteArray();
    lastUpdateDate = sett.value(QString::fromLatin1("update/lastupdatedate"), QDate()).toDate();
    updateFreq = static_cast<UpdateFreq>(
        sett.value(QString::fromLatin1("update/updatefreq"), UpdateDaily).toInt());
}

void Settings::saveToDisk()
{
    QSettings sett;

    sett.beginGroup(QString::fromLatin1("media"));
    sett.setValue(QString::fromLatin1("graphics"), enableGraphics);
#ifndef NO_AUDIO
    sett.setValue(QString::fromLatin1("sounds"), enableSoundEffects);
    sett.setValue(QString::fromLatin1("music"), enableMusic);
#endif
    sett.setValue(QString::fromLatin1("links"), enableLinks);
    sett.setValue(QString::fromLatin1("smoothImageScaling"), useSmoothScaling);
    sett.endGroup();

    sett.beginGroup(QString::fromLatin1("colors"));
    sett.setValue(QString::fromLatin1("mainbg"), mainBgColor);
    sett.setValue(QString::fromLatin1("maintext"), mainTextColor);
    sett.setValue(QString::fromLatin1("bannerbg"), bannerBgColor);
    sett.setValue(QString::fromLatin1("bannertext"), bannerTextColor);
    sett.setValue(QString::fromLatin1("input"), inputColor);
    sett.setValue(QString::fromLatin1("underlinelinks"), underlineLinks);
    sett.setValue(QString::fromLatin1("highlightlinks"), highlightLinks);
    sett.setValue(QString::fromLatin1("unvisitedlinks"), unvisitedLinkColor);
    sett.setValue(QString::fromLatin1("hoveringlinks"), hoveringLinkColor);
    sett.setValue(QString::fromLatin1("clickedlinks"), clickedLinkColor);
    sett.endGroup();

    sett.beginGroup(QString::fromLatin1("fonts"));
    sett.setValue(QString::fromLatin1("main"), mainFont.toString());
    sett.setValue(QString::fromLatin1("fixed"), fixedFont.toString());
    sett.setValue(QString::fromLatin1("serif"), serifFont.toString());
    sett.setValue(QString::fromLatin1("sans"), sansFont.toString());
    sett.setValue(QString::fromLatin1("script"), scriptFont.toString());
    sett.setValue(QString::fromLatin1("typewriter"), writerFont.toString());
    sett.setValue(QString::fromLatin1("input"), inputFont.toString());
    sett.setValue(QString::fromLatin1("useMainFontForInput"), useMainFontForInput);
    sett.endGroup();

    sett.beginGroup(QString::fromLatin1("misc"));
    sett.setValue(QString::fromLatin1("ioSafetyLevelRead"), ioSafetyLevelRead);
    sett.setValue(QString::fromLatin1("ioSafetyLevelWrite"), ioSafetyLevelWrite);
    sett.setValue(QString::fromLatin1("tads2encoding"), tads2Encoding);
    sett.setValue(QString::fromLatin1("pasteondoubleclick"), pasteOnDblClk);
    sett.setValue(QString::fromLatin1("softscrolling"), softScrolling);
    sett.setValue(QString::fromLatin1("askforfileatstart"), askForGameFile);
    sett.setValue(QString::fromLatin1("confirmrestartgame"), confirmRestartGame);
    sett.setValue(QString::fromLatin1("confirmquitgame"), confirmQuitGame);
    sett.setValue(QString::fromLatin1("linewidth"), textWidth);
    sett.setValue(QString::fromLatin1("lastFileOpenDir"), lastFileOpenDir);
    sett.endGroup();

    sett.beginGroup(QString::fromLatin1("recent"));
    sett.setValue(QString::fromLatin1("games"), recentGamesList);
    sett.endGroup();

    sett.setValue(QString::fromLatin1("geometry/appgeometry"), qWinGroup->saveGeometry());
    sett.setValue(QString::fromLatin1("update/lastupdatedate"), lastUpdateDate);
    sett.setValue(QString::fromLatin1("update/updatefreq"), updateFreq);
    sett.sync();
}

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
