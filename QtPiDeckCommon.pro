TEMPLATE = subdirs

SUBDIRS += \
    QtPiDeckCommon \
    Tests

Tests.QtPiDeckCommonCommandTests.depends += QmlTestsCommon QtPiDeckCommon
