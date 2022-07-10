
#include <ostream>
#include <string>

#include <QString>

#ifndef LOGGER_TOOLS_H
#define LOGGER_TOOLS_H

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define CLIENT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define CLIENT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define CLIENT_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define CLIENT_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define CLIENT_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define CLIENT_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define CLIENT_FUNC_SIG __func__
#else
#define CLIENT_FUNC_SIG "FUNC_SIG unknown!"
#endif

inline std::ostream& operator<<(std::ostream& os, const QString& qstr) {
  os << qstr.toStdString();
  return os;
}

static QString info_file(char const* function, char const* file, long line) {
  auto mess = QString(function);
  mess += " ";
  mess += QString(file);
  mess += ":" + QString::number(line) + "] ";
  return mess;
}

#define INFOFILE info_file(CLIENT_FUNC_SIG, __FILE__, __LINE__)

#endif  // LOGGER_TOOLS_H
