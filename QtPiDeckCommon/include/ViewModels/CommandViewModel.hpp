#pragma once

#include "QtPiDeckCommon_global.hpp"

#include <QObject>

#include "Utilities/Traits.hpp"

namespace QtPiDeck::ViewModels {
class QTPIDECKCOMMON_EXPORT CommandViewModel : public QObject, Utilities::ViewModelTraits<CommandViewModel> {
  Q_OBJECT // NOLINT

#if QT_VERSION_MAJOR == 6
  Q_PROPERTY(QString text READ text WRITE setText BINDABLE bindableText)
  Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath BINDABLE bindableImagePath)
#else
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
  Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
#endif
public:
  explicit CommandViewModel(QObject* parent = nullptr) noexcept;

  [[nodiscard]] auto text() const noexcept -> QString { return m_text; }
  void setText(const QString& text) noexcept;
  [[nodiscard]] auto imagePath() const noexcept -> QString { return m_imagePath; }
  void setImagePath(const QString& imagePath) noexcept;

#if QT_VERSION_MAJOR == 6
  auto bindableText() -> QBindable<QString> { return &m_text; }
  auto bindableImagePath() -> QBindable<QString> { return &m_imagePath; }
#endif

  static void registerType();

signals:
  void textChanged();
  void imagePathChanged();

private:
#if QT_VERSION_MAJOR == 6
  Q_OBJECT_BINDABLE_PROPERTY(CommandViewModel, QString, m_text, &CommandViewModel::textChanged)
  Q_OBJECT_BINDABLE_PROPERTY(CommandViewModel, QString, m_imagePath, &CommandViewModel::imagePathChanged)
#else
  QString m_text;
  QString m_imagePath;
#endif
};
}
