#ifndef CUSTOM_ASSERT_H
#define CUSTOM_ASSERT_H

#if !defined(NDEBUG)
#define m_assert(condition, message)                                     \
  do {                                                                   \
    if (!(condition)) {                                                  \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__   \
                << " line " << __LINE__ << ": " << message << std::endl; \
      std::terminate();                                                  \
    }                                                                    \
  } while (false)

#define c_assert(condition)                                            \
  do {                                                                 \
    if (!(condition)) {                                                \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                << " line " << __LINE__ << std::endl;                  \
      std::terminate();                                                \
    }                                                                  \
  } while (false)
#else
#define m_assert(condition, message) \
  do {                               \
    if (false) {                     \
      (void)(condition);             \
      (void)sizeof(message);         \
    }                                \
  } while (false)
#define c_assert(condition) \
  do {                      \
    if (false) {            \
      (void)(condition);    \
    }                       \
  } while (false)
#endif

#endif  // CUSTOM_ASSERT_H
