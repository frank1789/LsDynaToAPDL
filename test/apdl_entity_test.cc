/**
 * @file apdl_entity_test.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Tests for the CRTP APDL entities and the deck writer.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "apdl/apdl_entity.hh"

#include <fmt/format.h>
#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "apdl/deck_writer.hh"
#include "apdl/keypoint.hh"
#include "apdl/material.hh"
#include "apdl/model.hh"
#include "apdl/node.hh"
#include "apdl/section.hh"
#include "apdl/shell181.hh"

namespace {

using lsdynatoapdl::apdl::DeckWriter;
using lsdynatoapdl::apdl::ElementType;
using lsdynatoapdl::apdl::Keypoint;
using lsdynatoapdl::apdl::Material;
using lsdynatoapdl::apdl::MaterialProperty;
using lsdynatoapdl::apdl::Model;
using lsdynatoapdl::apdl::Node;
using lsdynatoapdl::apdl::Shell181;
using lsdynatoapdl::apdl::ShellSection;

/// Renders a model into a string so assertions can look at the whole deck.
auto render(const Model& model) -> std::string {
  std::ostringstream os;
  DeckWriter::write(model, os);
  return os.str();
}

}  // namespace

// ---------------------------------------------------------------------------
// The CRTP contract
// ---------------------------------------------------------------------------

TEST(ApdlEntity, EveryMandatoryEntityIsPrintable) {
  static_assert(lsdynatoapdl::apdl::ApdlPrintable<Node>);
  static_assert(lsdynatoapdl::apdl::ApdlPrintable<Keypoint>);
  static_assert(lsdynatoapdl::apdl::ApdlPrintable<Shell181>);
  static_assert(lsdynatoapdl::apdl::ApdlPrintable<ElementType>);
  static_assert(lsdynatoapdl::apdl::ApdlPrintable<ShellSection>);
  SUCCEED();
}

TEST(ApdlEntity, EntitiesStayValueTypesWithoutAVtable) {
  // The whole point of CRTP over runtime polymorphism: a deck holding millions
  // of nodes must not pay for a vtable pointer per entity.
  static_assert(not std::is_polymorphic_v<Node>);
  static_assert(not std::is_polymorphic_v<Shell181>);
  static_assert(std::is_trivially_copyable_v<Keypoint>);
  EXPECT_EQ(sizeof(Keypoint), sizeof(std::uint64_t) + (3 * sizeof(double)));
}

TEST(ApdlEntity, AllThreePrintingRoutesAgree) {
  const Node node{7, 1.0, 2.0, 3.0};
  const auto expected = node.to_apdl();

  // 1. ostream
  std::ostringstream os;
  os << node;
  EXPECT_EQ(os.str(), expected);

  // 2. fmt
  EXPECT_EQ(fmt::format("{}", node), expected);

#if LSDYNATOAPDL_HAS_STD_FORMAT
  // 3. std::format
  EXPECT_EQ(std::format("{}", node), expected);
#else
  GTEST_SKIP() << "std::format is unavailable in this standard library";
#endif
}

TEST(ApdlEntity, EveryEntityWorksThroughEveryRoute) {
  const Keypoint keypoint{3, 0.0, 12.5, -4.25};
  const Shell181 shell{9, {1, 2, 3, 4}, 1.5};
  const ElementType element_type{1, "SHELL181"};
  const ShellSection section{2, 3.5};

  const auto check = [](const auto& entity) {
    std::ostringstream os;
    os << entity;
    EXPECT_EQ(os.str(), entity.to_apdl());
    EXPECT_EQ(fmt::format("{}", entity), entity.to_apdl());
#if LSDYNATOAPDL_HAS_STD_FORMAT
    EXPECT_EQ(std::format("{}", entity), entity.to_apdl());
#endif
  };

  check(keypoint);
  check(shell);
  check(element_type);
  check(section);
}

TEST(ApdlEntity, FormatSpecifiersReachTheUnderlyingString) {
  const Keypoint keypoint{1, 0.0, 0.0, 0.0};
  // The formatters inherit from the std::string formatter, so width and
  // alignment work on the rendered command rather than being rejected.
  EXPECT_EQ(fmt::format("[{:>16}]", keypoint), "[   K, 1, 0, 0, 0]");
#if LSDYNATOAPDL_HAS_STD_FORMAT
  EXPECT_EQ(std::format("[{:>16}]", keypoint), "[   K, 1, 0, 0, 0]");
#endif
}

TEST(ApdlEntity, EntitiesCompareByValue) {
  // Regression: `operator== = default` is implicitly deleted unless the CRTP
  // base provides one, so these comparisons used to fail to compile the moment
  // anything actually used them.
  EXPECT_EQ(Node(1, 1.0, 2.0, 3.0), Node(1, 1.0, 2.0, 3.0));
  EXPECT_NE(Node(1, 1.0, 2.0, 3.0), Node(2, 1.0, 2.0, 3.0));
  EXPECT_NE(Node(1, 1.0, 2.0, 3.0), Node(1, 9.0, 2.0, 3.0));

  EXPECT_EQ(Keypoint(1, 0.0, 0.0, 0.0), Keypoint(1, 0.0, 0.0, 0.0));
  EXPECT_NE(Keypoint(1, 0.0, 0.0, 0.0), Keypoint(1, 0.0, 0.0, 1.0));

  EXPECT_EQ(Shell181(1, {1, 2, 3, 4}, 2.0), Shell181(1, {1, 2, 3, 4}, 2.0));
  EXPECT_NE(Shell181(1, {1, 2, 3, 4}, 2.0), Shell181(1, {1, 2, 3, 5}, 2.0));
  EXPECT_NE(Shell181(1, {1, 2, 3, 4}, 2.0), Shell181(1, {1, 2, 3, 4}, 3.0));
}

// ---------------------------------------------------------------------------
// Node — the N command
// ---------------------------------------------------------------------------

TEST(ApdlNode, RendersTheNCommand) {
  const Node node{1162886, -25.5204048, 320.880554, -161.429962};
  EXPECT_EQ(node.to_apdl(), "N, 1162886, -25.5204048, 320.880554, -161.429962");
}

TEST(ApdlNode, OmitsUnsetRotations) {
  const Node node{1, 0.0, 0.0, 0.0};
  EXPECT_EQ(node.to_apdl(), "N, 1, 0, 0, 0");
}

TEST(ApdlNode, WritesAllThreeRotations) {
  const Node node{1, 0.0, 0.0, 0.0, 10.0, 20.0, 30.0};
  EXPECT_EQ(node.to_apdl(), "N, 1, 0, 0, 0, 10, 20, 30");
}

TEST(ApdlNode, PadsAGapBetweenRotations) {
  // THXY unset but THYZ set: APDL is positional, so the empty slot has to be
  // kept, otherwise 45 degrees would be read as THXY.
  const Node node{1, 0.0, 0.0, 0.0, std::nullopt, 45.0, std::nullopt};
  EXPECT_EQ(node.to_apdl(), "N, 1, 0, 0, 0, , 45");
}

TEST(ApdlNode, KeepsFullPrecisionOfTheSourceDeck) {
  const Node node{1, -240.277832, 281.437195, -55.9616051};
  EXPECT_EQ(node.to_apdl(), "N, 1, -240.277832, 281.437195, -55.9616051");
}

// ---------------------------------------------------------------------------
// Keypoint — the K command
// ---------------------------------------------------------------------------

TEST(ApdlKeypoint, RendersTheKCommand) {
  const Keypoint keypoint{3, 0.0, 12.5, -4.25};
  EXPECT_EQ(keypoint.to_apdl(), "K, 3, 0, 12.5, -4.25");
}

// ---------------------------------------------------------------------------
// Shell181 — the EN command
// ---------------------------------------------------------------------------

TEST(ApdlShell181, RendersTheEnCommand) {
  const Shell181 shell{
      1229174, {1189339, 1162886, 1189341, 1189340}, 3.9788582};
  EXPECT_EQ(shell.to_apdl(), "EN, 1229174, 1189339, 1162886, 1189341, 1189340");
}

TEST(ApdlShell181, DetectsADegenerateTriangle) {
  const Shell181 triangle{1, {10, 11, 12, 12}, 1.0};
  const Shell181 quad{2, {10, 11, 12, 13}, 1.0};

  EXPECT_TRUE(triangle.is_degenerate());
  EXPECT_FALSE(quad.is_degenerate());
  // A degenerate quad is still written with four nodes: that is how SHELL181
  // expects a triangular element to be given.
  EXPECT_EQ(triangle.to_apdl(), "EN, 1, 10, 11, 12, 12");
}

// ---------------------------------------------------------------------------
// Sections
// ---------------------------------------------------------------------------

TEST(ApdlSection, RendersElementTypeAndShellSection) {
  EXPECT_EQ(ElementType(1, "SHELL181").to_apdl(), "ET, 1, SHELL181");
  EXPECT_EQ(ShellSection(2, 3.5).to_apdl(),
            "SECTYPE, 2, SHELL, , shell_2\nSECDATA, 3.5, 1, 0, 3");
}

TEST(ApdlModel, GroupsEqualThicknessesIntoOneSection) {
  Model model;
  model.add_shell(1, {1, 2, 3, 4}, 2.0);
  model.add_shell(2, {5, 6, 7, 8}, 2.0);
  model.add_shell(3, {9, 10, 11, 12}, 4.0);

  ASSERT_EQ(model.sections().size(), 2U);
  EXPECT_EQ(model.section_of(model.shells()[0]),
            model.section_of(model.shells()[1]));
  EXPECT_NE(model.section_of(model.shells()[0]),
            model.section_of(model.shells()[2]));
}

TEST(ApdlModel, MergesThicknessesThatAgreeToTheConfiguredPrecision) {
  Model model;
  model.set_thickness_decimals(3);
  model.add_shell(1, {1, 2, 3, 4}, 3.97885820);
  model.add_shell(2, {5, 6, 7, 8}, 3.97885899);

  EXPECT_EQ(model.sections().size(), 1U);
}

// ---------------------------------------------------------------------------
// Deck writer
// ---------------------------------------------------------------------------

TEST(ApdlDeckWriter, EmitsAPreprocessorBlock) {
  Model model;
  model.add_node(Node{1, 0.0, 0.0, 0.0});

  const auto deck = render(model);

  EXPECT_NE(deck.find("/PREP7\n"), std::string::npos);
  EXPECT_NE(deck.find("N, 1, 0, 0, 0\n"), std::string::npos);
  EXPECT_NE(deck.find("FINISH\n"), std::string::npos);
}

TEST(ApdlDeckWriter, DeclaresElementTypeAndSectionsBeforeElements) {
  Model model;
  model.add_node(Node{1, 0.0, 0.0, 0.0});
  model.add_shell(10, {1, 2, 3, 4}, 2.5);

  const auto deck = render(model);

  const auto et = deck.find("ET, 1, SHELL181");
  const auto sectype = deck.find("SECTYPE, 1, SHELL");
  const auto element = deck.find("EN, 10,");

  ASSERT_NE(et, std::string::npos);
  ASSERT_NE(sectype, std::string::npos);
  ASSERT_NE(element, std::string::npos);
  EXPECT_LT(et, sectype);
  EXPECT_LT(sectype, element);
}

TEST(ApdlDeckWriter, EmitsSecnumOncePerSectionGroup) {
  Model model;
  model.add_shell(1, {1, 2, 3, 4}, 2.0);
  model.add_shell(2, {5, 6, 7, 8}, 4.0);
  model.add_shell(3, {9, 10, 11, 12}, 2.0);

  const auto deck = render(model);

  // Three elements but only two thicknesses, so SECNUM appears twice, not
  // three times: elements are reordered so each section is written once.
  auto occurrences = 0;
  for (auto pos = deck.find("SECNUM, "); pos != std::string::npos;
       pos = deck.find("SECNUM, ", pos + 1)) {
    ++occurrences;
  }
  EXPECT_EQ(occurrences, 2);
}

TEST(ApdlDeckWriter, WritesKeypointsBeforeNodes) {
  Model model;
  model.add_keypoint(Keypoint{1, 1.0, 0.0, 0.0});
  model.add_node(Node{5, 0.0, 0.0, 0.0});

  const auto deck = render(model);

  EXPECT_LT(deck.find("K, 1,"), deck.find("N, 5,"));
}

TEST(ApdlDeckWriter, RefusesToWriteWithoutAFilename) {
  const DeckWriter writer;
  Model model;
  model.add_node(Node{1, 0.0, 0.0, 0.0});

  EXPECT_FALSE(writer.write(model));
}

// ---------------------------------------------------------------------------
// Materials, supplied from the command line
// ---------------------------------------------------------------------------

TEST(ApdlMaterial, DeclaresParametersTheReferenceDeckWay) {
  const Material dural{"Dural", "72e9", "0.33", "2810"};

  const auto block = dural.to_apdl();
  EXPECT_NE(block.find("!*** Dural"), std::string::npos);
  EXPECT_NE(block.find("*SET, DuralEyounG, 72e9"), std::string::npos);
  EXPECT_NE(block.find("*SET, DuralNi, 0.33"), std::string::npos);
  EXPECT_NE(block.find("*SET, DuralDensity, 2810"), std::string::npos);
}

TEST(ApdlMaterial, KeepsTheValueTextTheUserTyped) {
  // 72e9 is how an engineer writes a Young's modulus; re-rendering it from a
  // double would print 72000000000 and change their input for no reason.
  const Material dural{"Dural", "72e9", "0.33", "2810"};
  EXPECT_NE(dural.to_apdl().find("72e9"), std::string::npos);
  EXPECT_EQ(dural.to_apdl().find("72000000000"), std::string::npos);
}

TEST(ApdlMaterial, BindsParametersToAMaterialNumber) {
  const MaterialProperty property{2, "Aluminium"};
  EXPECT_EQ(property.to_apdl(),
            "MP, EX, 2, AluminiumEyounG\n"
            "MP, NUXY, 2, AluminiumNi\n"
            "MP, DENS, 2, AluminiumDensity");
}

// ---------------------------------------------------------------------------
// The deck preamble
// ---------------------------------------------------------------------------

TEST(ApdlDeckWriter, OpensWithACommentedBannerAndSessionSetup) {
  Model model;
  model.set_title("Helicopter tail boom modal analysis shell model");
  model.set_jobname("Shellmodel");
  model.set_source("tail.k");
  model.add_node(Node{1, 0.0, 0.0, 0.0});

  const auto deck = render(model);

  EXPECT_NE(deck.find("/COM,   PROBLEM: Helicopter tail boom"),
            std::string::npos);
  EXPECT_NE(deck.find("/COM,   Converted from tail.k"), std::string::npos);
  EXPECT_NE(deck.find("\nFINISH\n/CLEAR, START, NEW\n"), std::string::npos);
  EXPECT_NE(deck.find("/FILNAM, Shellmodel"), std::string::npos);
  EXPECT_NE(deck.find("/TITLE, Helicopter tail boom"), std::string::npos);
  EXPECT_NE(deck.find("/UNIT, SI"), std::string::npos);
  EXPECT_NE(deck.find("/INQUIRE, StrJobname, JOBNAME"), std::string::npos);
  EXPECT_NE(deck.find("*SET, Pi, ACOS(-1)"), std::string::npos);
  EXPECT_NE(deck.find("*AFUN, DEG"), std::string::npos);
  EXPECT_NE(deck.find("*SET, eps, 10e-3"), std::string::npos);
}

TEST(ApdlDeckWriter, PutsParametersBeforePrep7AndTheModelInside) {
  Model model;
  model.add_material(Material{"Dural", "72e9", "0.33", "2810"});
  model.add_node(Node{1, 0.0, 0.0, 0.0});
  model.add_shell(10, {1, 2, 3, 4}, 2.5);

  const auto deck = render(model);

  const auto parameters = deck.find("*SET, DuralEyounG");
  const auto prep7 = deck.find("/PREP7");
  const auto material = deck.find("MP, EX, 1, DuralEyounG");
  const auto node = deck.find("N, 1, 0, 0, 0");
  const auto element = deck.find("EN, 10,");
  const auto finish = deck.rfind("FINISH");

  ASSERT_NE(parameters, std::string::npos);
  ASSERT_NE(prep7, std::string::npos);
  ASSERT_NE(material, std::string::npos);
  ASSERT_NE(node, std::string::npos);
  ASSERT_NE(element, std::string::npos);

  // *SET parameters are session-level and belong before the preprocessor;
  // everything that defines the model belongs after it.
  EXPECT_LT(parameters, prep7);
  EXPECT_LT(prep7, material);
  EXPECT_LT(material, node);
  EXPECT_LT(node, element);
  EXPECT_LT(element, finish);
}

TEST(ApdlDeckWriter, SaysSoWhenNoMaterialWasSupplied) {
  Model model;
  model.add_shell(1, {1, 2, 3, 4}, 2.0);

  const auto deck = render(model);

  // A deck whose sections refer to material 1 while declaring none stops in
  // Ansys; the reason should be in the file, not only in the log.
  EXPECT_NE(deck.find("!*** none supplied"), std::string::npos);
  EXPECT_EQ(deck.find("MP, EX,"), std::string::npos);
}
