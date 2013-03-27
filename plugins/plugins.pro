include(../proconfig.pri)

message($$BASE_PATH)

TEMPLATE = subdirs


SUBDIRS = debugrenderer.pro \
          qosdrenderer.pro \
          fakeplayer.pro \
          #qgstaudio.pro
