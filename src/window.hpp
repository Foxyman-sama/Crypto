#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <memory>

#include "crypto_strategies_binds.hpp"
#include "input.hpp"

class Alignment {
 public:
  static void center_by_width(float width) { ImGui::SetCursorPosX((ImGui::GetWindowSize().x - width) * 0.5f); }
};

class GLFWBackendWindow {
 public:
  void show(const char *title, int width, int height) {
    glfw_init();
    create_window(title, width, height);
    imgui_init();

    while (!glfwWindowShouldClose(window)) {
      handle_poll();
    }

    clear();
  }

 private:
  void glfw_init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  }

  void create_window(const char *title, int width, int height) {
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
  }

  void imgui_init() {
    ImGui::CreateContext();

    auto &context { ImGui::GetIO() };
    context.IniFilename = "";
    context.LogFilename = "";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
  }

  void handle_poll() {
    glfwPollEvents();
    create_frame();
    show_main_window();
    render();
  }

  void create_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  virtual void show_main_window() = 0;

  std::unique_ptr<char> get_input_text() {
    constexpr auto text_size { 100 };
    auto *text { new char[text_size] {} };
    constexpr auto elem_size { 200 };
    Alignment::center_by_width(elem_size);
    ImGui::SetNextItemWidth(elem_size);
    ImGui::InputText("###first", text, text_size);
    return std::unique_ptr<char> { text };
  }

  void render() {
    ImGui::Render();

    auto display_w { 0 };
    auto display_h { 0 };
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_h, display_h);
    glClearColor(1.00f, 1.00f, 1.00f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  void clear() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
  }

 protected:
  GLFWwindow *window;
};

class Window : public GLFWBackendWindow {
 public:
  Window(Input &input, DataView &data_view)
      : input { input },
        data_view { data_view },
        mode { WorkMode::ENCRYPTION },
        selected_crypto_strategy { crypto_strategies_binds[0] } {}

 private:
  enum class WorkMode { ENCRYPTION, DECRYPTION };

  void show_main_window() override {
    ImGui::SetNextWindowSize({ window_width, window_height });
    ImGui::Begin("Crypto", nullptr);

    show_crypto_input_table();
    show_settings_table();
    show_button();

    ImGui::End();
  }

  void show_crypto_input_table() {
    if (ImGui::BeginTable("CryptoInput", 2)) {
      ImGui::TableNextColumn();
      show_inputs_titles();

      ImGui::TableNextColumn();
      show_inputs();

      ImGui::EndTable();
    }
  }

  void show_inputs_titles() {
    ImGui::Text("Text for encrypting:");
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("Key for encrypting:");
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("Output:");
  }

  void show_inputs() {
    get_text_for_encryption();
    get_key();
    show_output_text();
  }

  void get_text_for_encryption() {
    ImGui::SetNextItemWidth(text_inputs_width);

    std::string temp { input_text };
    if (ImGui::InputText("###input_1", &temp)) {
      this->input_text = temp;
    }
  }

  void get_key() {
    ImGui::SetNextItemWidth(text_inputs_width);

    std::string temp { key };
    if (ImGui::InputText("###input_2", &temp)) {
      this->key = temp;
    }
  }

  void show_button() {
    ImGui::Spacing();
    Alignment::center_by_width(button_elem_width);
    if (ImGui::Button(button_text, { button_elem_width, button_elem_height })) {
      handle_input_text();
    }
  }

  void handle_input_text() {
    if (mode == WorkMode::ENCRYPTION) {
      input.encrypt(selected_crypto_strategy, input_text, key.c_str());
    } else {
      input.decrypt(selected_crypto_strategy, input_text, key.c_str());
    }
  }

  void show_output_text() {
    ImGui::SetNextItemWidth(text_inputs_width);
    ImGui::InputTextMultiline("###output", &data_view.output_text);
  }

  void show_settings_table() {
    if (ImGui::BeginTable("Settings", 2)) {
      ImGui::TableNextColumn();
      if (ImGui::TreeNode("Crypto Mode")) {
        show_crypto_selection();
        ImGui::TreePop();
      }

      ImGui::TableNextColumn();
      if (ImGui::TreeNode("Crypto Strategy")) {
        show_crypto_strategy_selection();
        ImGui::TreePop();
      }

      ImGui::EndTable();
    }
  }

  void show_crypto_selection() {
    static auto selected { 0 };

    if (ImGui::Selectable("Encrypt", selected == 0)) {
      mode = WorkMode::ENCRYPTION;
      selected = 0;
    } else if (ImGui::Selectable("Decrypt", selected == 1)) {
      mode = WorkMode::DECRYPTION;
      selected = 1;
    }
  }

  void show_crypto_strategy_selection() {
    static auto selected { 0 };

    for (auto i { 0 }; i < crypto_strategies_binds.size(); ++i) {
      if (ImGui::Selectable(crypto_strategies_binds[i].data(), selected == i)) {
        selected_crypto_strategy = crypto_strategies_binds[i];
        selected = i;
      }
    }
  }

  static constexpr auto text_inputs_width { 200 };

  static constexpr auto button_text { "Execute" };
  static constexpr auto button_elem_width { 200 };
  static constexpr auto button_elem_height { 20 };

  static constexpr auto window_width { 450 };
  static constexpr auto window_height { 300 };

  Input &input;
  DataView &data_view;

  std::string input_text;
  std::string key;

  WorkMode mode;
  std::string_view selected_crypto_strategy;
};

#endif