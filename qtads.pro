QT += network widgets
QT_CONFIG -= no-pkg-config
TEMPLATE = app
CONFIG += silent warn_off strict_c strict_c++ c11 c++14 gc_binaries

VERSION_MAJOR = 2
VERSION_MINOR = 1
VERSION_PATCH = 7
VERSION = "$$VERSION_MAJOR"."$$VERSION_MINOR"."$$VERSION_PATCH"
DEFINES += QTADS_VERSION=\\\"$$VERSION\\\"

lessThan(QT_MAJOR_VERSION, 5) {
    error(Qt 4 is not supported. You need at least Qt 5.9.)
}
equals(QT_MAJOR_VERSION, 5) : lessThan(QT_MINOR_VERSION, 9) {
    error(Qt 5.9 or higher is required. You are using Qt "$$QT_MAJOR_VERSION"."$$QT_MINOR_VERSION".)
}

# Mac OS application and file icons.
macx {
    ICON = QTads.icns
    OtherIcons.files = QTadsGameFile.icns
    OtherIcons.path = Contents/Resources
    QMAKE_BUNDLE_DATA += OtherIcons
    QMAKE_INFO_PLIST = Info.plist
}

# MS Windows executable resource.
win32 {
    RC_FILE += qtads.rc
    RC_DEFINES += \
        QTADS_VERSION=\""$$VERSION"\" \
        W32_RC_FILEVERSION=\""$$VERSION_MAJOR","$$VERSION_MINOR","$$VERSION_PATCH",00\" \
        W32_RC_PRODUCTVERSION=\""$$VERSION_MAJOR","$$VERSION_MINOR","$$VERSION_PATCH",00\"

    *-g++* {
        QMAKE_CFLAGS += -march=i686 -mtune=generic
        QMAKE_CXXFLAGS += -march=i686 -mtune=generic
    }
}

macx | win32 {
    DEFINES += OS_NO_TYPES_DEFINED
    TARGET = QTads
} else {
    TARGET = qtads
}

RESOURCES += resources.qrc

# We use warn_off to allow only default warnings, not to supress them all.
QMAKE_CXXFLAGS_WARN_OFF =
QMAKE_CFLAGS_WARN_OFF =

*-g++* | *-clang* {
    # For stat() and lstat().
    DEFINES += _SVID_SOURCE _DEFAULT_SOURCE

    # The code does a lot of type punning, which breaks ANSI/ISO strict
    # aliasing rules.
    QMAKE_CXXFLAGS += -fno-strict-aliasing
    QMAKE_CFLAGS += -fno-strict-aliasing

    # TADS 3 doesn't build with C++14 sized delete.
    QMAKE_CXXFLAGS += -fno-sized-deallocation

    # Avoid a flood of "unused parameter" warnings.
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
    QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter
}

disable-audio {
    DEFINES += NO_AUDIO
} else {
    CONFIG(debug, release|debug) {
        DEFINES += AULIB_DEBUG
    }

    DEFINES += \
        AULIB_STATIC_DEFINE \
        SPX_RESAMPLE_EXPORT= \
        RANDOM_PREFIX=SDL_audiolib \
        OUTSIDE_SPEEX

    CONFIG += link_pkgconfig
    PKGCONFIG += sdl2 sndfile libmpg123 fluidsynth vorbisfile

    INCLUDEPATH += \
        SDL_audiolib \
        SDL_audiolib/include \
        SDL_audiolib/resampler \
        SDL_audiolib/src

    HEADERS += \
        "$$PWD"/SDL_audiolib/include/Aulib/*.h \
        "$$PWD"/SDL_audiolib/src/*.h \
        "$$PWD"/SDL_audiolib/*.h

    SOURCES += \
        SDL_audiolib/resampler/resample.c \
        SDL_audiolib/src/AudioDecoder.cpp \
        SDL_audiolib/src/AudioDecoderFluidsynth.cpp \
        SDL_audiolib/src/AudioDecoderMpg123.cpp \
        SDL_audiolib/src/AudioDecoderSndfile.cpp \
        SDL_audiolib/src/AudioDecoderVorbis.cpp \
        SDL_audiolib/src/AudioResampler.cpp \
        SDL_audiolib/src/AudioResamplerSpeex.cpp \
        SDL_audiolib/src/AudioStream.cpp \
        SDL_audiolib/src/Stream.cpp \
        SDL_audiolib/src/audiostream_p.cpp \
        SDL_audiolib/src/aulib.cpp \
        SDL_audiolib/src/sampleconv.cpp
}

DEFINES += \
    QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_TO_ASCII \
    QT_DEPRECATED_WARNINGS \
    QT_USE_QSTRINGBUILDER \
    QTADS \
    _M_LE_C11 \
    T3_COMPILING_FOR_HTML \
    VM_FLAT_POOL \
    USE_HTML \
    TC_TARGET_T3

#CONFIG(release, debug|release) {
#    DEFINES += VMGLOB_VARS
#} else {
#    DEFINES += VMGLOB_PARAM TADSHTML_DEBUG T3_DEBUG NEW_DELETE_NEED_THROW
#}
DEFINES += VMGLOB_VARS

# These macros override some default buffer-sizes the T3VM uses for UNDO
# operations.  QTads runs on systems with a lot of RAM (compared to DOS), so we
# could increase these buffers.  Normally, the player would only be able to
# UNDO turns in a T3 game only 10 times or so.  With larger UNDO-buffers, more
# turns are possible.  See tads3/vmparam.h for an explanation.  Note that
# VM_UNDO_MAX_SAVEPTS can't be larger than 255.
#
# This, however, incurs quite a bit of overhead, slowing game response down
# over time, which is why we disable this for now.
#DEFINES += \
#   VM_STACK_SIZE=65536 \
#   VM_STACK_RESERVE=1024 \
#   VM_UNDO_MAX_RECORDS=65536 \
#   VM_UNDO_MAX_SAVEPTS=255

# Where to find the portable Tads sources.
T2DIR = tads2
T3DIR = tads3
HTDIR = htmltads

INCLUDEPATH += \
    src \
    $$T2DIR \
    $$T3DIR \
    $$HTDIR

win32:INCLUDEPATH += $$T2DIR/msdos
DEPENDPATH += src $$T2DIR $$T3DIR $$HTDIR
OBJECTS_DIR = obj
MOC_DIR = tmp
UI_DIR = tmp

# DISTFILES += AUTHORS COPYING ChangeLog INSTALL NEWS README SOURCE_README TODO
# Extra files to include in the distribution tar.gz archive (by doing a
# "make dist").
# DISTFILES += \
# $$T2DIR/LICENSE.TXT \
# $$T2DIR/tadsver.htm \
# $$T2DIR/portnote.txt \
# $$T3DIR/portnote.htm \
# $$HTDIR/LICENSE.TXT \
# $$HTDIR/notes/porting.htm

FORMS += \
    src/confdialog.ui \
    src/gameinfodialog.ui \
    src/aboutqtadsdialog.ui

# QTads headers
HEADERS += \
    src/osqt.h \
    src/hos_qt.h \
    src/oswin.h \
    src/hos_w32.h \
    src/missing.h \
    src/globals.h \
    src/sysfont.h \
    src/sysframe.h \
    src/syswinaboutbox.h \
    src/syswingroup.h \
    src/syswin.h \
    src/syswininput.h \
    src/sysimagejpeg.h \
    src/sysimagepng.h \
    src/sysimagemng.h \
    src/syssoundogg.h \
    src/syssoundwav.h \
    src/syssoundmpeg.h \
    src/syssoundmidi.h \
    src/qtadsimage.h \
    src/qtadssound.h \
    src/dispwidget.h \
    src/dispwidgetinput.h \
    src/qtadshostifc.h \
    src/qtadstimer.h \
    src/confdialog.h \
    src/settings.h \
    src/gameinfodialog.h \
    src/kcolorbutton.h \
    src/aboutqtadsdialog.h \
    src/config.h \
    src/rwopsbundle.h

# QTads sources.
SOURCES += \
    src/oemqt.c \
    src/osqt.cc \
    src/hos_qt.cc \
    src/globals.cc \
    src/sysframe.cc \
    src/syswingroup.cc \
    src/syswin.cc \
    src/syswinaboutbox.cc \
    src/syswininput.cc \
    src/sysimage.cc \
    src/syssound.cc \
    src/missing.cc \
    src/qtadsimage.cc \
    src/qtadssound.cc \
    src/main.cc \
    src/dispwidget.cc \
    src/dispwidgetinput.cc \
    src/confdialog.cc \
    src/settings.cc \
    src/gameinfodialog.cc \
    src/kcolorbutton.cc \
    src/aboutqtadsdialog.cc \
    src/rwopsbundle.c

unix:SOURCES += \
    $$T2DIR/ostzposix.c

win32:SOURCES += \
    $$T2DIR/msdos/ostzw32.c

# Portable Tads headers.  We simply include every header from the Tads
# directories.  It's sub-optimal, but the safest solution.
# Tads 2 headers.
HEADERS += $$PWD/$$T2DIR/*.h

# Tads 3 headers.
HEADERS += $$PWD/$$T3DIR/*.h

# HTML TADS headers.
HEADERS += $$PWD/$$HTDIR/*.h

# Portable Tads sources.  We always know which are needed.
# HTML Tads sources.
SOURCES += \
    $$HTDIR/tadshtml.cpp \
    $$HTDIR/tadsrtyp.cpp \
    $$HTDIR/htmlprs.cpp \
    $$HTDIR/htmltags.cpp \
    $$HTDIR/htmlfmt.cpp \
    $$HTDIR/htmldisp.cpp \
    $$HTDIR/htmlsys.cpp \
    $$HTDIR/htmltxar.cpp \
    $$HTDIR/htmlinp.cpp \
    $$HTDIR/htmlrc.cpp \
    $$HTDIR/htmlrf.cpp \
    $$HTDIR/htmlhash.cpp \
    $$HTDIR/oshtml.cpp \
    $$HTDIR/htmlsnd.cpp

# Tads2 sources.
SOURCES += \
    $$T2DIR/argize.c \
    $$T2DIR/ler.c \
    $$T2DIR/mcm.c \
    $$T2DIR/mcs.c \
    $$T2DIR/mch.c \
    $$T2DIR/obj.c \
    $$T2DIR/cmd.c \
    $$T2DIR/errmsg.c \
    $$T2DIR/fioxor.c \
    $$T2DIR/oserr.c \
    $$T2DIR/runstat.c \
    $$T2DIR/fio.c \
    $$T2DIR/getstr.c \
    $$T2DIR/cmap.c \
    $$T2DIR/askf_os.c \
    $$T2DIR/indlg_os.c \
    $$T2DIR/osifc.c \
    $$T2DIR/dat.c \
    $$T2DIR/lst.c \
    $$T2DIR/run.c \
    $$T2DIR/out.c \
    $$T2DIR/voc.c \
    $$T2DIR/bif.c \
    $$T2DIR/suprun.c \
    $$T2DIR/regex.c \
    $$T2DIR/vocab.c \
    $$T2DIR/execmd.c \
    $$T2DIR/ply.c \
    $$T2DIR/qas.c \
    $$T2DIR/trd.c \
    $$T2DIR/dbgtr.c \
    $$T2DIR/linfdum.c \
    $$T2DIR/osrestad.c \
    $$T2DIR/bifgdum.c \
    $$T2DIR/output.c \
    $$T2DIR/osstzprs.c \
    $$T2DIR/osnoui.c

# Tads3 sources.
SOURCES += \
    $$T3DIR/derived/vmuni_cs.cpp \
    $$T3DIR/askf_os3.cpp \
    $$T3DIR/charmap.cpp \
    $$T3DIR/gameinfo.cpp \
    $$T3DIR/indlg_os3.cpp \
    $$T3DIR/md5.cpp \
    $$T3DIR/resfind.cpp \
    $$T3DIR/resload.cpp \
    $$T3DIR/resnoexe.cpp \
    $$T3DIR/sha2.cpp \
    $$T3DIR/std.cpp \
    $$T3DIR/tcerr.cpp \
    $$T3DIR/tcerrmsg.cpp \
    $$T3DIR/tcgen.cpp \
    $$T3DIR/tcglob.cpp \
    $$T3DIR/tcmain.cpp \
    $$T3DIR/tcprs.cpp \
    $$T3DIR/tcprs_rt.cpp \
    $$T3DIR/tcprsnf.cpp \
    $$T3DIR/tcprsnl.cpp \
    $$T3DIR/tcprsstm.cpp \
    $$T3DIR/tcsrc.cpp \
    $$T3DIR/tct3.cpp \
    $$T3DIR/tct3_d.cpp \
    $$T3DIR/tct3nl.cpp \
    $$T3DIR/tct3stm.cpp \
    $$T3DIR/tct3unas.cpp \
    $$T3DIR/tctok.cpp \
    $$T3DIR/utf8.cpp \
    $$T3DIR/vmanonfn.cpp \
    $$T3DIR/vmbif.cpp \
    $$T3DIR/vmbifl.cpp \
    $$T3DIR/vmbifregx.cpp \
    $$T3DIR/vmbift3.cpp \
    $$T3DIR/vmbiftad.cpp \
    $$T3DIR/vmbiftio.cpp \
    $$T3DIR/vmbiftix.cpp \
    $$T3DIR/vmbignum.cpp \
    $$T3DIR/vmbignumlib.cpp \
    $$T3DIR/vmbt3_nd.cpp \
    $$T3DIR/vmbytarr.cpp \
    $$T3DIR/vmcfgfl.cpp \
    $$T3DIR/vmcoll.cpp \
    $$T3DIR/vmconhmp.cpp \
    $$T3DIR/vmconhtm.cpp \
    $$T3DIR/vmconsol.cpp \
    $$T3DIR/vmcrc.cpp \
    $$T3DIR/vmcset.cpp \
    $$T3DIR/vmdate.cpp \
    $$T3DIR/vmdict.cpp \
    $$T3DIR/vmdynfunc.cpp \
    $$T3DIR/vmerr.cpp \
    $$T3DIR/vmerrmsg.cpp \
    $$T3DIR/vmfile.cpp \
    $$T3DIR/vmfilnam.cpp \
    $$T3DIR/vmfilobj.cpp \
    $$T3DIR/vmfref.cpp \
    $$T3DIR/vmfunc.cpp \
    $$T3DIR/vmglob.cpp \
    $$T3DIR/vmgram.cpp \
    $$T3DIR/vmhash.cpp \
    $$T3DIR/vmimage.cpp \
    $$T3DIR/vmimg_nd.cpp \
    $$T3DIR/vmini_nd.cpp \
    $$T3DIR/vminit.cpp \
    $$T3DIR/vminitfl.cpp \
    $$T3DIR/vmintcls.cpp \
    $$T3DIR/vmisaac.cpp \
    $$T3DIR/vmiter.cpp \
    $$T3DIR/vmlog.cpp \
    $$T3DIR/vmlookup.cpp \
    $$T3DIR/vmlst.cpp \
    $$T3DIR/vmmain.cpp \
    $$T3DIR/vmmcreg.cpp \
    $$T3DIR/vmmeta.cpp \
    $$T3DIR/vmnetfillcl.cpp \
    $$T3DIR/vmobj.cpp \
    $$T3DIR/vmop.cpp \
    $$T3DIR/vmpack.cpp \
    $$T3DIR/vmpat.cpp \
    $$T3DIR/vmpool.cpp \
    $$T3DIR/vmpoolfl.cpp \
    $$T3DIR/vmregex.cpp \
    $$T3DIR/vmrun.cpp \
    $$T3DIR/vmrunsym.cpp \
    $$T3DIR/vmsa.cpp \
    $$T3DIR/vmsave.cpp \
    $$T3DIR/vmsort.cpp \
    $$T3DIR/vmsortv.cpp \
    $$T3DIR/vmsrcf.cpp \
    $$T3DIR/vmstack.cpp \
    $$T3DIR/vmstr.cpp \
    $$T3DIR/vmstrbuf.cpp \
    $$T3DIR/vmstrcmp.cpp \
    $$T3DIR/vmtmpfil.cpp \
    $$T3DIR/vmtobj.cpp \
    $$T3DIR/vmtype.cpp \
    $$T3DIR/vmtypedh.cpp \
    $$T3DIR/vmtz.cpp \
    $$T3DIR/vmtzobj.cpp \
    $$T3DIR/vmundo.cpp \
    $$T3DIR/vmvec.cpp
