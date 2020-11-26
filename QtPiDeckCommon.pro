TEMPLATE = subdirs

SUBDIRS += \
    QmlTestsCommon \
    QtPiDeckCommon \
    QtPiDeckCommonQmlTests

QtPiDeckCommonQmlTests.depends = QtPiDeckCommon QmlTestsCommon
