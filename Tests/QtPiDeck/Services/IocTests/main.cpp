#include <QtTest>

#include "Services/Ioc.hpp"

namespace QtPiDeck::Services::Tests {
class IocTests : public QObject
{
    // NOLINTNEXTLINE
    Q_OBJECT

private slots:
    void init();
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

    void resolve_with_missing_dependency();
    void resolve_with_dependency();
    void resolve_with_two_dependencies();
    void resolve_with_multilevel_dependencies();

    void make_unique_pointer_without_dependencies();
    void make_shared_pointer_without_dependencies();
    void make_copy_without_dependencies();
    void make_raw_in_memory_without_dependencies();
    void make_raw_without_dependencies();

    void make_unique_pointer_with_one_dependency_constructor();
    void make_shared_pointer_with_one_dependency_constructor();
    void make_copy_with_one_dependency_constructor();
    void make_raw_in_memory_with_one_dependency_constructor();
    void make_raw_with_one_dependency_constructor();

    void make_unique_pointer_with_one_dependency_constructor_qobject();
    void make_shared_pointer_with_one_dependency_constructor_qobject();
    void make_copy_with_one_dependency_constructor_qobject();
    void make_raw_in_memory_with_one_dependency_constructor_qobject();
    void make_raw_with_one_dependency_constructor_qobject();

    void make_unique_pointer_with_one_dependency_no_constructor();
    void make_shared_pointer_with_one_dependency_no_constructor();
    void make_copy_with_one_dependency_no_constructor();
    void make_raw_in_memory_with_one_dependency_no_constructor();
    void make_raw_with_one_dependency_no_constructor();

    void make_unique_pointer_with_two_dependencies_constructor();
    void make_shared_pointer_with_two_dependencies_constructor();
    void make_copy_with_two_dependencies_constructor();
    void make_raw_in_memory_with_two_dependencies_constructor();
    void make_raw_with_two_dependencies_constructor();

    void make_unique_pointer_with_nested_dependencies_constructor();
    void make_shared_pointer_with_nested_dependencies_constructor();
    void make_copy_with_nested_dependencies_constructor();
    void make_raw_in_memory_with_nested_dependencies_constructor();
    void make_raw_with_nested_dependencies_constructor();

private: // NOLINT(readability-redundant-access-specifiers)
    std::unique_ptr<Ioc> m_ioc;
};

void IocTests::init() {
    m_ioc = std::make_unique<Ioc>();
}

struct Interface : ServiceInterface {
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

struct Interface2 : ServiceInterface {
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

void IocTests::no_service() // NOLINT(readability-convert-member-functions-to-static)
{
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(service == nullptr);
}

void IocTests::register_service() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, Implementation>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
}

void IocTests::register_two_services() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, Implementation>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    m_ioc->registerService<Interface2, Implementation2>();
    auto service2 = m_ioc->resolveService<Interface2>();
    QVERIFY(dynamic_cast<Implementation2*>(service2.get()) != nullptr);
}

void IocTests::replace_service() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, Implementation>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    m_ioc->registerService<Interface, ImplementationB>();
    auto service2 = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<ImplementationB*>(service2.get()) != nullptr);
}

void IocTests::resolve_by_impl() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, Implementation>();
    auto service = m_ioc->resolveService<Implementation>();
    QVERIFY(service != nullptr);
}

void IocTests::register_impl() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Implementation, Implementation>();
    auto service = m_ioc->resolveService<Implementation>();
    QVERIFY(service != nullptr);
}

void IocTests::register_specialization() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Implementation, Implementation>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    auto service2 = m_ioc->resolveService<Implementation>();
    QVERIFY(service2 != nullptr);
}

void IocTests::register_specialization_different() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<ImplementationB, ImplementationB>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    auto service2 = m_ioc->resolveService<ImplementationB>();
    QVERIFY(service2 != nullptr);
    auto service3 = m_ioc->resolveService<Implementation>();
    QVERIFY(service3 != nullptr);
}

void IocTests::resolve_not_same() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, Implementation>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    QCOMPARE(service->getVal(), defaultValue);
    std::dynamic_pointer_cast<Implementation>(service)->setVal(changedValue);
    QCOMPARE(service->getVal(), changedValue);
    auto service2 = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service2.get()) != nullptr);
    QCOMPARE(service2->getVal(), defaultValue);
}

void IocTests::register_singleton() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerSingleton<Interface>(std::make_shared<Implementation>());
    auto service = m_ioc->resolveService<Interface>();
    QCOMPARE(service.use_count(), 2);
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
}

void IocTests::replace_singleton() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerSingleton<Interface>(std::make_shared<Implementation>());
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    m_ioc->registerSingleton<Interface>(std::make_shared<ImplementationB>());
    QCOMPARE(service.use_count(), 1);
    auto service2 = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service2.get()) == nullptr);
    QVERIFY(dynamic_cast<ImplementationB*>(service2.get()) != nullptr);
}

void IocTests::register_two_singletons() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerSingleton<Interface>(std::make_shared<Implementation>());
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    m_ioc->registerSingleton<Interface2>(std::make_shared<Implementation2>());
    auto service2 = m_ioc->resolveService<Interface2>();
    QVERIFY(dynamic_cast<Implementation2*>(service2.get()) != nullptr);
}

void IocTests::resolve_same_singleton() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerSingleton<Interface>(std::make_shared<Implementation>());
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service.get()) != nullptr);
    QCOMPARE(service->getVal(), defaultValue);
    std::dynamic_pointer_cast<Implementation>(service)->setVal(changedValue);
    QCOMPARE(service->getVal(), changedValue);
    auto service2 = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<Implementation*>(service2.get()) != nullptr);
    QCOMPARE(service2->getVal(), changedValue);
}

struct ImplementationWithDeps final : Interface, UseServices<Interface2> {
    ImplementationWithDeps() = default;
    ImplementationWithDeps(const ImplementationWithDeps &) = default;
    ImplementationWithDeps(ImplementationWithDeps &&) = default;

    ~ImplementationWithDeps() final = default;
    auto operator=(const ImplementationWithDeps&) -> ImplementationWithDeps& = default;
    auto operator=(ImplementationWithDeps&&) -> ImplementationWithDeps& = default;
    auto getVal() -> int final { return value; }
    void setVal(int val) { value = val; }

    auto ResolvedService() -> std::shared_ptr<Interface2>& {
        return service<Interface2>();
    }

private:
    int value = defaultValue;
};

void IocTests::resolve_with_missing_dependency() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, ImplementationWithDeps>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<ImplementationWithDeps*>(service.get()) != nullptr);
    QVERIFY(std::dynamic_pointer_cast<ImplementationWithDeps>(service)->ResolvedService() == nullptr);
}

void IocTests::resolve_with_dependency() // NOLINT(readability-convert-member-functions-to-static)
{
    m_ioc->registerService<Interface, ImplementationWithDeps>();
    m_ioc->registerService<Interface2, Implementation2>();
    auto service = m_ioc->resolveService<Interface>();
    QVERIFY(dynamic_cast<ImplementationWithDeps*>(service.get()) != nullptr);
    auto resolverService = std::dynamic_pointer_cast<ImplementationWithDeps>(service)->ResolvedService();
    QVERIFY(resolverService != nullptr);
    QVERIFY(dynamic_cast<Implementation2*>(resolverService.get()) != nullptr);
}

struct Interface3 : ServiceInterface {
    Interface3() = default;
    Interface3(const Interface3 &) = default;
    Interface3(Interface3 &&) = default;
    ~Interface3() override = 0;

    auto operator=(const Interface3&) -> Interface3& = default;
    auto operator=(Interface3&&) -> Interface3& = default;
};

Interface3::~Interface3() = default;

struct ImplementationWithTwoDeps final : Interface3, UseServices<Interface, Interface2> {
    ImplementationWithTwoDeps() = default;
    ImplementationWithTwoDeps(const ImplementationWithTwoDeps &) = default;
    ImplementationWithTwoDeps(ImplementationWithTwoDeps &&) = default;

    ~ImplementationWithTwoDeps() final = default;
    auto operator=(const ImplementationWithTwoDeps&) -> ImplementationWithTwoDeps& = default;
    auto operator=(ImplementationWithTwoDeps&&) -> ImplementationWithTwoDeps& = default;

    template<class TService>
    auto ResolvedService() -> std::shared_ptr<TService>& {
        return service<TService>();
    }
};

void IocTests::resolve_with_two_dependencies() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    m_ioc->registerService<Interface3, ImplementationWithTwoDeps>();
    auto service = m_ioc->resolveService<Interface3>();
    QVERIFY(dynamic_cast<ImplementationWithTwoDeps*>(service.get()) != nullptr);
    auto resolverService1 = std::dynamic_pointer_cast<ImplementationWithTwoDeps>(service)->ResolvedService<Interface>();
    QVERIFY(resolverService1 != nullptr);
    QVERIFY(dynamic_cast<Implementation*>(resolverService1.get()) != nullptr);
    auto resolverService2 = std::dynamic_pointer_cast<ImplementationWithTwoDeps>(service)->ResolvedService<Interface2>();
    QVERIFY(resolverService2 != nullptr);
    QVERIFY(dynamic_cast<Implementation2*>(resolverService2.get()) != nullptr);
}

struct ImplementationWithNestedDeps final : Interface3, UseServices<Interface> {
    ImplementationWithNestedDeps() = default;
    ImplementationWithNestedDeps(const ImplementationWithNestedDeps &) = default;
    ImplementationWithNestedDeps(ImplementationWithNestedDeps &&) = default;

    ~ImplementationWithNestedDeps() final = default;
    auto operator=(const ImplementationWithNestedDeps&) -> ImplementationWithNestedDeps& = default;
    auto operator=(ImplementationWithNestedDeps&&) -> ImplementationWithNestedDeps& = default;

    auto ResolvedService() -> std::shared_ptr<Interface>& {
        return service<Interface>();
    }
};

void IocTests::resolve_with_multilevel_dependencies() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, ImplementationWithDeps>();
    m_ioc->registerService<Interface2, Implementation2>();
    m_ioc->registerService<Interface3, ImplementationWithNestedDeps>();
    auto service = m_ioc->resolveService<Interface3>();
    QVERIFY(dynamic_cast<ImplementationWithNestedDeps*>(service.get()) != nullptr);
    auto resolverService1 = std::dynamic_pointer_cast<ImplementationWithNestedDeps>(service)->ResolvedService();
    QVERIFY(resolverService1 != nullptr);
    QVERIFY(dynamic_cast<ImplementationWithDeps*>(resolverService1.get()) != nullptr);
    auto resolverService2 = std::dynamic_pointer_cast<ImplementationWithDeps>(resolverService1)->ResolvedService();
    QVERIFY(resolverService2 != nullptr);
    QVERIFY(dynamic_cast<Implementation2*>(resolverService2.get()) != nullptr);
}

class ClassWithoutDependencies {};

void IocTests::make_unique_pointer_without_dependencies() { // NOLINT(readability-convert-member-functions-to-static)
    auto result = m_ioc->make<ClassWithoutDependencies, CreationType::UniquePointer>();
    static_assert(std::is_same_v<std::unique_ptr<ClassWithoutDependencies>, decltype(result)>);
    QVERIFY(result != nullptr);
}

void IocTests::make_shared_pointer_without_dependencies() { // NOLINT(readability-convert-member-functions-to-static)
    auto result = m_ioc->make<ClassWithoutDependencies, CreationType::SharedPointer>();
    static_assert(std::is_same_v<std::shared_ptr<ClassWithoutDependencies>, decltype(result)>);
    QVERIFY(result != nullptr);
}

void IocTests::make_copy_without_dependencies() { // NOLINT(readability-convert-member-functions-to-static)
    [[maybe_unused]] auto result = m_ioc->make<ClassWithoutDependencies, CreationType::Copy>();
    static_assert(std::is_same_v<ClassWithoutDependencies, decltype(result)>);
}

void IocTests::make_raw_in_memory_without_dependencies() { // NOLINT(readability-convert-member-functions-to-static)
    alignas(ClassWithoutDependencies) std::array<std::byte, sizeof(ClassWithoutDependencies)> buffer{std::byte{}};
    auto* result = m_ioc->make<ClassWithoutDependencies, CreationType::RawInMemory>(buffer.data());
    static_assert(std::is_same_v<ClassWithoutDependencies*, decltype(result)>);
    QVERIFY(result != nullptr);
    result->~ClassWithoutDependencies();
}

void IocTests::make_raw_without_dependencies() { // NOLINT(readability-convert-member-functions-to-static)
    auto* result = m_ioc->make<ClassWithoutDependencies, CreationType::Raw>();
    static_assert(std::is_same_v<ClassWithoutDependencies*, decltype(result)>);
    auto wrapper = std::unique_ptr<ClassWithoutDependencies>(result);
    QVERIFY(result != nullptr);
}

class ClassWithOneDependencyConstructor : public Services::UseServices<Interface> {
public:
    ClassWithOneDependencyConstructor(std::shared_ptr<Interface> interface) {
        setService(std::move(interface));
    }

    auto getInterface() -> std::shared_ptr<Interface> {
        return service<Interface>();
    }
};

void IocTests::make_unique_pointer_with_one_dependency_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyConstructor, CreationType::UniquePointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

void IocTests::make_shared_pointer_with_one_dependency_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyConstructor, CreationType::SharedPointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

void IocTests::make_copy_with_one_dependency_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyConstructor, CreationType::Copy>();
    QVERIFY(result.getInterface() != nullptr);
    QCOMPARE(result.getInterface()->getVal(), defaultValue);
}

void IocTests::make_raw_in_memory_with_one_dependency_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    using classType = ClassWithOneDependencyConstructor;
    alignas(classType) std::array<std::byte, sizeof(classType)> buffer {std::byte{}};
    auto* result = m_ioc->make<classType, CreationType::RawInMemory>(buffer.data());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
    result->~ClassWithOneDependencyConstructor();
}

void IocTests::make_raw_with_one_dependency_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    using classType = ClassWithOneDependencyConstructor;
    auto result = std::unique_ptr<classType>(m_ioc->make<classType, CreationType::Raw>());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

class ClassWithOneDependencyConstructorQobject : public Services::UseServices<Interface> {
public:
    ClassWithOneDependencyConstructorQobject(QObject* /*parent*/, std::shared_ptr<Interface> interface) {
        setService(std::move(interface));
    }

    auto getInterface() -> std::shared_ptr<Interface> {
        return service<Interface>();
    }
};

void IocTests::make_unique_pointer_with_one_dependency_constructor_qobject() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyConstructorQobject, CreationType::UniquePointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

void IocTests::make_shared_pointer_with_one_dependency_constructor_qobject() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyConstructorQobject, CreationType::SharedPointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

void IocTests::make_copy_with_one_dependency_constructor_qobject() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyConstructorQobject, CreationType::Copy>();
    QVERIFY(result.getInterface() != nullptr);
    QCOMPARE(result.getInterface()->getVal(), defaultValue);
}

void IocTests::make_raw_in_memory_with_one_dependency_constructor_qobject() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    using classType = ClassWithOneDependencyConstructorQobject;
    alignas(classType) std::array<std::byte, sizeof(classType)> buffer{std::byte{}};
    auto* result = m_ioc->make<ClassWithOneDependencyConstructorQobject, CreationType::RawInMemory>(buffer.data());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
    result->~ClassWithOneDependencyConstructorQobject();
}

void IocTests::make_raw_with_one_dependency_constructor_qobject() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    using classType = ClassWithOneDependencyConstructorQobject;
    auto result = std::unique_ptr<classType>(m_ioc->make<classType, CreationType::Raw>());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

class ClassWithOneDependencyNoConstructor : public Services::UseServices<Interface> { // NOLINT(readability-convert-member-functions-to-static)
public:
    ClassWithOneDependencyNoConstructor(std::shared_ptr<Interface> interface) {
        ServiceUser<Interface>::setService(std::move(interface));
    }

    auto getInterface() -> std::shared_ptr<Interface> {
        return service<Interface>();
    }
};

void IocTests::make_unique_pointer_with_one_dependency_no_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyNoConstructor, CreationType::UniquePointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

void IocTests::make_shared_pointer_with_one_dependency_no_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyNoConstructor, CreationType::SharedPointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

void IocTests::make_copy_with_one_dependency_no_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    auto result = m_ioc->make<ClassWithOneDependencyNoConstructor, CreationType::Copy>();
    QVERIFY(result.getInterface() != nullptr);
    QCOMPARE(result.getInterface()->getVal(), defaultValue);
}

void IocTests::make_raw_in_memory_with_one_dependency_no_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    using classType = ClassWithOneDependencyNoConstructor;
    alignas(classType) std::array<std::byte, sizeof(classType)> buffer{std::byte{}};
    auto* result = m_ioc->make<classType, CreationType::RawInMemory>(buffer.data());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
    result->~ClassWithOneDependencyNoConstructor();
}

void IocTests::make_raw_with_one_dependency_no_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    using classType = ClassWithOneDependencyNoConstructor;
    auto result = std::unique_ptr<classType>(m_ioc->make<classType, CreationType::Raw>());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface() != nullptr);
    QCOMPARE(result->getInterface()->getVal(), defaultValue);
}

class ClassWithTwoDependenciesConstructor : public Services::UseServices<Interface, Interface2> { // NOLINT(readability-convert-member-functions-to-static)
public:
    ClassWithTwoDependenciesConstructor(std::shared_ptr<Interface> interface, std::shared_ptr<Interface2> interface2) {
        setService(std::move(interface));
        setService(std::move(interface2));
    }

    template<class TInterface>
    auto getInterface() -> std::shared_ptr<TInterface> {
        return service<TInterface>();
    }
};

void IocTests::make_unique_pointer_with_two_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    auto result = m_ioc->make<ClassWithTwoDependenciesConstructor, CreationType::UniquePointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface>() != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QCOMPARE(result->getInterface<Interface>()->getVal(), defaultValue);
}

void IocTests::make_shared_pointer_with_two_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    auto result = m_ioc->make<ClassWithTwoDependenciesConstructor, CreationType::SharedPointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface>() != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QCOMPARE(result->getInterface<Interface>()->getVal(), defaultValue);
}

void IocTests::make_copy_with_two_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    auto result = m_ioc->make<ClassWithTwoDependenciesConstructor, CreationType::Copy>();
    QVERIFY(result.getInterface<Interface>() != nullptr);
    QVERIFY(result.getInterface<Interface2>() != nullptr);
    QCOMPARE(result.getInterface<Interface>()->getVal(), defaultValue);
}

void IocTests::make_raw_in_memory_with_two_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    using classType = ClassWithTwoDependenciesConstructor;
    alignas(classType) std::array<std::byte, sizeof(classType)> buffer{std::byte{}};
    auto* result = m_ioc->make<classType, CreationType::RawInMemory>(buffer.data());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface>() != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QCOMPARE(result->getInterface<Interface>()->getVal(), defaultValue);
    result->~ClassWithTwoDependenciesConstructor();
}

void IocTests::make_raw_with_two_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    using classType = ClassWithTwoDependenciesConstructor;
    auto result = std::unique_ptr<classType>(m_ioc->make<classType, CreationType::Raw>());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface>() != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QCOMPARE(result->getInterface<Interface>()->getVal(), defaultValue);
}

class ClassWithNestedDependenciesConstructor : public Services::UseServices<Interface3, Interface2> {
public:
    ClassWithNestedDependenciesConstructor(std::shared_ptr<Interface3> interface3,
                                           std::shared_ptr<Interface2> interface2) {
        setService(std::move(interface3));
        setService(std::move(interface2));
    }

    template<class TInterface>
    auto getInterface() -> std::shared_ptr<TInterface> {
        return service<TInterface>();
    }
};

struct ImplementationWithTwoDepsContructor final : Interface3, UseServices<Interface, Interface2> {
    ImplementationWithTwoDepsContructor(std::shared_ptr<Interface> interface, std::shared_ptr<Interface2> interface2) {
        setService(std::move(interface));
        setService(std::move(interface2));
    }
    ImplementationWithTwoDepsContructor(const ImplementationWithTwoDepsContructor &) = default;
    ImplementationWithTwoDepsContructor(ImplementationWithTwoDepsContructor &&) = default;

    ~ImplementationWithTwoDepsContructor() final = default;
    auto operator=(const ImplementationWithTwoDepsContructor&) -> ImplementationWithTwoDepsContructor& = default;
    auto operator=(ImplementationWithTwoDepsContructor&&) -> ImplementationWithTwoDepsContructor& = default;

    template<class TService>
    auto ResolvedService() -> std::shared_ptr<TService>& {
        return service<TService>();
    }
};

void IocTests::make_unique_pointer_with_nested_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    m_ioc->registerService<Interface3, ImplementationWithTwoDepsContructor>();
    auto result = m_ioc->make<ClassWithNestedDependenciesConstructor, CreationType::UniquePointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QVERIFY(result->getInterface<Interface3>() != nullptr);
    auto castedInterface = std::dynamic_pointer_cast<ImplementationWithTwoDepsContructor>(result->getInterface<Interface3>());
    QVERIFY(castedInterface != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface>() != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface2>() != nullptr);
}

void IocTests::make_shared_pointer_with_nested_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    m_ioc->registerService<Interface3, ImplementationWithTwoDepsContructor>();
    auto result = m_ioc->make<ClassWithNestedDependenciesConstructor, CreationType::SharedPointer>();
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QVERIFY(result->getInterface<Interface3>() != nullptr);
    auto castedInterface = std::dynamic_pointer_cast<ImplementationWithTwoDepsContructor>(result->getInterface<Interface3>());
    QVERIFY(castedInterface != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface>() != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface2>() != nullptr);
}

void IocTests::make_copy_with_nested_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    m_ioc->registerService<Interface3, ImplementationWithTwoDepsContructor>();
    auto result = m_ioc->make<ClassWithNestedDependenciesConstructor, CreationType::Copy>();
    QVERIFY(result.getInterface<Interface2>() != nullptr);
    QVERIFY(result.getInterface<Interface3>() != nullptr);
    auto castedInterface = std::dynamic_pointer_cast<ImplementationWithTwoDepsContructor>(result.getInterface<Interface3>());
    QVERIFY(castedInterface != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface>() != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface2>() != nullptr);
}

void IocTests::make_raw_in_memory_with_nested_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    m_ioc->registerService<Interface3, ImplementationWithTwoDepsContructor>();
    using classType = ClassWithNestedDependenciesConstructor;
    alignas(classType) std::array<std::byte, sizeof(classType)> buffer{std::byte{}};
    auto* result = m_ioc->make<classType, CreationType::RawInMemory>(buffer.data());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QVERIFY(result->getInterface<Interface3>() != nullptr);
    auto castedInterface = std::dynamic_pointer_cast<ImplementationWithTwoDepsContructor>(result->getInterface<Interface3>());
    QVERIFY(castedInterface != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface>() != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface2>() != nullptr);
    result->~ClassWithNestedDependenciesConstructor();
}

void IocTests::make_raw_with_nested_dependencies_constructor() { // NOLINT(readability-convert-member-functions-to-static)
    m_ioc->registerService<Interface, Implementation>();
    m_ioc->registerService<Interface2, Implementation2>();
    m_ioc->registerService<Interface3, ImplementationWithTwoDepsContructor>();
    using classType = ClassWithNestedDependenciesConstructor;
    auto result = std::unique_ptr<classType>(m_ioc->make<classType, CreationType::Raw>());
    QVERIFY(result != nullptr);
    QVERIFY(result->getInterface<Interface2>() != nullptr);
    QVERIFY(result->getInterface<Interface3>() != nullptr);
    auto castedInterface = std::dynamic_pointer_cast<ImplementationWithTwoDepsContructor>(result->getInterface<Interface3>());
    QVERIFY(castedInterface != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface>() != nullptr);
    QVERIFY(castedInterface->ResolvedService<Interface2>() != nullptr);
}
}

QTEST_APPLESS_MAIN(QtPiDeck::Services::Tests::IocTests) // NOLINT

#include "main.moc"
