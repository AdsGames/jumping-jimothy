#include "Dialog.h"

#include <asw/asw.h>
#include <array>
#include <atomic>
#include <mutex>
#include <utility>

namespace {
// SDL may run the file chooser callback on another thread
std::mutex file_mutex;
std::optional<std::string> chosen_file;
std::atomic<bool> file_pending = false;

void SDLCALL onFileChosen(void* /*userdata*/,
                          const char* const* files,
                          int /*filter*/) {
  if (files == nullptr) {
    asw::log::warn("File chooser failed: {}", SDL_GetError());
  } else if (files[0] != nullptr) {
    const std::scoped_lock lock(file_mutex);
    chosen_file = files[0];
  }

  file_pending = false;
}

void showMessage(SDL_MessageBoxFlags flags,
                 const std::string& title,
                 const std::string& message) {
  SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(),
                           asw::display::get_window());
}
}  // namespace

void Dialog::info(const std::string& title, const std::string& message) {
  showMessage(SDL_MESSAGEBOX_INFORMATION, title, message);
}

void Dialog::error(const std::string& title, const std::string& message) {
  showMessage(SDL_MESSAGEBOX_ERROR, title, message);
}

bool Dialog::confirm(const std::string& title, const std::string& message) {
  const std::array buttons{
      SDL_MessageBoxButtonData{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0,
                               "No"},
      SDL_MessageBoxButtonData{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1,
                               "Yes"},
  };

  const SDL_MessageBoxData data{
      SDL_MESSAGEBOX_WARNING,
      asw::display::get_window(),
      title.c_str(),
      message.c_str(),
      static_cast<int>(buttons.size()),
      buttons.data(),
      nullptr,
  };

  int pressed = 0;
  return SDL_ShowMessageBox(&data, &pressed) && pressed == 1;
}

void Dialog::requestFile(FileMode mode, const std::string& default_location) {
  static constexpr std::array filters{
      SDL_DialogFileFilter{"Levels", "xml"},
      SDL_DialogFileFilter{"All files", "*"},
  };

  {
    const std::scoped_lock lock(file_mutex);
    chosen_file.reset();
  }

  file_pending = true;

  if (mode == FileMode::Save) {
    SDL_ShowSaveFileDialog(onFileChosen, nullptr, asw::display::get_window(),
                           filters.data(), static_cast<int>(filters.size()),
                           default_location.c_str());
  } else {
    SDL_ShowOpenFileDialog(onFileChosen, nullptr, asw::display::get_window(),
                           filters.data(), static_cast<int>(filters.size()),
                           default_location.c_str(), false);
  }
}

bool Dialog::filePending() {
  return file_pending;
}

std::optional<std::string> Dialog::takeFile() {
  const std::scoped_lock lock(file_mutex);
  return std::exchange(chosen_file, std::nullopt);
}
