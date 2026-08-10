/**
 * @file c_api.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Implementation of the stable C API.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <algorithm>
#include <cstring>
#include <exception>
#include <filesystem>
#include <new>
#include <sstream>
#include <string>
#include <utility>

#include "apdl/deck_writer.hh"
#include "apdl/model.hh"
#include "lsdyna/converter.hh"
#include "lsdynatoapdl/lsdynatoapdl.h"

/**
 * @brief The opaque handle is simply an APDL model.
 */
struct lsdynatoapdl_model {
  lsdynatoapdl::apdl::Model model;
};

namespace {

/**
 * @brief Run @p action, turning any exception into a status code.
 *
 * @details Every entry point funnels through here. Letting a C++ exception
 * unwind through a C caller's frame is undefined behaviour, so the boundary
 * has to be exception-tight even for failures nobody expects, such as
 * std::bad_alloc.
 *
 * @param action the work to perform.
 * @return the status returned by @p action, or an error describing the
 * exception that escaped it.
 */
template <typename Action>
auto guarded(Action&& action) noexcept -> lsdynatoapdl_status {
  try {
    return std::forward<Action>(action)();
  } catch (const std::bad_alloc&) {
    return LSDYNATOAPDL_ERROR_UNKNOWN;
  } catch (const std::filesystem::filesystem_error&) {
    return LSDYNATOAPDL_ERROR_IO;
  } catch (const std::exception&) {
    return LSDYNATOAPDL_ERROR_UNKNOWN;
  } catch (...) {
    return LSDYNATOAPDL_ERROR_UNKNOWN;
  }
}

/**
 * @brief Serialise @p model into a string.
 */
auto render_model(const lsdynatoapdl::apdl::Model& model) -> std::string {
  std::ostringstream os;
  lsdynatoapdl::apdl::DeckWriter::write(model, os);
  return os.str();
}

}  // namespace

extern "C" {

const char* lsdynatoapdl_version(void) { return LSDYNATOAPDL_VERSION; }

const char* lsdynatoapdl_status_message(lsdynatoapdl_status status) {
  switch (status) {
    case LSDYNATOAPDL_OK:
      return "success";
    case LSDYNATOAPDL_ERROR_INVALID_ARGUMENT:
      return "invalid argument";
    case LSDYNATOAPDL_ERROR_IO:
      return "input/output error";
    case LSDYNATOAPDL_ERROR_PARSE:
      return "the input deck could not be parsed";
    case LSDYNATOAPDL_ERROR_EMPTY:
      return "the input deck produced no entities";
    case LSDYNATOAPDL_ERROR_UNKNOWN:
      break;
  }
  return "unknown error";
}

lsdynatoapdl_model* lsdynatoapdl_model_create(void) {
  // The caller gets NULL rather than an exception when allocation fails.
  return new (std::nothrow) lsdynatoapdl_model{};
}

void lsdynatoapdl_model_destroy(lsdynatoapdl_model* model) { delete model; }

lsdynatoapdl_status lsdynatoapdl_model_set_thickness_decimals(
    lsdynatoapdl_model* model, int decimals) {
  if (model == nullptr) {
    return LSDYNATOAPDL_ERROR_INVALID_ARGUMENT;
  }
  return guarded([&]() {
    model->model.set_thickness_decimals(decimals);
    return LSDYNATOAPDL_OK;
  });
}

lsdynatoapdl_status lsdynatoapdl_model_add_node(lsdynatoapdl_model* model,
                                                uint64_t id, double x, double y,
                                                double z) {
  if (model == nullptr) {
    return LSDYNATOAPDL_ERROR_INVALID_ARGUMENT;
  }
  return guarded([&]() {
    model->model.add_node(lsdynatoapdl::apdl::Node{id, x, y, z});
    return LSDYNATOAPDL_OK;
  });
}

lsdynatoapdl_status lsdynatoapdl_model_add_keypoint(lsdynatoapdl_model* model,
                                                    uint64_t id, double x,
                                                    double y, double z) {
  if (model == nullptr) {
    return LSDYNATOAPDL_ERROR_INVALID_ARGUMENT;
  }
  return guarded([&]() {
    model->model.add_keypoint(lsdynatoapdl::apdl::Keypoint{id, x, y, z});
    return LSDYNATOAPDL_OK;
  });
}

lsdynatoapdl_status lsdynatoapdl_model_add_shell181(
    lsdynatoapdl_model* model, uint64_t id,
    const uint64_t nodes[LSDYNATOAPDL_SHELL181_NODES], double thickness) {
  if (model == nullptr || nodes == nullptr) {
    return LSDYNATOAPDL_ERROR_INVALID_ARGUMENT;
  }
  return guarded([&]() {
    lsdynatoapdl::apdl::Shell181::NodeIds connectivity{};
    std::copy_n(nodes, LSDYNATOAPDL_SHELL181_NODES, connectivity.begin());
    model->model.add_shell(id, connectivity, thickness);
    return LSDYNATOAPDL_OK;
  });
}

size_t lsdynatoapdl_model_node_count(const lsdynatoapdl_model* model) {
  return model == nullptr ? 0U : model->model.nodes().size();
}

size_t lsdynatoapdl_model_keypoint_count(const lsdynatoapdl_model* model) {
  return model == nullptr ? 0U : model->model.keypoints().size();
}

size_t lsdynatoapdl_model_shell_count(const lsdynatoapdl_model* model) {
  return model == nullptr ? 0U : model->model.shells().size();
}

size_t lsdynatoapdl_model_section_count(const lsdynatoapdl_model* model) {
  return model == nullptr ? 0U : model->model.sections().size();
}

lsdynatoapdl_status lsdynatoapdl_model_write(const lsdynatoapdl_model* model,
                                             const char* path) {
  if (model == nullptr || path == nullptr || *path == '\0') {
    return LSDYNATOAPDL_ERROR_INVALID_ARGUMENT;
  }
  return guarded([&]() {
    lsdynatoapdl::apdl::DeckWriter writer;
    writer.set_filename(std::filesystem::path{path});
    return writer.write(model->model) ? LSDYNATOAPDL_OK : LSDYNATOAPDL_ERROR_IO;
  });
}

lsdynatoapdl_status lsdynatoapdl_model_render(const lsdynatoapdl_model* model,
                                              char* buffer, size_t capacity,
                                              size_t* required) {
  if (model == nullptr) {
    return LSDYNATOAPDL_ERROR_INVALID_ARGUMENT;
  }
  return guarded([&]() {
    const auto deck = render_model(model->model);
    const auto needed = deck.size() + 1U;
    if (required != nullptr) {
      *required = needed;
    }
    if (buffer == nullptr || capacity == 0U) {
      return LSDYNATOAPDL_ERROR_IO;
    }
    if (capacity < needed) {
      // Still terminate, so a caller that ignores the status does not read
      // past the end of its own buffer.
      buffer[0] = '\0';
      return LSDYNATOAPDL_ERROR_IO;
    }
    std::memcpy(buffer, deck.data(), deck.size());
    buffer[deck.size()] = '\0';
    return LSDYNATOAPDL_OK;
  });
}

lsdynatoapdl_status lsdynatoapdl_convert_file(const char* input_path,
                                              const char* output_path,
                                              int thickness_decimals) {
  if (input_path == nullptr || *input_path == '\0' || output_path == nullptr ||
      *output_path == '\0') {
    return LSDYNATOAPDL_ERROR_INVALID_ARGUMENT;
  }

  return guarded([&]() {
    lsdynatoapdl::lsdyna::ConverterSyntax converter;
    if (thickness_decimals >= 0) {
      converter.set_thickness_decimals(thickness_decimals);
    }

    converter.set_input_file(std::filesystem::path{input_path});
    if (not converter.is_ready()) {
      return LSDYNATOAPDL_ERROR_IO;
    }
    if (not converter.parse()) {
      return LSDYNATOAPDL_ERROR_PARSE;
    }
    if (converter.model().empty()) {
      return LSDYNATOAPDL_ERROR_EMPTY;
    }

    lsdynatoapdl::apdl::DeckWriter writer;
    writer.set_filename(std::filesystem::path{output_path});
    return writer.write(converter.model()) ? LSDYNATOAPDL_OK
                                           : LSDYNATOAPDL_ERROR_IO;
  });
}

}  // extern "C"
