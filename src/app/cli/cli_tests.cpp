// Aseprite
// Copyright (C) 2016  David Capello
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.

#include "tests/test.h"

#include "app/cli/app_options.h"
#include "app/cli/cli_processor.h"
#include "app/document_exporter.h"

#include <initializer_list>

using namespace app;

class CliTestDelegate : public CliDelegate {
public:
  CliTestDelegate() {
    m_helpWasShown = false;
    m_versionWasShown = false;
    m_uiMode = false;
    m_shellMode = false;
    m_batchMode = false;
  }

  void showHelp(const AppOptions& options) override { m_helpWasShown = true; }
  void showVersion() override { m_versionWasShown = true; }
  void uiMode() override { m_uiMode = true; }
  void shellMode() override { m_shellMode = true; }
  void batchMode() override { m_batchMode = true; }
  void beforeOpenFile(const CliOpenFile& cof) override { }
  void afterOpenFile(const CliOpenFile& cof) override { }
  void saveFile(const CliOpenFile& cof) override { }
  void exportFiles(DocumentExporter& exporter) override { }
  void execScript(const std::string& filename) override { }

  bool helpWasShown() const { return m_helpWasShown; }
  bool versionWasShown() const { return m_versionWasShown; }

private:
  bool m_helpWasShown;
  bool m_versionWasShown;
  bool m_uiMode;
  bool m_shellMode;
  bool m_batchMode;
};

AppOptions args(std::initializer_list<const char*> l) {
  int argc = l.size()+1;
  const char** argv = new const char*[argc];
  argv[0] = "aseprite.exe";
  auto it = l.begin();
  for (int i=1; i<argc; ++i, ++it) {
    argv[i] = *it;
    TRACE("argv[%d] = %s\n", i, argv[i]);
  }
  AppOptions opts(argc, argv);
  delete[] argv;
  return opts;
}

TEST(Cli, None)
{
  CliTestDelegate d;
  CliProcessor p(&d, args({ }));
  p.process();
  EXPECT_TRUE(!d.helpWasShown());
  EXPECT_TRUE(!d.versionWasShown());
}

TEST(Cli, Help)
{
  CliTestDelegate d;
  CliProcessor p(&d, args({ "--help" }));
  p.process();
  EXPECT_TRUE(d.helpWasShown());
}

TEST(Cli, Version)
{
  CliTestDelegate d;
  CliProcessor p(&d, args({ "--version" }));
  p.process();
  EXPECT_TRUE(d.versionWasShown());
}