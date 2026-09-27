/**
 * Dialog
 * Allan Legemaate
 * Native message boxes and file choosers
 * 27/09/2026
 **/

#pragma once

#include <optional>
#include <string>

namespace Dialog {

void info(const std::string& title, const std::string& message);
void error(const std::string& title, const std::string& message);

// Ask a yes or no question, true on yes
bool confirm(const std::string& title, const std::string& message);

enum class FileMode { Open, Save };

// Open a level file chooser. The choice arrives later through takeFile().
void requestFile(FileMode mode, const std::string& default_location);

// True while a file chooser is open
bool filePending();

// The chosen file, once. Empty while waiting or when cancelled.
std::optional<std::string> takeFile();

}  // namespace Dialog
