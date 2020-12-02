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
}

using Ioc = QtPiDeck::Tests::Ioc;
QTEST_APPLESS_MAIN(QtPiDeck::Tests::Ioc)

#include "tst_QtPiDeckCommonIocTests.moc"
