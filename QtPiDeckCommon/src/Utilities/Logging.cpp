#include "Utilities/Logging.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/message.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <QCoreApplication>
#include <QFileInfo>

namespace QtPiDeck::Utilities {
namespace {
struct Formatter {
  Formatter()                 = default;
  Formatter(const Formatter&) = default;
  Formatter(Formatter&&)      = delete;
  auto operator=(const Formatter&) -> Formatter& = delete;
  auto operator=(Formatter&&) -> Formatter& = delete;
  ~Formatter()                              = default;

  void operator()(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) {
    strm << "[";
    m_dateFormatter(rec, strm);
    strm << "] " << boost::log::extract<std::string>(categoryAttribute, rec) << ": ";

    strm << "<" << rec[boost::log::trivial::severity] << "> ";

    strm << rec[boost::log::expressions::smessage];
  }

private:
  using actorType =
      decltype(std::declval<boost::log::expressions::stream_type>()
               << std::declval<boost::log::expressions::format_date_time_actor<boost::posix_time::ptime,
                                                                               boost::log::fallback_to_none, char>>());
  static auto getDateFormatter() -> actorType {
    return boost::log::expressions::stream
           << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%f");
  }

  actorType m_dateFormatter{getDateFormatter()};
};
}

void initLogging(std::string_view execName) {
  const Formatter formatter;
#if !defined(NDEBUG)
  boost::log::add_console_log(std::clog)->set_formatter(formatter);
#endif
  const auto logFilename  = std::string{execName} + "_%N.log";
  constexpr auto fileSize = 10 * 1024 * 1024;
  boost::log::add_file_log(boost::log::keywords::file_name     = logFilename,
                           boost::log::keywords::rotation_size = fileSize)
      ->set_formatter(formatter);
}
}
