load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

def include_third_party_repositories():
    http_archive(
        name = "com_github_gabime_spdlog",
        build_file = "//thirdparty:spdlog.BUILD",
        sha256 = "4dccf2d10f410c1e2feaff89966bfc49a1abb29ef6f08246335b110e001e09a9",
        strip_prefix = "spdlog-1.12.0",
        urls = ["https://github.com/gabime/spdlog/archive/v1.12.0.tar.gz"],
    )
