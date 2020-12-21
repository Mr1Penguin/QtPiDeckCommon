#pragma once

#include <vector>
#include <algorithm>

#include <QtCore/qglobal.h>

//TODO: same lines as in QtPiDeckCommon
//      create a separate header-only lib?
#ifndef QTPI4_VERSION
#define QTPI4_VERSION QT_VERSION_CHECK(5, 11, 3)
#endif
//TODO: need to be renamed
#ifndef QT_MIN_TESTED_VERSION
#define QT_MIN_TESTED_VERSION QT_VERSION_CHECK(5, 15, 1)
#endif

#if QT_VERSION > QTPI4_VERSION && QT_VERSION < QT_MIN_TESTED_VERSION
#error "New rasberrypi 4 qt version detected(?)"
#endif

#if QT_VERSION == QTPI4_VERSION

#define QTPIDECK_QUICK_TEST_MAIN_WITH_SETUP(name, setupType, imports, registration) \
    int main(int argc, char **argv) \
    { \
        QTEST_ADD_GPU_BLACKLIST_SUPPORT \
        QTEST_SET_MAIN_SOURCE_PATH \
        registration(); \
        auto additionalImports = imports(); \
        std::vector<char*> newArgv; \
        newArgv.reserve(argc + std::size(additionalImports)); \
        std::copy(argv, argv + argc, std::back_inserter(newArgv)); \
        std::string import = "-import"; \
        for (auto & str : additionalImports) { \
            newArgv.push_back(import.data()); \
            newArgv.push_back(str.data()); \
        } \
        setupType setup; \
        return quick_test_main_with_setup(newArgv.size(), newArgv.data(), #name, QUICK_TEST_SOURCE_DIR, &setup); \
    }
#endif

#ifdef Q_MOC_RUN
#define MAYBE_UNUSED
#else
#define MAYBE_UNUSED [[maybe_unused]]
#endif

#include <QQmlEngine>

template<class T>
void addImports15(MAYBE_UNUSED QQmlEngine *engine, MAYBE_UNUSED T makeImports) noexcept {
    if constexpr(QT_VERSION >= QT_MIN_TESTED_VERSION) {
        auto imports = makeImports();
        for (auto & str : imports) {
            engine->addImportPath(str.data());
        }
    }
}

template<class T>
void callRegisterTypes15(MAYBE_UNUSED T registerTypes) noexcept {
    if constexpr(QT_VERSION >= QT_MIN_TESTED_VERSION) {
        registerTypes();
    }
}
