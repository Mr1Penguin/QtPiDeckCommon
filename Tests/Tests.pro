TEMPLATE = subdirs

SUBDIRS += \
    QmlTestsCommon \
    QtPiDeckCommonCommandTests \

QtPiDeckCommonCommandTests.depends += QmlTestsCommon
