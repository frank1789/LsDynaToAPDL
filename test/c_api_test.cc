/**
 * @file c_api_test.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Tests for the stable C API.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "lsdynatoapdl/lsdynatoapdl.h"

namespace {

/// Owns a handle so a failing assertion cannot leak it.
class ModelHandle {
 public:
  ModelHandle() : m_model(lsdynatoapdl_model_create()) {}
  ~ModelHandle() { lsdynatoapdl_model_destroy(m_model); }

  ModelHandle(const ModelHandle&) = delete;
  ModelHandle(ModelHandle&&) = delete;
  auto operator=(const ModelHandle&) -> ModelHandle& = delete;
  auto operator=(ModelHandle&&) -> ModelHandle& = delete;

  [[nodiscard]] auto get() const noexcept -> lsdynatoapdl_model* {
    return m_model;
  }

 private:
  lsdynatoapdl_model* m_model;
};

/// Removes its file when the test ends, whatever the outcome.
class TemporaryFile {
 public:
  explicit TemporaryFile(std::string name) :
      m_path(std::filesystem::temp_directory_path() / std::move(name)) {}
  ~TemporaryFile() {
    std::error_code ignored;
    std::filesystem::remove(m_path, ignored);
  }

  TemporaryFile(const TemporaryFile&) = delete;
  TemporaryFile(TemporaryFile&&) = delete;
  auto operator=(const TemporaryFile&) -> TemporaryFile& = delete;
  auto operator=(TemporaryFile&&) -> TemporaryFile& = delete;

  [[nodiscard]] auto path() const -> const std::filesystem::path& {
    return m_path;
  }

  [[nodiscard]] auto read() const -> std::string {
    const std::ifstream ifs(m_path);
    std::ostringstream os;
    os << ifs.rdbuf();
    return os.str();
  }

  void write(std::string_view content) const {
    std::ofstream ofs(m_path);
    ofs << content;
  }

 private:
  std::filesystem::path m_path;
};

}  // namespace

TEST(CApi, ReportsVersionAndStatusMessages) {
  EXPECT_STRNE(lsdynatoapdl_version(), "");
  EXPECT_STREQ(lsdynatoapdl_status_message(LSDYNATOAPDL_OK), "success");
  EXPECT_STRNE(lsdynatoapdl_status_message(LSDYNATOAPDL_ERROR_IO), "");
}

TEST(CApi, BuildsAModelThroughTheHandle) {
  const ModelHandle handle;
  ASSERT_NE(handle.get(), nullptr);

  EXPECT_EQ(lsdynatoapdl_model_add_node(handle.get(), 1, 0.0, 0.0, 0.0),
            LSDYNATOAPDL_OK);
  EXPECT_EQ(lsdynatoapdl_model_add_keypoint(handle.get(), 1, 1.0, 2.0, 3.0),
            LSDYNATOAPDL_OK);

  const std::uint64_t nodes[LSDYNATOAPDL_SHELL181_NODES] = {1, 2, 3, 4};
  EXPECT_EQ(lsdynatoapdl_model_add_shell181(handle.get(), 10, nodes, 2.5),
            LSDYNATOAPDL_OK);

  EXPECT_EQ(lsdynatoapdl_model_node_count(handle.get()), 1U);
  EXPECT_EQ(lsdynatoapdl_model_keypoint_count(handle.get()), 1U);
  EXPECT_EQ(lsdynatoapdl_model_shell_count(handle.get()), 1U);
  EXPECT_EQ(lsdynatoapdl_model_section_count(handle.get()), 1U);
}

TEST(CApi, RejectsNullHandlesInsteadOfCrashing) {
  const std::uint64_t nodes[LSDYNATOAPDL_SHELL181_NODES] = {1, 2, 3, 4};

  EXPECT_EQ(lsdynatoapdl_model_add_node(nullptr, 1, 0.0, 0.0, 0.0),
            LSDYNATOAPDL_ERROR_INVALID_ARGUMENT);
  EXPECT_EQ(lsdynatoapdl_model_add_shell181(nullptr, 1, nodes, 1.0),
            LSDYNATOAPDL_ERROR_INVALID_ARGUMENT);
  EXPECT_EQ(lsdynatoapdl_model_write(nullptr, "x"),
            LSDYNATOAPDL_ERROR_INVALID_ARGUMENT);
  EXPECT_EQ(lsdynatoapdl_convert_file(nullptr, "out", -1),
            LSDYNATOAPDL_ERROR_INVALID_ARGUMENT);
  EXPECT_EQ(lsdynatoapdl_model_node_count(nullptr), 0U);

  // Destroying a null handle must be a no-op, like free(NULL).
  lsdynatoapdl_model_destroy(nullptr);
}

TEST(CApi, RejectsANullNodeArray) {
  const ModelHandle handle;
  EXPECT_EQ(lsdynatoapdl_model_add_shell181(handle.get(), 1, nullptr, 1.0),
            LSDYNATOAPDL_ERROR_INVALID_ARGUMENT);
}

TEST(CApi, RendersWithTheTwoCallSizeIdiom) {
  const ModelHandle handle;
  ASSERT_EQ(lsdynatoapdl_model_add_node(handle.get(), 7, 1.0, 2.0, 3.0),
            LSDYNATOAPDL_OK);

  std::size_t required{0};
  EXPECT_EQ(lsdynatoapdl_model_render(handle.get(), nullptr, 0, &required),
            LSDYNATOAPDL_ERROR_IO);
  ASSERT_GT(required, 1U);

  std::vector<char> buffer(required);
  ASSERT_EQ(lsdynatoapdl_model_render(handle.get(), buffer.data(),
                                      buffer.size(), nullptr),
            LSDYNATOAPDL_OK);

  const std::string deck(buffer.data());
  EXPECT_NE(deck.find("N, 7, 1, 2, 3"), std::string::npos);
  EXPECT_NE(deck.find("/PREP7"), std::string::npos);
}

TEST(CApi, TerminatesTheBufferWhenItIsTooSmall) {
  const ModelHandle handle;
  ASSERT_EQ(lsdynatoapdl_model_add_node(handle.get(), 7, 1.0, 2.0, 3.0),
            LSDYNATOAPDL_OK);

  std::vector<char> buffer(4, 'x');
  EXPECT_EQ(lsdynatoapdl_model_render(handle.get(), buffer.data(),
                                      buffer.size(), nullptr),
            LSDYNATOAPDL_ERROR_IO);
  // A caller that ignores the status must still not read past its buffer.
  EXPECT_EQ(buffer[0], '\0');
}

TEST(CApi, WritesADeckToDisk) {
  const ModelHandle handle;
  const std::uint64_t nodes[LSDYNATOAPDL_SHELL181_NODES] = {1, 2, 3, 4};
  ASSERT_EQ(lsdynatoapdl_model_add_node(handle.get(), 1, 0.0, 0.0, 0.0),
            LSDYNATOAPDL_OK);
  ASSERT_EQ(lsdynatoapdl_model_add_shell181(handle.get(), 10, nodes, 2.5),
            LSDYNATOAPDL_OK);

  const TemporaryFile output{"lsdynatoapdl_c_api_write.inp"};
  ASSERT_EQ(
      lsdynatoapdl_model_write(handle.get(), output.path().string().c_str()),
      LSDYNATOAPDL_OK);

  const auto deck = output.read();
  EXPECT_NE(deck.find("ET, 1, SHELL181"), std::string::npos);
  EXPECT_NE(deck.find("EN, 10, 1, 2, 3, 4"), std::string::npos);
}

TEST(CApi, ConvertsAWholeDeckInOneCall) {
  const TemporaryFile input{"lsdynatoapdl_c_api_input.k"};
  input.write(
      "*KEYWORD\n"
      "*NODE\n"
      " 1162886     -25.5204048      320.880554     -161.429962     0.0\n"
      " 1188851     -19.4350681      316.302582     -161.181335     0.0\n"
      "*ELEMENT_SHELL_THICKNESS\n"
      " 1229174       4 1189339 1162886 1189341 1189340\n"
      "       3.9788582       3.9788582       3.9788582       3.9788582\n"
      "*END\n");

  const TemporaryFile output{"lsdynatoapdl_c_api_output.inp"};
  ASSERT_EQ(lsdynatoapdl_convert_file(input.path().string().c_str(),
                                      output.path().string().c_str(), 3),
            LSDYNATOAPDL_OK);

  const auto deck = output.read();
  EXPECT_NE(deck.find("N, 1162886,"), std::string::npos);
  EXPECT_NE(deck.find("EN, 1229174, 1189339, 1162886, 1189341, 1189340"),
            std::string::npos);
  EXPECT_NE(deck.find("FINISH"), std::string::npos);
}

TEST(CApi, ReportsAMissingInputFile) {
  const TemporaryFile output{"lsdynatoapdl_c_api_missing.inp"};
  EXPECT_EQ(lsdynatoapdl_convert_file("this_file_does_not_exist.k",
                                      output.path().string().c_str(), -1),
            LSDYNATOAPDL_ERROR_IO);
}
