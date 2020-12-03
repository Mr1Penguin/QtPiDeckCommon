#include <QtTest>

#include "Ioc.hpp"

namespace QtPiDeck::Tests {
class Ioc : public QObject
{
    Q_OBJECT

public:
    Ioc();
    ~Ioc();

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

Ioc::Ioc()
{

}

Ioc::~Ioc()
{

}

void Ioc::no_service()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    QtPiDeck::Ioc ioc;
    auto service = ioc.ResolveService<Int>();
    QVERIFY(service == nullptr);
}

void Ioc::register_service()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Int, Impl>();
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
}

void Ioc::register_two_services()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    struct Int2 : QtPiDeck::Services::ServiceInterface {};
    struct Impl2 : Int2 {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Int, Impl>();
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    ioc.RegisterService<Int2, Impl2>();
    auto service2 = ioc.ResolveService<Int2>();
    QVERIFY(dynamic_cast<Impl2*>(service2.get()) != nullptr);
}

void Ioc::replace_service()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    struct Impl2 : Int {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Int, Impl>();
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    ioc.RegisterService<Int, Impl2>();
    auto service2 = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl2*>(service2.get()) != nullptr);
}

void Ioc::resolve_by_impl()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Int, Impl>();
    auto service = ioc.ResolveService<Impl>();
    QVERIFY(service != nullptr);
}

void Ioc::register_impl()
{
    struct Impl : QtPiDeck::Services::ServiceInterface {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Impl, Impl>();
    auto service = ioc.ResolveService<Impl>();
    QVERIFY(service != nullptr);
}

void Ioc::register_specialization()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Int, Impl>();
    ioc.RegisterService<Impl, Impl>();
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    auto service2 = ioc.ResolveService<Impl>();
    QVERIFY(service2 != nullptr);
}

void Ioc::register_specialization_different()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    struct ImplSpec : Int {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Int, Impl>();
    ioc.RegisterService<ImplSpec, ImplSpec>();
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    auto service2 = ioc.ResolveService<ImplSpec>();
    QVERIFY(service2 != nullptr);
    auto service3 = ioc.ResolveService<Impl>();
    QVERIFY(service3 != nullptr);
}

void Ioc::resolve_not_same()
{
    struct Int : QtPiDeck::Services::ServiceInterface { virtual int getVal() = 0; };
    struct Impl : Int { int getVal() final { return local; } int local = 10;};
    QtPiDeck::Ioc ioc;
    ioc.RegisterService<Int, Impl>();
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    QCOMPARE(service->getVal(), 10);
    std::dynamic_pointer_cast<Impl>(service)->local = 20;
    QCOMPARE(service->getVal(), 20);
    auto service2 = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service2.get()) != nullptr);
    QCOMPARE(service2->getVal(), 10);
}

void Ioc::register_singleton()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Int>(std::make_shared<Impl>());
    auto service = ioc.ResolveService<Int>();
    QCOMPARE(service.use_count(), 2);
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
}

void Ioc::replace_singleton()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    struct Impl2 : Int {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Int>(std::make_shared<Impl>());
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    ioc.RegisterSingleton<Int>(std::make_shared<Impl2>());
    QCOMPARE(service.use_count(), 1);
    auto service2 = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service2.get()) == nullptr);
    QVERIFY(dynamic_cast<Impl2*>(service2.get()) != nullptr);
}

void Ioc::register_two_singletons()
{
    struct Int : QtPiDeck::Services::ServiceInterface {};
    struct Impl : Int {};
    struct Int2 : QtPiDeck::Services::ServiceInterface {};
    struct Impl2 : Int2 {};
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Int>(std::make_shared<Impl>());
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    ioc.RegisterSingleton<Int2>(std::make_shared<Impl2>());
    auto service2 = ioc.ResolveService<Int2>();
    QVERIFY(dynamic_cast<Impl2*>(service2.get()) != nullptr);
}

void Ioc::resolve_same_singleton()
{
    struct Int : QtPiDeck::Services::ServiceInterface { virtual int getVal() = 0; };
    struct Impl : Int { int getVal() final { return local; } int local = 10;};
    QtPiDeck::Ioc ioc;
    ioc.RegisterSingleton<Int>(std::make_shared<Impl>());
    auto service = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service.get()) != nullptr);
    QCOMPARE(service->getVal(), 10);
    std::dynamic_pointer_cast<Impl>(service)->local = 20;
    QCOMPARE(service->getVal(), 20);
    auto service2 = ioc.ResolveService<Int>();
    QVERIFY(dynamic_cast<Impl*>(service2.get()) != nullptr);
    QCOMPARE(service2->getVal(), 20);
}
}

using Ioc = QtPiDeck::Tests::Ioc;
QTEST_APPLESS_MAIN(QtPiDeck::Tests::Ioc)

#include "tst_QtPiDeckCommonIocTests.moc"
