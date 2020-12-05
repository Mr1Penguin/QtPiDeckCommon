#include <QtTest>

#include "Ioc.hpp"

namespace QtPiDeck::Tests {
class Ioc : public QObject
{
    // NOLINTNEXTLINE
    Q_OBJECT

private slots:
    void no_service();
    void register_service();
    void register_two_services();
    void replace_service();
    void resolve_by_impl();
    void register_impl();
    void register_specialization();
    void register_specialization_different();
    void resolve_not_same();
    void register_singleton();
    void replace_singleton();
    void register_two_singletons();
    void resolve_same_singleton();
};

struct Interface : QtPiDeck::Services::ServiceInterface {
    ~Interface() override = 0;
    virtual auto getVal() -> int = 0;

protected:
    Interface() = default;
    Interface(const Interface &) = default;
    Interface(Interface &&) = default;

    auto operator=(const Interface&) -> Interface& = default;
    auto operator=(Interface&&) -> Interface& = default;
};

Interface::~Interface() = default;

constexpr int defaultValue = 10;
constexpr int changedValue = 20;

struct Implementation final : Interface {
    Implementation() = default;
    Implementation(const Implementation &) = default;
    Implementation(Implementation &&) = default;

    ~Implementation() final = default;
    auto operator=(const Implementation&) -> Implementation& = default;
    auto operator=(Implementation&&) -> Implementation& = default;
    auto getVal() -> int final { return value; }
    void setVal(int val) { value = val; }
private:
    int value = defaultValue;
};

struct ImplementationB final : Interface {
    ImplementationB() = default;
    ImplementationB(const ImplementationB &) = default;
    ImplementationB(ImplementationB &&) = default;

    ~ImplementationB() final = default;
    auto operator=(const ImplementationB&) -> ImplementationB& = default;
    auto operator=(ImplementationB&&) -> ImplementationB& = default;
    auto getVal() -> int final { return value; }
    void setVal(int val) { value = val; }
private:
    int value = defaultValue;
};

struct Interface2 : QtPiDeck::Services::ServiceInterface {
    ~Interface2() override = 0;

protected:
    Interface2() = default;
    Interface2(const Interface2 &) = default;
    Interface2(Interface2 &&) = default;

    auto operator=(const Interface2&) -> Interface2& = default;
    auto operator=(Interface2&&) -> Interface2& = default;
};

Interface2::~Interface2() = default;

struct Implementation2 final : Interface2 {
    Implementation2() = default;
    Implementation2(const Implementation2 &) = default;
    Implementation2(Implementation2 &&) = default;

    ~Implementation2() final = default;
    auto operator=(const Implementation2&) -> Implementation2& = default;
    auto operator=(Implementation2&&) -> Implementation2& = default;
};

void Ioc::no_service() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(service == nullptr);
}

void Ioc::register_service() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Interface, Implementation>();
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
}

void Ioc::register_two_services() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Interface, Implementation>();
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    ioc.RegisterService<Interface2, Implementation2>();
    auto service2 = ioc.ResolveService<Interface2>();
    QVERIFY(dynamic_cast<Implementation2*>(service2.get()) != nullptr);
}

void Ioc::replace_service() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Interface, Implementation>();
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    ioc.RegisterService<Interface, ImplementationB>();
    auto service2 = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<ImplementationB*>(service2.get()) != nullptr);
}

void Ioc::resolve_by_impl() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Interface, Implementation>();
    auto service = ioc.ResolveService<Implementation>();
    QVERIFY(service != nullptr);
}

void Ioc::register_impl() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Implementation, Implementation>();
    auto service = ioc.ResolveService<Implementation>();
    QVERIFY(service != nullptr);
}

void Ioc::register_specialization() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Interface, Implementation>();
    ioc.RegisterService<Implementation, Implementation>();
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    auto service2 = ioc.ResolveService<Implementation>();
    QVERIFY(service2 != nullptr);
}

void Ioc::register_specialization_different() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Interface, Implementation>();
    ioc.RegisterService<ImplementationB, ImplementationB>();
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    auto service2 = ioc.ResolveService<ImplementationB>();
    QVERIFY(service2 != nullptr);
    auto service3 = ioc.ResolveService<Implementation>();
    QVERIFY(service3 != nullptr);
}

void Ioc::resolve_not_same() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Interface, Implementation>();
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    QCOMPARE(service->getVal(), defaultValue);
    std::dynamic_pointer_cast<Implementation>(service)->setVal(changedValue);
    QCOMPARE(service->getVal(), changedValue);
    auto service2 = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service2.get()) != nullptr);
    QCOMPARE(service2->getVal(), defaultValue);
}

void Ioc::register_singleton() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Interface>(std::make_shared<Implementation>());
    auto service = ioc.ResolveService<Interface>();
    QCOMPARE(service.use_count(), 2);
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
}

void Ioc::replace_singleton() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Interface>(std::make_shared<Implementation>());
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    ioc.RegisterSingleton<Interface>(std::make_shared<ImplementationB>());
    QCOMPARE(service.use_count(), 1);
    auto service2 = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service2.get()) == nullptr);
    QVERIFY(dynamic_cast<ImplementationB*>(service2.get()) != nullptr);
}

void Ioc::register_two_singletons() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Interface>(std::make_shared<Implementation>());
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    ioc.RegisterSingleton<Interface2>(std::make_shared<Implementation2>());
    auto service2 = ioc.ResolveService<Interface2>();
    QVERIFY(dynamic_cast<Implementation2*>(service2.get()) != nullptr);
}

void Ioc::resolve_same_singleton() // NOLINT(readability-convert-member-functions-to-static)
{
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Interface>(std::make_shared<Implementation>());
    auto service = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    QCOMPARE(service->getVal(), defaultValue);
    std::dynamic_pointer_cast<Implementation>(service)->setVal(changedValue);
    QCOMPARE(service->getVal(), changedValue);
    auto service2 = ioc.ResolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service2.get()) != nullptr);
    QCOMPARE(service2->getVal(), changedValue);
}
}

using Ioc = QtPiDeck::Tests::Ioc;
QTEST_APPLESS_MAIN(QtPiDeck::Tests::Ioc) // NOLINT

#include "tst_QtPiDeckCommonIocTests.moc"
