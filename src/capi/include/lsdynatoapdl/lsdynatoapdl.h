/**
 * @file lsdynatoapdl.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Stable C API of LsDynaToAPDL.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 * @details The C API exists so the converter can be driven from C, Python
 * (ctypes/cffi), Rust or any other language with a C FFI, without exposing the
 * C++ ABI. It follows the usual rules for a stable boundary:
 *
 *   - only C types cross it; every C++ object is an opaque handle;
 *   - no exception ever escapes — failures are reported as
 *     @ref lsdynatoapdl_status codes;
 *   - the caller owns nothing it did not create, and frees every handle it
 *     did create with the matching `_destroy` function;
 *   - returned strings are static and must not be freed.
 *
 * @code
 * lsdynatoapdl_model* model = lsdynatoapdl_model_create();
 * lsdynatoapdl_model_add_node(model, 1, 0.0, 0.0, 0.0);
 * const uint64_t nodes[4] = {1, 2, 3, 4};
 * lsdynatoapdl_model_add_shell181(model, 10, nodes, 2.5);
 * lsdynatoapdl_model_write(model, "out.inp");
 * lsdynatoapdl_model_destroy(model);
 * @endcode
 */

#ifndef LSDYNA_TO_APDL_C_API_H
#define LSDYNA_TO_APDL_C_API_H

#include <stddef.h>
#include <stdint.h>

#include "lsdynatoapdl/export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Number of nodes of a SHELL181 element.
 */
#define LSDYNATOAPDL_SHELL181_NODES 4

/*
 * A C header cannot use enum class, `using`, or a sized enum base, and the
 * suppressions have to live here rather than in a .clang-tidy: clang-tidy
 * applies the configuration of the translation unit being compiled, and this
 * header is included from C++ tests.
 */
/* NOLINTBEGIN(cppcoreguidelines-use-enum-class,modernize-use-using,performance-enum-size)
 */

/**
 * @brief Outcome of an API call.
 */
typedef enum lsdynatoapdl_status {
  LSDYNATOAPDL_OK = 0,                     /**< the call succeeded. */
  LSDYNATOAPDL_ERROR_INVALID_ARGUMENT = 1, /**< a null or invalid argument. */
  LSDYNATOAPDL_ERROR_IO = 2,     /**< a file could not be read or written. */
  LSDYNATOAPDL_ERROR_PARSE = 3,  /**< the input deck could not be read. */
  LSDYNATOAPDL_ERROR_EMPTY = 4,  /**< the deck produced no entities. */
  LSDYNATOAPDL_ERROR_UNKNOWN = 5 /**< an unexpected failure. */
} lsdynatoapdl_status;

/**
 * @brief An APDL model under construction.
 */
typedef struct lsdynatoapdl_model lsdynatoapdl_model;
/* NOLINTEND(cppcoreguidelines-use-enum-class,modernize-use-using,performance-enum-size)
 */

/**
 * @brief Library version, as "major.minor.patch".
 *
 * @return a static, null-terminated string; do not free it.
 */
LSDYNATOAPDL_API const char* lsdynatoapdl_version(void);

/**
 * @brief Human readable description of @p status.
 *
 * @param status the code to describe.
 * @return a static, null-terminated string; do not free it.
 */
LSDYNATOAPDL_API const char* lsdynatoapdl_status_message(
    lsdynatoapdl_status status);

/**
 * @brief Create an empty model.
 *
 * @return a new handle, or NULL when allocation failed. Release it with
 * @ref lsdynatoapdl_model_destroy.
 */
LSDYNATOAPDL_API lsdynatoapdl_model* lsdynatoapdl_model_create(void);

/**
 * @brief Release a model.
 *
 * @param model handle to release; passing NULL is allowed and does nothing.
 */
LSDYNATOAPDL_API void lsdynatoapdl_model_destroy(lsdynatoapdl_model* model);

/**
 * @brief Set the decimal places used to group shell thicknesses.
 *
 * @details Call before adding elements. Lower values merge more elements into
 * a single APDL section.
 *
 * @param model the model.
 * @param decimals number of decimal places.
 * @return LSDYNATOAPDL_OK, or LSDYNATOAPDL_ERROR_INVALID_ARGUMENT.
 */
LSDYNATOAPDL_API lsdynatoapdl_status lsdynatoapdl_model_set_thickness_decimals(
    lsdynatoapdl_model* model, int decimals);

/**
 * @brief Append a node.
 *
 * @param model the model.
 * @param id node number.
 * @param x coordinate along X.
 * @param y coordinate along Y.
 * @param z coordinate along Z.
 * @return LSDYNATOAPDL_OK, or LSDYNATOAPDL_ERROR_INVALID_ARGUMENT.
 */
LSDYNATOAPDL_API lsdynatoapdl_status lsdynatoapdl_model_add_node(
    lsdynatoapdl_model* model, uint64_t id, double x, double y, double z);

/**
 * @brief Append a keypoint.
 *
 * @param model the model.
 * @param id keypoint number.
 * @param x coordinate along X.
 * @param y coordinate along Y.
 * @param z coordinate along Z.
 * @return LSDYNATOAPDL_OK, or LSDYNATOAPDL_ERROR_INVALID_ARGUMENT.
 */
LSDYNATOAPDL_API lsdynatoapdl_status lsdynatoapdl_model_add_keypoint(
    lsdynatoapdl_model* model, uint64_t id, double x, double y, double z);

/**
 * @brief Append a SHELL181 element.
 *
 * @param model the model.
 * @param id element number.
 * @param nodes array of exactly LSDYNATOAPDL_SHELL181_NODES node numbers.
 * @param thickness shell thickness.
 * @return LSDYNATOAPDL_OK, or LSDYNATOAPDL_ERROR_INVALID_ARGUMENT.
 */
LSDYNATOAPDL_API lsdynatoapdl_status lsdynatoapdl_model_add_shell181(
    lsdynatoapdl_model* model, uint64_t id,
    const uint64_t nodes[LSDYNATOAPDL_SHELL181_NODES], double thickness);

/**
 * @brief Number of nodes in the model.
 *
 * @param model the model; NULL yields 0.
 */
LSDYNATOAPDL_API size_t
lsdynatoapdl_model_node_count(const lsdynatoapdl_model* model);

/**
 * @brief Number of keypoints in the model.
 *
 * @param model the model; NULL yields 0.
 */
LSDYNATOAPDL_API size_t
lsdynatoapdl_model_keypoint_count(const lsdynatoapdl_model* model);

/**
 * @brief Number of shell elements in the model.
 *
 * @param model the model; NULL yields 0.
 */
LSDYNATOAPDL_API size_t
lsdynatoapdl_model_shell_count(const lsdynatoapdl_model* model);

/**
 * @brief Number of shell sections the model will emit.
 *
 * @param model the model; NULL yields 0.
 */
LSDYNATOAPDL_API size_t
lsdynatoapdl_model_section_count(const lsdynatoapdl_model* model);

/**
 * @brief Write the model as an APDL deck.
 *
 * @param model the model to serialise.
 * @param path destination file, overwritten if it exists.
 * @return LSDYNATOAPDL_OK on success, otherwise an error code.
 */
LSDYNATOAPDL_API lsdynatoapdl_status
lsdynatoapdl_model_write(const lsdynatoapdl_model* model, const char* path);

/**
 * @brief Render the model into a caller-provided buffer.
 *
 * @details Follows the usual two-call idiom: pass NULL to learn the required
 * size, then call again with a buffer of at least that many bytes. The written
 * text is always null-terminated when @p capacity is greater than zero.
 *
 * @param model the model to serialise.
 * @param buffer destination buffer, or NULL to query the size.
 * @param capacity bytes available in @p buffer.
 * @param required receives the number of bytes needed, including the
 * terminator; may be NULL.
 * @return LSDYNATOAPDL_OK when the deck was written completely,
 * LSDYNATOAPDL_ERROR_IO when @p buffer was too small.
 */
LSDYNATOAPDL_API lsdynatoapdl_status
lsdynatoapdl_model_render(const lsdynatoapdl_model* model, char* buffer,
                          size_t capacity, size_t* required);

/**
 * @brief Convert a LS-DYNA deck into an APDL deck in one call.
 *
 * @param input_path the LS-DYNA keyword deck to read.
 * @param output_path the APDL deck to write.
 * @param thickness_decimals decimal places used to group shell thicknesses;
 * pass a negative value for the default.
 * @return LSDYNATOAPDL_OK on success, otherwise an error code.
 */
LSDYNATOAPDL_API lsdynatoapdl_status lsdynatoapdl_convert_file(
    const char* input_path, const char* output_path, int thickness_decimals);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LSDYNA_TO_APDL_C_API_H */
