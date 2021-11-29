#include "ViewModels/CommandViewModel.hpp"

#include "Application.hpp"

namespace QtPiDeck::ViewModels {
CommandViewModel::CommandViewModel(QObject* parent) noexcept : QObject(parent) {}

void CommandViewModel::setText(const QString& text) noexcept {
#if QT_VERSION_MAJOR == 5
  if (text == m_text) {
    return;
  }
#endif
  m_text = text;
#if QT_VERSION_MAJOR == 5
  emit textChanged();
#endif
}

void CommandViewModel::setImagePath(const QString& imagePath) noexcept {
#if QT_VERSION_MAJOR == 5
  if (imagePath == m_imagePath) {
    return;
  }
#endif

  m_imagePath = imagePath;
#if QT_VERSION_MAJOR == 5
  emit imagePathChanged();
#endif
}

void CommandViewModel::registerType() { qmlRegisterType<CommandViewModel>("QtPiDeck", 1, 0, "CommandViewModel"); }
}

namespace QQmlPrivate {
using namespace QtPiDeck;
template<>
class QQmlElement<ViewModels::CommandViewModel> final : public ViewModels::CommandViewModel // NOLINT
{
public:
  using ViewModels::CommandViewModel::CommandViewModel;
  ~QQmlElement() override { QQmlPrivate::qdeclarativeelement_destructor(this); }
  static void operator delete(void* ptr) { // NOLINT
    ::operator delete(ptr);
  }
  /* static void operator delete(void*, void*) { // NOLINT
  }*/
};

template<>
#if QT_VERSION_MAJOR == 6
void createInto<ViewModels::CommandViewModel>(void* memory, void*) { // NOLINT
#else
void createInto<ViewModels::CommandViewModel>(void* memory) {
#endif
  const auto& ioc          = Application::ccurrent()->ioc();
  using viewModelType      = QQmlElement<ViewModels::CommandViewModel>;
  [[maybe_unused]] auto* _ = ioc.make<viewModelType, Services::CreationType::RawInMemory>(memory);
}
}
