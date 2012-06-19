######################################################################
# Communi
######################################################################

UCHARDET_DIR = 3rdparty/uchardet-0.0.1

INCLUDEPATH += $$UCHARDET_DIR/src
DEPENDPATH += $$UCHARDET_DIR/src

HEADERS += $$UCHARDET_DIR/src/uchardet.h

SOURCES += $$UCHARDET_DIR/src/CharDistribution.cpp
SOURCES += $$UCHARDET_DIR/src/JpCntx.cpp
SOURCES += $$UCHARDET_DIR/src/LangBulgarianModel.cpp
SOURCES += $$UCHARDET_DIR/src/LangCyrillicModel.cpp
SOURCES += $$UCHARDET_DIR/src/LangGreekModel.cpp
SOURCES += $$UCHARDET_DIR/src/LangHungarianModel.cpp
SOURCES += $$UCHARDET_DIR/src/LangHebrewModel.cpp
SOURCES += $$UCHARDET_DIR/src/LangThaiModel.cpp
SOURCES += $$UCHARDET_DIR/src/nsHebrewProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsCharSetProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsBig5Prober.cpp
SOURCES += $$UCHARDET_DIR/src/nsEUCJPProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsEUCKRProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsEUCTWProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsEscCharsetProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsEscSM.cpp
SOURCES += $$UCHARDET_DIR/src/nsGB2312Prober.cpp
SOURCES += $$UCHARDET_DIR/src/nsMBCSGroupProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsMBCSSM.cpp
SOURCES += $$UCHARDET_DIR/src/nsSBCSGroupProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsSBCharSetProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsSJISProber.cpp
SOURCES += $$UCHARDET_DIR/src/nsUTF8Prober.cpp
SOURCES += $$UCHARDET_DIR/src/nsLatin1Prober.cpp
SOURCES += $$UCHARDET_DIR/src/nsUniversalDetector.cpp
SOURCES += $$UCHARDET_DIR/src/uchardet.cpp
